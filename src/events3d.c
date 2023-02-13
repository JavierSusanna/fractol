/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   voxels.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 08:41:05 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/13 21:45:48 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

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
	s->user.pxl.x = x;
	s->user.pxl.y = y;
	return (0);
}

int	vkey_press(int keycode, t_sack *s)
{
	if (ESC == keycode)
		exit(EXIT_SUCCESS);
	return (s->type);
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
	x1 = (double)(x - s->user.pxl.x);
	y1 = (double)(y - s->user.pxl.y);
	s->user.pxl.x = x;
	s->user.pxl.y = y;
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

void	open_cloud(t_sack *s)
{
	clear_img(s->img);
	mlx_hook(s->mlx_win, 2, 1L << 0, vkey_press, s);
	mlx_hook(s->mlx_win, 4, 1L << 2, vmouse_press, s);
	mlx_hook(s->mlx_win, 5, 1L << 3, vmouse_release, s);
	mlx_hook(s->mlx_win, 6, 1L << 6, vmouse_move, s);
	plot(*s, 1);
}
