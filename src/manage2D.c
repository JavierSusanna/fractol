/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/14 17:49:14 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	clear_img(t_data img)
{
	int				n;
	unsigned int	*addr;

	addr = (unsigned int *)img.addr;
	n = -1;
	while (++n < img.width * img.height)
		*(addr++) = 0x0;
}

unsigned int	color(int scheme, t_quaternion point, t_sack s)
{
	unsigned int	i;
	t_quaternion	zc;
	double			max_i;

	if (-1 == scheme)
		return (0);
	max_i = s.params2d.max_i;
	zc = point;
	i = -1;
	while (++i < max_i && confined(zc))
		iter(&zc);
	if (i >= max_i)
		return (0x00000000);
	else
	{
		i = i * 511 / max_i + 1;
		return ((i & 7) * 32 + (i & 56) * 1024 + (i & 448) * 32768);
	}
}

int	project2d(t_sack s, int colors)
{
	t_pixel			p;
	unsigned int	*pixel_addr;

	pixel_addr = s.params2d.addr;
	p.y = -1;
	while (++p.y < WIN_HEIGHT)
	{
		p.x = -1;
		while (++p.x < WIN_WIDTH)
		{
			*(pixel_addr) = color(colors, pixel_to_quat(p, s), s);
			if (2 == colors)
				find_border(p, s);
			pixel_addr++;
		}
	}
	draw_ln(s.user.ln, s);
	mlx_put_image_to_window(s.mlx, s.mlx_win, s.img.img, 0, 0);
	mlx_do_sync(s.mlx);
	return (0);
}

void	draw_ln(t_line l, t_sack s)
{
	int		n;
	t_line	aux;

	n = -1;
	if (!(s.user.buttons & 2) || (!(l.p0.x - l.p1.x) && !(l.p0.y - l.p1.y))
			|| l.p1.x < 0 || l.p1.x >= s.img.width
			|| l.p1.y < 0 || l.p1.y >= s.img.height)
	   return ;
	aux.p0 = l.p1;
	aux.p1 = l.p0;
	if ((l.p0.x - l.p1.x) * (l.p0.x - l.p1.x) >
			(l.p0.y - l.p1.y) * (l.p0.y - l.p1.y))
	{
		if (l.p1.x < l.p0.x)
			draw_ln(aux, s);
		while (++n <= l.p1.x - l.p0.x)
			*(s.params2d.addr + (l.p0.x + n) + s.img.width
					* (l.p0.y + n * (l.p1.y - l.p0.y) / (l.p1.x - l.p0.x)))
				= 0x0000ffff;
	}
	else
	{
		if (l.p1.y < l.p0.y)
			draw_ln(aux, s);
		while (++n <= l.p1.y - l.p0.y)
			*(s.params2d.addr + (l.p0.y + n) * s.img.width
					+ (l.p0.x + n * (l.p1.x - l.p0.x) / (l.p1.y - l.p0.y)))
				= 0x0000ffff;
	}
}
