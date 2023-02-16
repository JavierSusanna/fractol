/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quatmath.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/16 03:04:12 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	q_unit(t_quaternion *q)
{
	double	norm;

	norm = pow(dot_prod(*q, *q), -0.5);
	*q = q_by_scalar(*q, norm);
}

int	confined(t_quaternion z)
{
	return (dot_prod(z, z) < 36);
/*	return (z.r * z.r + z.i * z.i < 16);*/
}

void	zoom_at(t_pixel p, double zf, t_sack *s)
{
	double	tmp;

	tmp = (double)p.x * (1.0 - 1.0 / zf) + (double)WIN_WIDTH / 2.0 / zf;
	p.x = (int)tmp;
	tmp = (double)p.y * (1.0 - 1.0 / zf) + (double)WIN_HEIGHT / 2.0 / zf;
	p.y = (int)tmp;
	s->params2d.center = pixel_to_quat(p, *s);
	s->params2d.zoom *= zf;
}

void	enlighten(t_sack s)
{
	int				n;
	t_pixel			p;
	t_quaternion	tmp;
	int				*addr;
	int				count;

	addr = (int *)(&(s.cloud->shadow[0][0]));
	n = -1;
	while (++n < SH_W * SH_H)
	{
		*(addr + n) = -10000;
	}
	count = 0;
	n = -1;
	while (++n < s.cloud->points)
	{
		s.cloud->lit[n] = 1;
		tmp = rotate(s.cloud->voxels[n], s.cloud->rot_light);
		p.x = tmp.i * SH_H + 8 * SH_W;
		p.y = tmp.k * SH_H + 8 * SH_H;
		p.x /= 16;
		p.y /= 16;
		if (p.x >= 0 && p.x < SH_W && p.y >= 0 && p.y < SH_H
				&& *(addr + p.y * SH_W + p.x) < (tmp.j * 40))
		{
			*(addr + p.y * SH_W + p.x) = tmp.j * 40;
			count++;
		}
		else
			s.cloud->lit[n] = 0;
	}
	n = -1;
	while (++n < s.cloud->points)
	{
		if ((s.cloud->lit[n]))
		{
			tmp = rotate(s.cloud->voxels[n], s.cloud->rot_light);
			p.x = tmp.i * SH_H + 8 * SH_W;
			p.y = tmp.k * SH_H + 8 * SH_H;
			p.x /= 16;
			p.y /= 16;
			if (!(p.x >= 0 && p.x < SH_W && p.y >= 0 && p.y < SH_H
					&& *(addr + p.y * SH_W + p.x) <= (tmp.j * 40)))
			{
				s.cloud->lit[n] = 0;
				count--;
			}
		}
	}
}

void	plot(t_sack s, int paint)
{
	int				n;
	t_pixel			p;
	t_quaternion	tmp;
	unsigned int	*addr;

	enlighten(s);
	addr = (unsigned int *)s.img.addr;
	n = -1;
	while (++n < s.cloud->points)
	{
		if (!(s.cloud->lit[n]))
			continue ;
		tmp = rotate(s.cloud->voxels[n], s.cloud->rot);
		p.x = tmp.i * s.img.height + s.params2d.zoom * s.img.width;
		p.y = tmp.k * s.img.height + s.params2d.zoom * s.img.height;
		p.x /= s.params2d.zoom * 2;
		p.y /= s.params2d.zoom * 2;
		if (p.x >= 0 && p.x < s.img.width && p.y >= 0 && p.y < s.img.height)
			*(addr + p.y * s.img.width + p.x) = 
				(0x00a0ffa0U + 0x005f005fU * s.cloud->lit[n]) * paint;
	}
	mlx_put_image_to_window(s.mlx, s.mlx_win, s.img.img, 0, 0);
}

t_quaternion	rotate(t_quaternion p, t_quaternion rot)
{
	p = q_by(rot, p);
	p = q_by(p, q_star(rot));
	return (p);
}
