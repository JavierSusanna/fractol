/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events3d.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 08:41:05 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/23 16:02:14 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

int	vkey_press(int keycode, t_sack *s)
{
	printf("keycode3D [%i]\n", keycode);
	if (ESC == keycode)
		exit(EXIT_SUCCESS);
	if (LEFT_CAPS == keycode)
		s->user.buttons |= PR_LCAPS;
	return (s->type);
}

int	vkey_release(int keycode, t_sack *s)
{
	if (LEFT_CAPS == keycode)
		s->user.buttons &= 0xffffffff ^ PR_LCAPS;
	return (0);
}

int	vmouse_press(int button, int x, int y, t_sack *s)
{
	printf("button3D [%i]\n", button);
	if (y < 0)
		return (0);
	if (button < 4)
		s->user.buttons |= 1U << (button - 1);
	else
	{
		plot(*s, 0);
		if (5 == button && s->cloud->z_eye > -10)
			s->cloud->z_eye *= 1.2;
		else if (4 == button)
			s->cloud->z_eye /= 1.2;
		plot(*s, 1);
	}
	s->user.ln.p0.x = x;
	s->user.ln.p0.y = y;
	return (0);
}

int	vmouse_release(int button, int x, int y, t_sack *s)
{
	s->user.buttons &= 0xffffffff ^ (1U << (button - 1));
	return (x + y);
}

int	vmouse_move(int x, int y, t_sack *s)
{
	t_quaternion	rotation;
	double			x1;
	double			y1;
	double			h1;
	double			costh;

	x1 = (double)(x - s->user.ln.p0.x);
	y1 = (double)(y - s->user.ln.p0.y);
	s->user.ln.p0 = (t_pixel){x, y};
	h1 = hypot(x1, y1);
	if (!(s->user.buttons & 3) || !h1)
		return (0);
	h1 /= 1.0 + 99.0 * (s->user.buttons & PR_LCAPS) / PR_LCAPS;
	costh = 1.0 - h1 / 10000;
	rotation.r = costh;
/*	h1 = (hypot(x1, y1) + 0.1);*/
	rotation.i = -sqrt(1 - costh * costh) * y1 / h1;
	rotation.j = 0;
	rotation.k = sqrt(1 - costh * costh) * x1 / h1;
	plot(*s, 0);
	s->cloud->rot = q_by(rotation, s->cloud->rot);
	q_unit(&(s->cloud->rot));
	plot(*s, 1);
	return (0);
}
