/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quatmath.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/16 19:35:16 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	q_unit(t_quaternion *q)
{
	double	norm;

	norm = pow(dot_prod(*q, *q), -0.5);
	*q = q_by_scalar(*q, norm);
}

void	plot(t_sack s, int paint)
{
	int				n;
	t_pixel			p;
	t_quaternion	tmp;
	unsigned int	*addr;

	addr = (unsigned int *)s.img.addr;
	n = -1;
	while (++n < s.cloud->points)
	{
		tmp = rotate(s.cloud->voxels[n], s.cloud->rot);
		p.x = tmp.i * s.img.height + s.params2d.zoom * s.img.width;
		p.y = tmp.k * s.img.height + s.params2d.zoom * s.img.height;
		p.x /= s.params2d.zoom * 2;
		p.y /= s.params2d.zoom * 2;
		if (p.x >= 0 && p.x < s.img.width && p.y >= 0 && p.y < s.img.height)
			*(addr + p.y * s.img.width + p.x)
				= 0x00ffffff * paint;
	}
	mlx_put_image_to_window(s.mlx, s.mlx_win, s.img.img, 0, 0);
}

t_quaternion	rotate(t_quaternion p, t_quaternion rot)
{
	p = q_by(rot, p);
	p = q_by(p, q_star(rot));
	return (p);
}

void	clear_img(t_data img)
{
	int				n;
	unsigned int	*addr;

	addr = (unsigned int *)img.addr;
	n = -1;
	while (++n < img.width * img.height)
		*(addr++) = 0x0;
}

void	open_cloud(t_sack *s)
{
	clear_img(s->img);
	mlx_hook(s->mlx_win, 2, 1L << 0, vkey_press, s);
	mlx_hook(s->mlx_win, 4, 1L << 2, vmouse_press, s);
	mlx_hook(s->mlx_win, 5, 1L << 3, vmouse_release, s);
	mlx_hook(s->mlx_win, 6, 1L << 6, vmouse_move, s);
	plot(*s, 1);
}
