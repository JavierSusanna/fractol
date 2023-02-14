/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   voxels.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 08:41:05 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/14 12:37:03 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

int	vkey_press(int keycode, t_sack *s)
{
	if (ESC == keycode)
		exit(EXIT_SUCCESS);
	return (s->type);
}

int	vmouse_press(int button, int x, int y, t_sack *s)
{
	if (y < 0)
		return (0);
	if (button < 4)
		s->user.buttons |= 1U << button;
	else
	{
		plot(*s, 0);
		if (5 == button && s->params2d.zoom > 1)
			s->params2d.zoom -= 0.2;
		else if (4 == button)
			s->params2d.zoom += 0.2;
		plot(*s, 1);
		printf("zoom3d: [%f]\n", s->params2d.zoom);
	}
	s->user.ln.p0.x = x;
	s->user.ln.p0.y = y;
	return (0);
}

int	vmouse_release(int button, int x, int y, t_sack *s)
{
	s->user.buttons &= 255 ^ (1U << button);
	return (x + y);
}

int	vmouse_move(int x, int y, t_sack *s)
{
	t_quaternion	rotation;
	double			x1;
	double			y1;
	double			h1;
	double			costh;

	if (!(s->user.buttons))
		return (0);
	x1 = (double)(x - s->user.ln.p0.x);
	y1 = (double)(y - s->user.ln.p0.y);
	s->user.ln.p0.x = x;
	s->user.ln.p0.y = y;
	h1 = hypot(x1, y1);
	if (!h1)
		h1 = 1;
	costh = 1 - h1 / 10000;
	rotation.r = costh;
	rotation.i = -sqrt(1 - costh * costh) * y1 / h1;
	rotation.j = 0;
	rotation.k = sqrt(1 - costh * costh) * x1 / h1;
	plot(*s, 0);
	s->cloud->rot = q_by(rotation, s->cloud->rot);
	q_unit(&(s->cloud->rot));
	plot(*s, 1);
	return (0);
}
