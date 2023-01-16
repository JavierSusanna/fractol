/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/16 20:56:55 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

int key_press(int keycode, t_sack *s)
{
	printf("Código de tecla: %d\n", keycode);
	if (53 == keycode)
		exit(EXIT_SUCCESS);
	if (257 == keycode)
		s->user.buttons |= 128;
	if (258 == keycode)
		show_image(s->other);
	return (0);
}

int key_release(int keycode, t_sack *s)
{
	if (257 == keycode)
		s->user.buttons &= 0xffffffff ^ 128;
	return (0);
}

int mouse_press(int button, int x, int y, t_sack *s)
{
	if (5 == button)
		zoom_at(x, y, ZOOM_FACTOR, s);
	else if (4 == button)
		zoom_at(x, y, 1.0 / ZOOM_FACTOR, s);
	else if (2 == button && !(s->user.buttons & 128))
	{
		write(1, "PRB", 4);
		printf("Zoom: %f ", s->params2D.zoom);
		printf("at %f, %f\n", s->params2D.center.r, s->params2D.center.i);
		pile3D(*s);
		open_cloud(s->other3D);
		return (0);
	}
	else if (2 == button)
	{
		((t_sack *)s->other)->params2D.center = pixel_to_quat(x, y, *s);
		switch_wins(s->other);
/*		show_image(s->other);*/
	}
/*		show_julia(x, y, *s);*/
	else if (y >= 0)
		zoom_at(x, y, 1.0, s);
	project2D(*s, 1);
	return (0);
}

void	switch_wins(t_sack *s)
{
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
/*	mlx_hook(s->mlx_win, 5, 1L<<3, mouse_release, s);*/
/*	mlx_hook(s->mlx_win, 6, 1L<<6, mouse_move, s);*/
	project2D(*s, 1);
/*	mlx_loop(s->mlx);*/
}
