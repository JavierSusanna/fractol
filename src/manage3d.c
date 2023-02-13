/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/13 22:40:10 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	px_to_cloud(t_pixel p, t_sack s)
{
	t_quaternion	pt;

	if ((s.cloud)->points > MAX_POINTS)
		return ;
	pt = q_by_scalar(pixel_to_quat(p, s), s.params2d.zoom);
	s.cloud->voxels[s.cloud->points].r = 0;
	s.cloud->voxels[s.cloud->points].i = dot_prod(pt, s.params2d.base.x);
	s.cloud->voxels[s.cloud->points].j = dot_prod(pt, s.params2d.base.y);
	s.cloud->voxels[s.cloud->points].k = dot_prod(pt, s.params2d.base.z);
	(s.cloud)->points++;
}

void	find_border(t_pixel p, t_sack s)
{
	t_pixel			bp;
	int				n;
	unsigned int	*p_addr;

	p_addr = s.params2d.addr + WIN_WIDTH * p.y + p.x;
	n = -1;
	while (++n < 4)
	{
		bp.y = p.y + n / 3 - 1;
		bp.x = p.x + n % 3 - 1;
		if (bp.x > 0 && bp.x < WIN_WIDTH && bp.y > 0 && bp.y < WIN_HEIGHT)
		{
			if (*(p_addr) > 1 && !*(s.params2d.addr + WIN_WIDTH * bp.y + bp.x))
			{
				*(s.params2d.addr + WIN_WIDTH * bp.y + bp.x) = 1;
				px_to_cloud(bp, s);
			}
			else if (!*(p_addr) && *(s.params2d.addr + WIN_WIDTH * bp.y + bp.x) > 1)
			{
				*(p_addr) = 1;
				px_to_cloud(p, s);
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
	z_v = s.params2d.base.z;
	q_unit(&(s.params2d.base.z));
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
