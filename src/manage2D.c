/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/11 10:49:16 by fsusanna         ###   ########.fr       */
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
	while (++i < max_i && norm2(zc) < 16.0)
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
	int				x;
	int				y;
	unsigned int	*pixel_addr;

	pixel_addr = s.params2d.addr;
	y = -1;
	while (++y < WIN_HEIGHT)
	{
		x = -1;
		while (++x < WIN_WIDTH)
		{
			*(pixel_addr) = color(colors, pixel_to_quat(x, y, s), s);
			if (2 == colors)
				find_border(x, y, s);
			pixel_addr++;
		}
	}
	mlx_put_image_to_window(s.mlx, s.mlx_win, s.img.img, 0, 0);
	mlx_do_sync(s.mlx);
	return (0);
}
