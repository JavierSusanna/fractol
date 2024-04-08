/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events2d.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/23 18:32:59 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

int	key_press(int keycode, t_sack *s)
{
	double slow;

	printf("keycode [%i]\n", keycode);
	if (!in_win(s->user.ln.p0.x, s->user.ln.p0.y, *s))
		return (0);
	if (INIT_VIEW == keycode)
		initialise_2d(s);
	if (CHG_BASE == keycode)
	{
		chg_base(s);
		project2d(*(t_sack *)(s->other), 1);
	}
	slow = (PR_LCAPS & s->user.buttons) / PR_LCAPS;
	if (ZOOM_IN == keycode)
		zoom_at((t_pixel){WIN_WIDTH / 2, WIN_HEIGHT / 2}, ZOOM_FACTOR - SL_Z * slow, s);
	else if (ZOOM_OUT == keycode)
		zoom_at((t_pixel){WIN_WIDTH / 2, WIN_HEIGHT / 2}, 1.0 / (ZOOM_FACTOR - SL_Z * slow), s);
	if (s->user.buttons & PR_LCAPS)
		chg_iter(s, keycode);
	else if (KEY_UP == keycode || KEY_DOWN == keycode
		|| KEY_RIGHT == keycode || KEY_LEFT == keycode)
		chg_view(s, keycode);
	project2d(*s, 1);
	project2d(*(t_sack *)s->other, 1);
	if (ESC == keycode)
		exit(EXIT_SUCCESS);
	if (LEFT_CAPS == keycode)
		s->user.buttons |= PR_LCAPS;
	if (LEFT_CTRL == keycode)
		s->user.buttons |= OTHER_IMG;
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
	double	slow;

	printf("button [%i]\n", button);
	if (!in_win(x, y, *s))
		return (0);
	slow = (PR_LCAPS & s->user.buttons) / PR_LCAPS;
	s->user.ln.p0 = (t_pixel){x, y};
	s->user.ln.p1 = s->user.ln.p0;
	if (4 == button || 6 == button)
		zoom_at(s->user.ln.p0, ZOOM_FACTOR - SL_Z * slow, s);
	else if (5 == button || 7 == button)
		zoom_at(s->user.ln.p0, 1.0 / (ZOOM_FACTOR - SL_Z * slow), s);
	else
	{	
		s->user.buttons |= 1 << (button - 1);
		s->user.buttons &= 0xffffffff ^ DRAG_IMG;
	}
	if (LEFT_BUTTON == button)
	{
		((t_sack *)(s->other))->params2d.center
			= pass_center(s->user.ln.p0, *s);
		project2d(*(t_sack *)(s->other), 1);
	}
	project2d(*s, 1);
	project2d(*(t_sack *)s->other, 1);
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
	else if (LEFT_BUTTON == button)
	{
		other->params2d.center = pass_center(s->user.ln.p1, *s);
		other->params2d.base.z = q_add(((t_sack *)(s->other))->params2d.center,
				q_by_scalar(pass_center(s->user.ln.p0, *s), -1));
		pile3d(*other);
		project2d(*other, 1);
		((t_sack *)s->other3d)->params2d.zoom = 1;
		other->cloud->rot = (t_quaternion){1, 0, 0, 0};
		open_cloud(s->other3d);
	}
	return (x + y);
}

int	mouse_move(int x, int y, t_sack *s)
{
	t_pixel	p;

	if (!in_win(x, y, *s) || (3 & s->user.buttons) == 3)
		return (0);
	if ((LEFT_BUTTON - 1) & s->user.buttons)
	{
		s->user.buttons |= DRAG_IMG;
		p.x = s->img.width / 2 + s->user.ln.p0.x - x;
		p.y = s->img.height / 2 + s->user.ln.p0.y - y;
		s->params2d.center = pixel_to_quat(p, *s);
	}
	p = (t_pixel){x, y};
	if (s->user.buttons & (0xffffffff ^ 2))
		s->user.ln.p0 = p;
	else
		s->user.ln.p1 = p;
	if (OTHER_IMG & s->user.buttons)
		((t_sack *)(s->other))->params2d.center = pass_center(p, *s);
	if (2 & s->user.buttons)
		((t_sack *)(s->other))->params2d.center = pass_center(p, *s);
	if ((OTHER_IMG | 2) & s->user.buttons)
		project2d(*((t_sack *)s->other), 1);
	if (3 & s->user.buttons)
		project2d(*s, 1);
	return (0);
}
