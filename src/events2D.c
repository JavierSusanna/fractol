/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/24 02:39:36 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

int key_press(int keycode, t_sack *s)
{
	printf("Código de tecla: %d\n", keycode);
	if (ESC == keycode)
		exit(EXIT_SUCCESS);
	if (LEFT_CTRL == keycode)
		s->user.buttons |= OTHER_IMG;
	if (RIGHT_CAPS == keycode)
		show_image(s->other);
	return (0);
}

int key_release(int keycode, t_sack *s)
{
	if (LEFT_CTRL == keycode)
		s->user.buttons &= 0xffffffff ^ OTHER_IMG;
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
		s->user.buttons |= 0xffffffff ^ DRAG_IMG;
	}
	project2D(*s, 1);
	return (0);
}

int mouse_release(int button, int x, int y, t_sack *s)
{
	s->user.buttons &= 0xffffffff ^ (1<<(button - 1));
	if (2 == button)
	{
		pile3D(*s);
		((t_sack *)s->other3D)->params2D.zoom = s->params2D.zoom;
		open_cloud(s->other3D);
		return (0);
	}
	if (!(s->user.buttons & DRAG_IMG ))
	{
		s->params2D.center = pixel_to_quat(x, y, *s);
		project2D(*s, 1);
	}
	s->user.buttons &= 0xffffffff ^ DRAG_IMG;
	s->user.buttons &= 0xffffffff ^ 1;
	return (0);
}

int mouse_move(int x, int y, t_sack *s)
{
	if (1 & s->user.buttons)
	{
		s->user.buttons |= DRAG_IMG;
		s->params2D.center = pixel_to_quat(s->img.width / 2 + s->user.x - x,
				s->img.height / 2 + s->user.y - y, *s);
		project2D(*s, 1);
	}
	if (OTHER_IMG & s->user.buttons)
	{
		((t_sack *)s->other)->params2D.center.j = pixel_to_quat(x, y, *s).j;
		((t_sack *)s->other)->params2D.center.k = pixel_to_quat(x, y, *s).k;
/*		switch_wins(s->other);*/
		show_image(s->other);
	}
	s->user.x = x;
	s->user.y = y;
	return (0);
}

void	switch_wins(t_sack *s)
{
	project2D(*s, -1);
	project2D(*s, 1);
}

void	show_image(t_sack *s)
{
	if ('J' == s->type)
	{
		s->params2D.x_vector.r = 1.0;
		s->params2D.y_vector.i = 1.0;
	}
	if ('M' == s->type)
	{
		s->params2D.x_vector.j = 1.0;
		s->params2D.y_vector.k = 1.0;
	}
	mlx_hook(s->mlx_win, 2, 1L<<0, key_press, s);
	mlx_hook(s->mlx_win, 3, 1L<<1, key_release, s);
	mlx_hook(s->mlx_win, 4, 1L<<2, mouse_press, s);
	mlx_hook(s->mlx_win, 5, 1L<<3, mouse_release, s);
	mlx_hook(s->mlx_win, 6, 1L<<6, mouse_move, s);
	project2D(*s, 1);
/*	mlx_loop(s->mlx);*/
}
