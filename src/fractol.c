/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   voxel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 08:41:05 by fsusanna          #+#    #+#             */
/*   Updated: 2022/12/28 23:57:06 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	plot(t_sack s)
{
	int				n;
	int				x;
	int				y;
	unsigned int	*addr;

	addr = (unsigned int *)s.img.addr;
	n = -1;
	while (++n < s.points)
	{
		x = s.voxels[n].i * WIN_HEIGHT / 2 + WIN_WIDTH / 2;
		y = (s.voxels[n].k + 1) * WIN_HEIGHT / 2;
		*(addr + y * WIN_WIDTH + x) = 0x00ffffff;
/*		mlx_pixel_put(s.mlx, s.mlx_win, 
				s.voxels[n].i * WIN_HEIGHT / 2 + WIN_WIDTH / 2, 
				(s.voxels[n].k + 1) * WIN_HEIGHT / 2, 0x00ffffff);*/
	}
	mlx_put_image_to_window(s.mlx, s.mlx_win, s.img.img, 0, 0);
}

void	vsphere(t_sack *s)
{
	int		n;
	double	alpha1;
	double	alpha;

	alpha1 = M_PI * (3 - sqrt(5));
	n = -1;
	while (++n < s->points)
	{
		alpha = alpha1 * n;
		s->voxels[n].k = (1.0 - (double)(2 * n) / (s->points - 1)) * 0.9;
		s->voxels[n].i = cos(alpha) * sqrt(1 - s->voxels[n].k * s->voxels[n].k) * 0.9;
		s->voxels[n].j = sin(alpha) * sqrt(1 - s->voxels[n].k * s->voxels[n].k) * 0.9;
	}
	plot(*s);
}

void	initialise_s(t_sack *s)
{
	s->mlx = mlx_init();
	s->mlx_win = mlx_new_window(s->mlx, WIN_WIDTH, WIN_HEIGHT, "Prueba");
	s->img.img = mlx_new_image(s->mlx, WIN_WIDTH, WIN_HEIGHT);
	s->img.addr = mlx_get_data_addr(s->img.img, &s->img.bits_per_pixel,
			&s->img.line_length, &s->img.endian);
	s->mouse_buttons = 0;
	s->points = 200;
}

int closewin(int keycode)
{
	if (53 == keycode)
		exit(EXIT_SUCCESS);
	return (0);
}

int mouse_press(int button, int x, int y, t_sack *s)
{
	if (button < 4)
		s->mouse_buttons |= 1U<<button;
	return (x + y);
}

int mouse_release(int button, int x, int y, t_sack *s)
{
	s->mouse_buttons &= 255 ^ (1U<<button);
	return (x + y);
}

void	rotate(t_sack *s, t_quaternion rot)
{
	int				n;
	t_quaternion	tmp;
	double			aux;
	int				x;
	int				y;
	unsigned int	*addr;

	addr = (unsigned int *)s->img.addr;
	aux = rot.r * rot.r * 2 - 1;
	n = -1;
	while (++n < s->points)
	{
		tmp = s->voxels[n];
		x = tmp.i * WIN_HEIGHT / 2 + WIN_WIDTH / 2;
		y = (tmp.k + 1) * WIN_HEIGHT / 2;
		*(addr + y * WIN_WIDTH + x) = 0x0;
		s->voxels[n].i = tmp.i * (2 * rot.i * rot.i + aux) 
			+ 2 * rot.i * (rot.j * tmp.j + rot.k * tmp.k)
			+ 2 * rot.r * (rot.j * tmp.k - rot.k * tmp.j);
		s->voxels[n].j = tmp.j * (2 * rot.j * rot.j + aux) 
			+ 2 * rot.j * (rot.k * tmp.k + rot.i * tmp.i)
			+ 2 * rot.r * (rot.k * tmp.i - rot.i * tmp.k);
		s->voxels[n].k = tmp.k * (2 * rot.k * rot.k + aux) 
			+ 2 * rot.k * (rot.i * tmp.i + rot.j * tmp.j)
			+ 2 * rot.r * (rot.i * tmp.j - rot.j * tmp.i);
	}
}

int	mouse_move(int x, int y, t_sack *s)
{
	t_quaternion	rotation;

	if (!(s->mouse_buttons))
		return (0);
	rotation.r = 5100.0 / 5101.0;
	rotation.i = 101.0 / 5101.0 * 0.8;
	rotation.j = 101.0 / 5101.0 * 0.6;
	rotation.k = 0.0;
	rotate(s, rotation);
	plot(*s);
	return (x + y);
}

int	main(void)
{
	t_sack			s;

	initialise_s(&s);
	vsphere(&s);
	mlx_hook(s.mlx_win, 2, 1L<<0, closewin, NULL);
	mlx_hook(s.mlx_win, 4, 1L<<2, mouse_press, &s);
	mlx_hook(s.mlx_win, 5, 1L<<3, mouse_release, &s);
	mlx_hook(s.mlx_win, 6, 1L<<6, mouse_move, &s);
/*	project2D(s, 1);*/
	mlx_loop(s.mlx);
	return (0);
	
}
