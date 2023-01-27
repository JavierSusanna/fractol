/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/27 13:09:52 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	clear_img(t_data img)
{
	int				n;
	unsigned int	*addr;

	addr = (unsigned int*)img.addr;
	n = -1;
	while (++n < img.width * img.height)
		*(addr++)  = 0x0;
}

unsigned int	color(int scheme, t_quaternion point, t_sack s)
{
	unsigned int	i;
	t_quaternion	zc;
	double			max_i;

	if (-1 == scheme)
		return (0);
	max_i = s.params2D.max_i * log1p(s.params2D.zoom) / 4;
	point = q_add(point, q_by_scalar(s.params2D.z_vector, (double)rand()/RAND_MAX-0.5));
	zc = point;
	i = -1;
	while (++i < max_i && norm2(zc) < 16.0)
		iter(&zc);
	if (i >= max_i)
		return (0x00000000);
	else
	{
		i = i * 512 / max_i;
		if (400 < i)
		{
			if (2 == scheme && (s.cloud)->points < 209000)
			{
				(s.cloud)->voxels[(s.cloud)->points] = point;
				if ('M' == s.type)
					s.cloud->voxels[s.cloud->points].i = s.cloud->voxels[s.cloud->points].i;
				else
				{
					s.cloud->voxels[s.cloud->points].j = s.cloud->voxels[s.cloud->points].r;
					s.cloud->voxels[s.cloud->points].k = (s.cloud->voxels[s.cloud->points].k - s.cloud->center.k) * 1;
				}
				(s.cloud)->points++;
			}
			return (0x00ffff00);
		}
/*		else if (3 == i)
			return (0x00ff0000);
		else
			return (0xff000000);*/
		return ((i & 7) * 32 + (i & 56) * 1024 + (i & 448) * 32768);
/*			   + ((255 - ((1024 + i)>>2))<<24));*/
		/*return (i * 5 / MAX_ITER * 0x00323232);*/
/*		return (i * 0x00323232);*/
	}
/*	}
	else	
	{
		if (MAX_ITER == i)
			return (0x00000000);
		else
		{
			i = i * 512 / MAX_ITER;
			return ((i & 7) * 32 + (i & 56) * 8192 + (i & 448) * 65536);
			/return (i * 5 / MAX_ITER * 0x00323232);/
		}
	}*/
}

int	project2D(t_sack s, int colors)
{
	int				x;
	int				y;

	y = -1;
	while (++y < WIN_HEIGHT)
	{
		x = -1;
		while (++x < WIN_WIDTH)
			*(s.params2D.addr++) = color(colors, pixel_to_quat(x, y, s), s);
	}
	mlx_put_image_to_window(s.mlx, s.mlx_win, s.img.img, 0, 0);
	mlx_do_sync(s.mlx);
	return (0);
}

void	pile3D(t_sack s)
{
	int		z;

	s.params2D.z_vector.r = 0;
	s.params2D.z_vector.i = -10.0 * s.params2D.x_vector.j;
	s.params2D.z_vector.j = 0;
	s.params2D.z_vector.k = -2 * s.params2D.x_vector.r;

	project2D(s, -1);
	s.cloud->center = s.params2D.center;
	s.params2D.z_vector = q_by_scalar(s.params2D.z_vector, 4.0 / (WIN_WIDTH * s.params2D.zoom));
	s.params2D.center = q_add(s.params2D.center, q_by_scalar(s.params2D.z_vector, -15));
	z = 0;
	s.cloud->points = 0;
	while (z < WIN_HEIGHT / 10)
	{
		project2D(s, 2);
		z++;
		s.params2D.center = q_add(s.params2D.center, s.params2D.z_vector);
	}
}

void	zoom_at(int x, int y, double zf, t_sack *s)
{
	t_quaternion	x_axis;
	t_quaternion	y_axis;

	project2D(*s, -1);
	pixel_axis(s->params2D, &x_axis, &y_axis);
	s->params2D.center = q_add(s->params2D.center,
			q_by_scalar(x_axis, (x - WIN_WIDTH / 2)));
	s->params2D.center = q_add(s->params2D.center,
			q_by_scalar(y_axis, (y - WIN_HEIGHT / 2)));
	if (zf != 1.0)
	{
		s->params2D.center = q_add(s->params2D.center,
				q_by_scalar(x_axis, (WIN_WIDTH / 2 - x) / zf));
		s->params2D.center = q_add(s->params2D.center,
				q_by_scalar(y_axis, (WIN_HEIGHT / 2 - y) / zf));
	}
	s->params2D.zoom *= zf;
}
