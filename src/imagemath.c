/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imagemath.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/12 01:11:06 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	pixel_axis(t_2dhypersection sect,
		t_quaternion *x_axis, t_quaternion *y_axis)
{
	double	sc;

	sc = 4.0 / (WIN_WIDTH * sect.zoom);
	*x_axis = q_by_scalar(sect.x_vector, sc);
	*y_axis = q_by_scalar(sect.y_vector, sc);
}

t_quaternion	pixel_to_quat(int x, int y, t_sack s)
{
	t_quaternion	x_axis;
	t_quaternion	y_axis;
	t_quaternion	ret;

	pixel_axis(s.params2d, &x_axis, &y_axis);
	ret = q_add(s.params2d.center, q_by_scalar(x_axis, x - WIN_WIDTH / 2));
	ret = q_add(ret, q_by_scalar(y_axis, -(y - WIN_HEIGHT / 2)));
	return (ret);
}

void	chg_view(t_sack *s, int key)
{
	int	dx;
	int	dy;

	dx = 0;
	dy = 0;
	if (KEY_UP == key)
		dy = -10;
	if (KEY_DOWN == key)
		dy = 10;
	if (KEY_LEFT == key)
		dx = -10;
	if (KEY_RIGHT == key)
		dx = 10;
	s->params2d.center = pixel_to_quat(WIN_WIDTH / 2 + dx,
			WIN_HEIGHT / 2 + dy, *s);
}

void	chg_iter(t_sack *s, int key)
{
	if (KEY_UP == key)
		s->params2d.max_i *= 1.1;
	else if (KEY_DOWN == key && s->params2d.max_i > 10)
		s->params2d.max_i /= 1.1;
}

void	iter(t_quaternion *zc)
{
	t_quaternion	tmp;

	tmp = *zc;
	zc->r = tmp.r * tmp.r - tmp.i * tmp.i + tmp.j;
	zc->i = tmp.r * tmp.i * 2 + tmp.k;
}
