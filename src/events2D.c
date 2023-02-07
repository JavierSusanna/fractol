/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/07 01:58:10 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

int key_press(int keycode, t_sack *s)
{
	/*123=izq;124=dcha;125=abajo;126=arriba*/
	printf("Código de tecla: %d\n", keycode);
	if (LEFT_CAPS == keycode)
		s->user.buttons |= PR_LCAPS;
	if (KEY_UP == keycode)
	{
		if (s->user.buttons & PR_LCAPS)
			s->params2d.max_i *= 1.1;
		else if ('M' == s->type)
			s->params2d.center.k -= 0.2/s->params2d.zoom;
		else
			s->params2d.center.i -= 0.2/s->params2d.zoom;
	}
	if (KEY_DOWN == keycode)
	{
		if (s->user.buttons & PR_LCAPS)
			s->params2d.max_i /= 1.1;
		else if ('M' == s->type)
			s->params2d.center.k += 0.2/s->params2d.zoom;
		else
			s->params2d.center.i += 0.2/s->params2d.zoom;
	}
	if (KEY_LEFT == keycode)
	{
		if ('M' == s->type)
			s->params2d.center.j -= 0.2/s->params2d.zoom;
		else
			s->params2d.center.r -= 0.2/s->params2d.zoom;
	}
	if (KEY_RIGHT == keycode)
	{
		if ('M' == s->type)
			s->params2d.center.j += 0.2/s->params2d.zoom;
		else
			s->params2d.center.r += 0.2/s->params2d.zoom;
	}
	printf("zoom: [%f]\nmax_i: [%f]\n\n", s->params2d.zoom, s->params2d.max_i * log1p(s->params2d.zoom) / 4);
	project2d(*s, 1);
	if (ESC == keycode)
		exit(EXIT_SUCCESS);
	if (LEFT_CTRL == keycode)
		s->user.buttons |= OTHER_IMG;
	return (0);
}

int key_release(int keycode, t_sack *s)
{
	if (LEFT_CTRL == keycode)
		s->user.buttons &= 0xffffffff ^ OTHER_IMG;
	if (LEFT_CAPS == keycode)
		s->user.buttons &= 0xffffffff ^ PR_LCAPS;
	return (0);
}

int mouse_press(int button, int x, int y, t_sack *s)
{
	if (y < 0)
		return (0);
	s->user.x = x;
	s->user.y = y;
	if (5 == button)
		zoom_at(x, y, ZOOM_FACTOR, s);
	else if (4 == button)
		zoom_at(x, y, 1.0 / ZOOM_FACTOR, s);
	else
	{	
		s->user.buttons |= 1<<(button - 1);
		s->user.buttons &= 0xffffffff ^ DRAG_IMG;
	}
	project2d(*s, 1);
	return (0);
}

int mouse_release(int button, int x, int y, t_sack *s)
{
	s->user.buttons &= 0xffffffff ^ (1<<(button - 1));
	if (1 == button)
	{
		if (!(s->user.buttons & DRAG_IMG))
		{
			s->params2d.center = pixel_to_quat(x, y, *s);
			project2d(*s, 1);
		}
		s->user.buttons &= 0xffffffff ^ DRAG_IMG;
	}
	else if (2 == button)
	{
		pile3d(*s);
		((t_sack *)s->other3d)->params2d.zoom = s->params2d.zoom;
		s->cloud->rot = q_zero();
		s->cloud->rot.r = 1;
		open_cloud(s->other3d);
	}
	return (0);
}

int mouse_move(int x, int y, t_sack *s)
{
	if (1 & s->user.buttons)
	{
		s->user.buttons |= DRAG_IMG;
		s->params2d.center = pixel_to_quat(s->img.width / 2 + s->user.x - x,
				s->img.height / 2 + s->user.y - y, *s);
		project2d(*s, 1);
	}
	if (OTHER_IMG & s->user.buttons)
	{
		if ('M' == s->type)
		{
			((t_sack *)s->other)->params2d.center.j = pixel_to_quat(x, y, *s).j;
			((t_sack *)s->other)->params2d.center.k = pixel_to_quat(x, y, *s).k;
		}
		else
		{
			((t_sack *)s->other)->params2d.center.r = pixel_to_quat(x, y, *s).r;
			((t_sack *)s->other)->params2d.center.i = pixel_to_quat(x, y, *s).i;
		}
		project2d(*((t_sack *)s->other), 1);
	}
	s->user.x = x;
	s->user.y = y;
	return (0);
}
