/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage2d.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/20 19:42:12 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	pixel_axis(t_2dhypersection sect,
		t_quaternion *x_axis, t_quaternion *y_axis, t_quaternion *z_axis)
{
	double	sc;

	sc = 4.0 / (WIN_WIDTH * sect.zoom);
	*z_axis = q_by_scalar(sect.base.z, sc);
	*x_axis = q_by_scalar(sect.base.x, sc);
	*y_axis = q_by_scalar(sect.base.y, sc);
}

t_quaternion	pixel_to_quat(t_pixel p, t_sack s)
{
	t_quaternion	x_axis;
	t_quaternion	y_axis;
	t_quaternion	ret;

	pixel_axis(s.params2d, &x_axis, &y_axis, &y_axis);
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

void	zoom_at(t_pixel p, double zf, t_sack *s)
{
	double	tmp;

	tmp = (double)p.x * (1.0 - 1.0 / zf) + (double)WIN_WIDTH / 2.0 / zf;
	p.x = (int)tmp;
	tmp = (double)p.y * (1.0 - 1.0 / zf) + (double)WIN_HEIGHT / 2.0 / zf;
	p.y = (int)tmp;
	s->params2d.center = pixel_to_quat(p, *s);
	s->params2d.zoom *= zf;
	p = (t_pixel){WIN_WIDTH / 2, WIN_HEIGHT / 2};
	s = (t_sack *)(s->other);
	tmp = (double)p.x * (1.0 - 1.0 / zf) + (double)WIN_WIDTH / 2.0 / zf;
	p.x = (int)tmp;
	tmp = (double)p.y * (1.0 - 1.0 / zf) + (double)WIN_HEIGHT / 2.0 / zf;
	p.y = (int)tmp;
	s->params2d.center = pixel_to_quat(p, *s);
	s->params2d.zoom *= zf;
}
