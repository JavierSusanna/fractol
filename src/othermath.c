/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quatmath.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/12 01:34:49 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	q_unit(t_quaternion *q)
{
	double	norm;

	norm = pow(dot_product(*q, *q), -0.5);
	*q = q_by_scalar(*q, norm);
}

double	norm2(t_quaternion z)
{
	return (z.r * z.r + z.i * z.i);
}

void	zoom_at(int x, int y, double zf, t_sack *s)
{
	s->params2d.center = pixel_to_quat(
			x * (1 - 1 / zf) + WIN_WIDTH / 2 / zf,
			y * (1 - 1 / zf) + WIN_HEIGHT / 2 / zf,
			*s);
	s->params2d.zoom *= zf;
}

void	plot(t_sack s, int paint)
{
	int				n;
	int				x;
	int				y;
	t_quaternion	tmp;
	unsigned int	*addr;

	addr = (unsigned int *)s.img.addr;
	n = -1;
	while (++n < s.cloud->points)
	{
		tmp = rotate(s.cloud->voxels[n], s.cloud->rot);
		x = tmp.i * s.img.height + s.params2d.zoom * s.img.width;
		y = tmp.k * s.img.height + s.params2d.zoom * s.img.height;
		x /= s.params2d.zoom * 2;
		y /= s.params2d.zoom * 2;
		if (x >= 0 && x < s.img.width && y >= 0 && y < s.img.height)
			*(addr + y * s.img.width + x) = 0x00ffffffU * paint;
	}
	mlx_put_image_to_window(s.mlx, s.mlx_win, s.img.img, 0, 0);
}

t_quaternion	rotate(t_quaternion p, t_quaternion rot)
{
	p = q_by(rot, p);
	p = q_by(p, q_star(rot));
	return (p);
}
