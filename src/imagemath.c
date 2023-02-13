/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imagemath.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/13 23:21:45 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	pixel_axis(t_2dhypersection sect,
		t_quaternion *x_axis, t_quaternion *y_axis)
{
	double	sc;

	sc = 4.0 / (WIN_WIDTH * sect.zoom);
	*x_axis = q_by_scalar(sect.base.x, sc);
	*y_axis = q_by_scalar(sect.base.y, sc);
}

t_quaternion	pixel_to_quat(t_pixel p, t_sack s)
{
	t_quaternion	x_axis;
	t_quaternion	y_axis;
	t_quaternion	ret;

	pixel_axis(s.params2d, &x_axis, &y_axis);
	ret = q_add(s.params2d.center, q_by_scalar(x_axis, p.x - WIN_WIDTH / 2));
	ret = q_add(ret, q_by_scalar(y_axis, -(p.y - WIN_HEIGHT / 2)));
	return (ret);
}

t_quaternion	pass_center(t_pixel p, t_sack s)
{
	t_pixel	c0;

	c0.x = WIN_WIDTH / 2;
	c0.y = WIN_HEIGHT / 2;
	return (q_add(pxl_other(p, s), pxl_other(c0, *(t_sack *)(s.other))));
}

t_quaternion	pxl_other(t_pixel p, t_sack s)
{
	t_quaternion	xy_q;
	t_quaternion	r;

	xy_q = pixel_to_quat(p, s);
	r = q_add(
			q_by_scalar(s.params2d.base.x, dot_prod(xy_q, s.params2d.base.x)),
			q_by_scalar(s.params2d.base.y, dot_prod(xy_q, s.params2d.base.y)));
	return (r);
}

void	iter(t_quaternion *zc)
{
	t_quaternion	tmp;

	tmp = *zc;
	zc->r = tmp.r * tmp.r - tmp.i * tmp.i + tmp.j;
	zc->i = tmp.r * tmp.i * 2 + tmp.k;
}
