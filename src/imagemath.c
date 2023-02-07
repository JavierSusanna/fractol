/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imagemath.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/06 21:03:05 by fsusanna         ###   ########.fr       */
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
	ret = q_add(s.params2d.center, q_by_scalar(x_axis, -1 - WIN_WIDTH / 2));
	ret = q_add(ret, q_by_scalar(y_axis, y - WIN_HEIGHT / 2));
	ret = q_add(ret, q_by_scalar(x_axis, x));
	return (ret);
}

/*void	set_axis(t_sack *s)
{
	if ('J' == s->type)
	{
		s->params2d.x_vector.r = 1.0;
		s->params2d.y_vector.i = 1.0;
	}
	if ('M' == s->type)
	{
		s->params2d.x_vector.j = 1.0;
		s->params2d.y_vector.k = 1.0;
	}
}*/
