/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/12 01:28:40 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	px_to_cloud(int x, int y, t_sack s)
{
	t_quaternion	pt;

	if ((s.cloud)->points > MAX_POINTS)
		return ;
	pt = pixel_to_quat(x, y, s);
	s.cloud->voxels[s.cloud->points].r = 0;
	s.cloud->voxels[s.cloud->points].i = dot_product(pt, s.params2d.x_vector);
	s.cloud->voxels[s.cloud->points].j = dot_product(pt, s.params2d.y_vector);
	s.cloud->voxels[s.cloud->points].k = dot_product(pt, s.params2d.z_vector);
	(s.cloud)->points++;
}

void	find_border(int x, int y, t_sack s)
{
	int				bx;
	int				by;
	int				n;
	unsigned int	*p_addr;

	p_addr = s.params2d.addr + WIN_WIDTH * y + x;
	n = -1;
	while (++n < 4)
	{
		by = y + n / 3 - 1;
		bx = x + n % 3 - 1;
		if (bx > 0 && bx < WIN_WIDTH && by > 0 && by < WIN_HEIGHT)
		{
			if (*(p_addr) > 1 && !*(s.params2d.addr + WIN_WIDTH * by + bx))
			{
				*(s.params2d.addr + WIN_WIDTH * by + bx) = 1;
				px_to_cloud(bx, by, s);
			}
			else if (!*(p_addr) && *(s.params2d.addr + WIN_WIDTH * by + bx) > 1)
			{
				*(p_addr) = 1;
				px_to_cloud(x, y, s);
			}
		}
	}
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
	sum = q_by_scalar(sum, -1.0 / p);
	i = -1;
	while (++i < p)
		s.cloud->voxels[i] = q_add(s.cloud->voxels[i], sum);
}

void	pile3d(t_sack s)
{
	int				z;
	t_quaternion	z_v;
	t_quaternion	c0;
	double			part;

	c0 = s.cloud->center;
	z_v = s.params2d.z_vector;
	q_unit(&(s.params2d.z_vector));
	project2d(s, -1);
	s.cloud->center = s.params2d.center;
	z = 0;
	s.cloud->points = 0;
	s.params2d.center = q_add(c0, z_v);
	project2d(s, 2);
	part = 0;
	while (s.cloud->points < MAX_POINTS && z < 200)
	{
		printf("%d, puntos: [%d]\n", z, s.cloud->points);
		z++;
		s.params2d.center = q_add(c0, q_by_scalar(z_v, part));
		project2d(s, 2);
		part += (sqrt(5) - 1) / 2;
		part = part - trunc(part);
	}
	center_cloud(s);
}
