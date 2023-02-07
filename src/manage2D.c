/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/07 00:47:52 by fsusanna         ###   ########.fr       */
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
	max_i = s.params2d.max_i; /* * log1p(s.params2d.zoom) / 4;*/
/*	point = q_add(point, q_by_scalar(s.params2d.z_vector, (double)rand()/RAND_MAX-0.5));*/
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

void	px_to_cloud(int x, int y, t_sack s)
{
	t_quaternion	point;

	if ((s.cloud)->points > 209000)
		return ;
	point = pixel_to_quat(x, y, s);
	s.cloud->voxels[s.cloud->points] = point;
	if ('M' == s.type)
		s.cloud->voxels[s.cloud->points].i = s.cloud->voxels[s.cloud->points].i;
	else
	{
		s.cloud->voxels[s.cloud->points].j = s.cloud->voxels[s.cloud->points].r;
		s.cloud->voxels[s.cloud->points].k = (s.cloud->voxels[s.cloud->points].k - s.cloud->center.k) * 1;
	}
	(s.cloud)->points++;
}

void	find_border(int x, int y, t_sack s)
{
	int				bx;
	int				by;
	int				n;
	unsigned int	*pixel_addr;

	pixel_addr = s.params2d.addr + WIN_WIDTH * y +  x;
	if (*(pixel_addr) > 1)
	{
		n = -1;
		while (++n < 4)
		{
			by = y + n / 3 - 1;
			bx = x + n % 3 - 1;
			if (bx > 0 && bx < WIN_WIDTH && by > 0 && by < WIN_HEIGHT)
			{
				if (!*(s.params2d.addr + WIN_WIDTH * by + bx))
				{
					*(s.params2d.addr + WIN_WIDTH * by + bx) = 1;
					px_to_cloud(bx, by, s);
				}
			}
		}
	}
	else
	{
		n = -1;
		while (++n < 4)
		{
			by = y + n / 3 - 1;
			bx = x + n % 3 - 1;
			if (bx > 0 && bx < WIN_WIDTH && by > 0 && by < WIN_HEIGHT)
			{
				if (*(s.params2d.addr + WIN_WIDTH * by + bx) > 1)
				{
					*(pixel_addr) = 1;
					px_to_cloud(x, y, s);
				}
			}
		}
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

void	center_cloud(t_sack s)
{
	int				i;
	int				p;
	t_quaternion	sum;

	p = s.cloud->points;
	i = -1;
	while (++i < p)
		sum = q_add(sum, s.cloud->voxels[i]);
	sum = q_by_scalar(sum, -1.0/p);
	i = -1;
	while (++i < p)
		s.cloud->voxels[i] = q_add(s.cloud->voxels[i], sum);
}

void	pile3d(t_sack s)
{
	int		z;

	s.params2d.z_vector.r = 0;
	s.params2d.z_vector.i = -8.0 * s.params2d.x_vector.j;
	s.params2d.z_vector.j = 0;
	s.params2d.z_vector.k = -3.0 * s.params2d.x_vector.r;

	project2d(s, -1);
	s.cloud->center = s.params2d.center;
	s.params2d.z_vector = q_by_scalar(s.params2d.z_vector, 4.0 /WIN_WIDTH);
	s.params2d.center = q_add(s.params2d.center, q_by_scalar(s.params2d.z_vector, -0));
	z = 0;
	s.cloud->points = 0;
	while (s.cloud->points < 209000 && z < 200)
	{
		project2d(s, 2);
		printf("%d, puntos: [%d]\n", z, s.cloud->points);
		z++;
		s.params2d.center = q_add(s.params2d.center, s.params2d.z_vector);
		if (100 == z)
			s.params2d.center = q_add(s.params2d.center, q_by_scalar(s.params2d.z_vector, -80.618));

	}
	center_cloud(s);
}

void	zoom_at(int x, int y, double zf, t_sack *s)
{
	t_quaternion	x_axis;
	t_quaternion	y_axis;

	project2d(*s, -1);
	pixel_axis(s->params2d, &x_axis, &y_axis);
	s->params2d.center = q_add(s->params2d.center,
			q_by_scalar(x_axis, (x - WIN_WIDTH / 2)));
	s->params2d.center = q_add(s->params2d.center,
			q_by_scalar(y_axis, (y - WIN_HEIGHT / 2)));
	if (zf != 1.0)
	{
		s->params2d.center = q_add(s->params2d.center,
				q_by_scalar(x_axis, (WIN_WIDTH / 2 - x) / zf));
		s->params2d.center = q_add(s->params2d.center,
				q_by_scalar(y_axis, (WIN_HEIGHT / 2 - y) / zf));
	}
	s->params2d.zoom *= zf;
}
