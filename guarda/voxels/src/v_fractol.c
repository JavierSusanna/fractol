/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   voxel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 08:41:05 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/16 18:51:23 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../v_fractol.h"

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
	while (++n < (s.cloud)->points)
	{
		x = (s.cloud)->voxels[n].i * WIN_HEIGHT / 2 + WIN_WIDTH / 2;
		y = ((s.cloud)->voxels[n].k + 1) * WIN_HEIGHT / 2;
		if ((s.cloud)->voxels[n].j < 0)
			*(addr + y * WIN_WIDTH + x) = col;
/*		else
			*(addr + y * WIN_WIDTH + x) = col / 15 * 8;*/
/*		mlx_pixel_put(s.mlx, s.mlx_win, 
				s.cloud.voxels[n].i * WIN_HEIGHT / 2 + WIN_WIDTH / 2, 
				(s.cloud.voxels[n].k + 1) * WIN_HEIGHT / 2, 0x00ffffff);*/
	}
	mlx_put_image_to_window(s.mlx, s.mlx_win, s.img.img, 0, 0);
}

void	vsphere(t_sack *s)
{
	int		n;
	double	alpha1;
	double	alpha;
	double	n1;
	double	n2;

	alpha1 = M_PI * (3 - sqrt(5));
	n1 = s->num/10946.0 * ((s->cloud)->points - 1);
	n = -1;
	while (++n < (s->cloud)->points)
	{
		alpha = alpha1 * n;
		n2 = (((double)n * n1) - ((s->cloud)->points - 1) * (int)((double)n * n1 / ((s->cloud)->points - 1)));
		(s->cloud)->voxels[n].k = (1.0 - (double)(2 * n2) / ((s->cloud)->points - 1)) * 0.9;
		(s->cloud)->voxels[n].i = cos(alpha) * sqrt(0.81 - (s->cloud)->voxels[n].k * (s->cloud)->voxels[n].k);
		(s->cloud)->voxels[n].j = sin(alpha) * sqrt(0.81 - (s->cloud)->voxels[n].k * (s->cloud)->voxels[n].k);
	}
	plot(*s, 1);
}

void	initialise_s(t_sack *s)
{	
	s->mlx = mlx_init();
	s->mlx_win = mlx_new_window(s->mlx, WIN_WIDTH, WIN_HEIGHT, "Prueba");
	s->img.img = mlx_new_image(s->mlx, WIN_WIDTH, WIN_HEIGHT);
	s->img.addr = mlx_get_data_addr(s->img.img, &s->img.bits_per_pixel,
			&s->img.line_length, &s->img.endian);
	s->user.buttons = 0;
	(s->cloud)->points = 10946;
/*	s->num = 6764.0 * (1.0 + (3-sqrt(5))/2/10946);*/
	s->num = 1;
}

int closewin(int keycode, t_sack *s)
{
	printf("keycode: %d\n", keycode);
	if (53 == keycode)
		exit(EXIT_SUCCESS);
	if (12 == keycode)
		s->num += .010;
	if (0 == keycode)
		s->num -= .010;
	if (13 == keycode)
		s->num += .001;
	if (1 == keycode)
		s->num -= .001;
	printf("num: %f\n", s->num);
	plot(*s, 0);
	vsphere(s);
	return (0);
}

int mouse_press(int button, int x, int y, t_sack *s)
{
	if (button < 4)
		s->user.buttons |= 1U<<button;
	s->user.x = x;
	s->user.y = y;
	return (0);
}

int mouse_release(int button, int x, int y, t_sack *s)
{
	s->user.buttons &= 255 ^ (1U<<button);
	return (x + y);
}

void	rotate(t_sack *s, t_quaternion rot)
{
	int				n;
	t_quaternion	tmp;
	double			aux;

	aux = rot.r * rot.r * 2 - 1;
	n = -1;
	while (++n < (s->cloud)->points)
	{
		tmp = (s->cloud)->voxels[n];
		(s->cloud)->voxels[n].i = tmp.i * (2 * rot.i * rot.i + aux) 
			+ 2 * rot.i * (rot.j * tmp.j + rot.k * tmp.k)
			+ 2 * rot.r * (rot.j * tmp.k - rot.k * tmp.j);
		(s->cloud)->voxels[n].j = tmp.j * (2 * rot.j * rot.j + aux) 
			+ 2 * rot.j * (rot.k * tmp.k + rot.i * tmp.i)
			+ 2 * rot.r * (rot.k * tmp.i - rot.i * tmp.k);
		(s->cloud)->voxels[n].k = tmp.k * (2 * rot.k * rot.k + aux) 
			+ 2 * rot.k * (rot.i * tmp.i + rot.j * tmp.j)
			+ 2 * rot.r * (rot.i * tmp.j - rot.j * tmp.i);
	}
}

int	mouse_move(int x, int y, t_sack *s)
{
	t_quaternion	rotation;
	double			x1;
	double			y1;
	double			h1;
	double			costh;
	double			sinth;

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
	sinth = sqrt(1 - costh * costh);
	rotation.r = costh;
	rotation.i = -sinth * y1 / h1;
	rotation.j = 0;
	rotation.k = sinth * x1 / h1;
	plot(*s, 0);
	rotate(s, rotation);
	plot(*s, 1);
	return (0);
}

int	main(void)
{
	t_sack	s;
	t_cloud	cloud;

	s.cloud = &cloud;
	initialise_s(&s);
	vsphere(&s);
	mlx_hook(s.mlx_win, 2, 1L<<0, closewin, &s);
	mlx_hook(s.mlx_win, 4, 1L<<2, mouse_press, &s);
	mlx_hook(s.mlx_win, 5, 1L<<3, mouse_release, &s);
	mlx_hook(s.mlx_win, 6, 1L<<6, mouse_move, &s);
/*	project2D(s, 1);*/
	mlx_loop(s.mlx);
	return (0);
	
}
