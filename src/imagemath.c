/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imagemath.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/16 01:46:11 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	pixel_axis(t_2Dhypersection sect, 
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

	pixel_axis(s.params2D, &x_axis, &y_axis);
	ret = q_add(s.params2D.center, q_by_scalar(x_axis, -1 - WIN_WIDTH / 2));
	ret = q_add(ret, q_by_scalar(y_axis, y - WIN_HEIGHT / 2));
	ret = q_add(ret, q_by_scalar(x_axis, x));
	return (ret);
}
