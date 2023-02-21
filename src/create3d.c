/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create3d.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/20 19:42:30 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	px_to_cloud(t_pixel p, t_sack s)
{
	t_quaternion	pt;

	if ((s.cloud)->points > MAX_POINTS)
		return ;
	pt = q_by_scalar(pixel_noise_quat(p, s), s.params2d.zoom);
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
		if (!in_win(bp.x, bp.y, s))
			continue ;
		if (*(p_addr) > 1 && !*(s.params2d.addr + WIN_WIDTH * bp.y + bp.x))
		{
			*(s.params2d.addr + WIN_WIDTH * bp.y + bp.x) = 1;
			px_to_cloud(bp, s);
		}
		else if (!*(p_addr)
			&& *(s.params2d.addr + WIN_WIDTH * bp.y + bp.x) > 1)
		{
			*(p_addr) = 1;
			px_to_cloud(p, s);
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

	c0 = pass_center(((t_sack *)(s.other))->user.ln.p0, *(t_sack *)(s.other));
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
		printf("%d, puntos: [%d]\r", z, s.cloud->points);
		z++;
		s.params2d.center = q_add(c0, q_by_scalar(z_v, part));
		project2d(s, 2);
		part += (sqrt(5) - 1) / 2;
		part = part - trunc(part);
	}
	center_cloud(s);
}

t_quaternion	pixel_noise_quat(t_pixel p, t_sack s)
{
	t_quaternion	x_axis;
	t_quaternion	y_axis;
	t_quaternion	z_axis;
	t_quaternion	ret;

	pixel_axis(s.params2d, &x_axis, &y_axis, &z_axis);
	ret = q_add(s.params2d.center, q_by_scalar(x_axis, p.x - WIN_WIDTH / 2));
	ret = q_add(ret, q_by_scalar(y_axis, -(p.y - WIN_HEIGHT / 2)));
	ret = q_add(ret, q_by_scalar(x_axis, ((double)rand()) / RAND_MAX - 0.5));
	ret = q_add(ret, q_by_scalar(y_axis, ((double)rand()) / RAND_MAX - 0.5));
	ret = q_add(ret, q_by_scalar(z_axis, (2.0 * rand()) / RAND_MAX - 1));
	return (ret);
}
