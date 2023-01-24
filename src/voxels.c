/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   voxels.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 08:41:05 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/24 12:40:25 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	plot(t_sack s, int paint)
{
	int				n;
	int				x;
	int				y;
	unsigned int	*addr;
	unsigned int	col;

	addr = (unsigned int *)s.img.addr;
	col = 0x00ffffff;
	if (!paint)
		col = 0x0;
	n = -1;
	while (++n < s.cloud->points)
	{
		x = s.cloud->voxels[n].i * s.img.height + s.img.width;
		y = (s.cloud->voxels[n].k + 1) * s.img.height;
		x /= 4 / s.params2D.zoom;
		y /= 4 / s.params2D.zoom;
		if (x >= 0 && x < s.img.width && y >= 0 && y < s.img.height)
			*(addr + y * s.img.width + x) = col;
	}
	mlx_put_image_to_window(s.mlx, s.mlx_win, s.img.img, 0, 0);
}

void	rotate(t_sack *s, t_quaternion rot)
{
	int				n;
	t_quaternion	tmp;
	double			aux;

	aux = rot.r * rot.r * 2 - 1;
	n = -1;
	while (++n < s->cloud->points)
	{
		tmp = s->cloud->voxels[n];
		s->cloud->voxels[n].i = tmp.i * (2 * rot.i * rot.i + aux) 
			+ 2 * rot.i * (rot.j * tmp.j + rot.k * tmp.k)
			+ 2 * rot.r * (rot.j * tmp.k - rot.k * tmp.j);
		s->cloud->voxels[n].j = tmp.j * (2 * rot.j * rot.j + aux) 
			+ 2 * rot.j * (rot.k * tmp.k + rot.i * tmp.i)
			+ 2 * rot.r * (rot.k * tmp.i - rot.i * tmp.k);
		s->cloud->voxels[n].k = tmp.k * (2 * rot.k * rot.k + aux) 
			+ 2 * rot.k * (rot.i * tmp.i + rot.j * tmp.j)
			+ 2 * rot.r * (rot.i * tmp.j - rot.j * tmp.i);
	}
}

int vmouse_press(int button, int x, int y, t_sack *s)
{
	if (y < 0)
		return (0);
	if (button < 4)
		s->user.buttons |= 1U<<button;
	s->user.x = x;
	s->user.y = y;
	return (0);
}

int vmouse_release(int button, int x, int y, t_sack *s)
{
	s->user.buttons &= 255 ^ (1U<<button);
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
	x1 = (double)(x - s->user.x);
	y1 = (double)(y - s->user.y);
	s->user.x = x;
	s->user.y = y;
	h1 = hypot(x1, y1);
	if (!h1)
		h1 = 1;
	costh = 1 - h1 / 10000;
	rotation.r = costh;
	rotation.i = -sqrt(1 - costh * costh) * y1 / h1;
	rotation.j = 0;
	rotation.k = sqrt(1 - costh * costh) * x1 / h1;
	plot(*s, 0);
	rotate(s, rotation);
	plot(*s, 1);
	return (0);
}

void	open_cloud(t_sack *s)
{
	clear_img(s->img);
	mlx_hook(s->mlx_win, 4, 1L<<2, vmouse_press, s);
	mlx_hook(s->mlx_win, 5, 1L<<3, vmouse_release, s);
	mlx_hook(s->mlx_win, 6, 1L<<6, vmouse_move, s);
	plot(*s, 1);
}
