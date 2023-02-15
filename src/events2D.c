/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/15 14:58:25 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

int	key_press(int keycode, t_sack *s)
{
	printf("Código de tecla: %d\n", keycode);

	if (CHG_BASE == keycode)
		chg_base(s);
	if (!(s->user.ln.p0.x) && !(s->user.ln.p0.y))
		s->user.ln.p0.x = -100;
	if (s->user.buttons & PR_LCAPS)
		chg_iter(s, keycode);
	else if (KEY_UP == keycode || KEY_DOWN == keycode
		|| KEY_RIGHT == keycode || KEY_LEFT == keycode)
		chg_view(s, keycode);
	project2d(*s, 1);
	if (ESC == keycode)
		exit(EXIT_SUCCESS);
	if (LEFT_CAPS == keycode)
		s->user.buttons |= PR_LCAPS;
	if (LEFT_CTRL == keycode)
		s->user.buttons |= OTHER_IMG;
	
	printf("zoom: [%f]\nmax_i: [%f]\n\n", s->params2d.zoom,
		s->params2d.max_i * log1p(s->params2d.zoom) / 4);
	return (0);
}

int	key_release(int keycode, t_sack *s)
{
	if (LEFT_CTRL == keycode)
		s->user.buttons &= 0xffffffff ^ OTHER_IMG;
	if (LEFT_CAPS == keycode)
		s->user.buttons &= 0xffffffff ^ PR_LCAPS;
	return (0);
}

int	mouse_press(int button, int x, int y, t_sack *s)
{
	if (y < 0)
		return (0);
	s->user.ln.p0.x = x;
	s->user.ln.p0.y = y;
	s->user.ln.p1 = s->user.ln.p0;
	if (5 == button)
		zoom_at(s->user.ln.p0, ZOOM_FACTOR, s);
	else if (4 == button)
		zoom_at(s->user.ln.p0, 1.0 / ZOOM_FACTOR, s);
	else
	{	
		s->user.buttons |= 1 << (button - 1);
		s->user.buttons &= 0xffffffff ^ DRAG_IMG;
	}
	if (2 == button)
	{
		((t_sack *)(s->other))->params2d.center = pass_center(s->user.ln.p0, *s);
/*		s->cloud->center = ((t_sack *)(s->other))->params2d.center;*/
		project2d(*(t_sack *)(s->other), 1);
	}
	project2d(*s, 1);
	return (0);
}

int	mouse_release(int button, int x, int y, t_sack *s)
{
	t_sack	*other;

	other = (t_sack *)(s->other);
	s->user.buttons &= 0xffffffff ^ (1 << (button - 1));
	if (1 == button)
	{
		if (!(s->user.buttons & DRAG_IMG))
		{
			s->params2d.center = pixel_to_quat(s->user.ln.p1, *s);
			project2d(*s, 1);
		}
		s->user.buttons &= 0xffffffff ^ DRAG_IMG;
	}
	else if (2 == button)
	{
		((t_sack *)(s->other))->params2d.center = pass_center(s->user.ln.p1, *s);
		other->params2d.base.z = q_add(((t_sack *)(s->other))->params2d.center,
				q_by_scalar(pass_center(s->user.ln.p0, *s), -1));
		pile3d(*other);
		project2d(*other, 1);
		((t_sack *)s->other3d)->params2d.zoom = 1;
		other->cloud->rot = QR;
		open_cloud(s->other3d);
	}
	return (x + y);
}

int	mouse_move(int x, int y, t_sack *s)
{
	t_pixel	p;

	if (-100 == s->user.ln.p0.x)
	{
		s->user.ln.p0.x = x;
		s->user.ln.p0.y = y;
	}
	if (1 & s->user.buttons)
	{
		s->user.buttons |= DRAG_IMG;
		p.x = s->img.width / 2 + s->user.ln.p1.x - x;
		p.y = s->img.height / 2 + s->user.ln.p1.y - y;
		s->params2d.center = pixel_to_quat(p, *s);
		project2d(*s, 1);
	}
	p.x = x;
	p.y = y;
	if (2 & s->user.buttons)
	{
		s->user.ln.p1 = p;
		project2d(*s, 1);
	}
	if ((OTHER_IMG | 2) & s->user.buttons)
	{
		((t_sack *)(s->other))->params2d.center = pass_center(p, *s);
		project2d(*((t_sack *)s->other), 1);
	}
	s->user.ln.p1 = p;
	return (0);
}
