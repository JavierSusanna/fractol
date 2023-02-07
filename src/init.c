/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/07 02:02:45 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	show_image(t_sack *s)
{
	if ('J' == s->type)
	{
		s->params2d.x_vector.r = 1.0;
		s->params2d.y_vector.i = 1.0;
	}
	if ('M' == s->type)
	{
		s->params2d.x_vector.j = 1.0;
		s->params2d.y_vector.k = 1.0;
	}
	mlx_hook(s->mlx_win, 2, 1L<<0, key_press, s);
	mlx_hook(s->mlx_win, 3, 1L<<1, key_release, s);
	mlx_hook(s->mlx_win, 4, 1L<<2, mouse_press, s);
	mlx_hook(s->mlx_win, 5, 1L<<3, mouse_release, s);
	mlx_hook(s->mlx_win, 6, 1L<<6, mouse_move, s);
	project2d(*s, 1);
}

t_2dhypersection	initialise_2d(unsigned int *addr, double zoom)
{
	t_2dhypersection	ret;

	ret.addr = addr;
	ret.center = q_zero();
	ret.x_vector = q_zero();
	ret.y_vector = q_zero();
	ret.z_vector = q_zero();
	ret.max_i = MAX_ITER;
	ret.zoom = zoom;
	return (ret);
}

void	initialise_s(t_sack *s, char *win_name)
{
	int	dup;

	dup = 1;
	s->type = win_name[0];
	if ('3' == s->type)
		dup = 4;
	if ('M' == s->type)
	{
		((t_sack *)(s->other))->cloud = s->cloud;
		((t_sack *)(s->other3d))->cloud = s->cloud;
	}
	s->img.width = dup * WIN_WIDTH;
	s->img.height = dup * WIN_HEIGHT;
	s->mlx_win = mlx_new_window(s->mlx, s->img.width, s->img.height, win_name);
	s->img.img = mlx_new_image(s->mlx, s->img.width, s->img.height);
	s->img.addr = mlx_get_data_addr(s->img.img, &s->img.bits_per_pixel, 
			&s->img.line_length, &s->img.endian);
	s->cloud->points = 0;
	s->cloud->rot = q_zero();
	s->cloud->rot.r = 1;
	s->user.buttons = 0;
	if ('3' == s->type)
		s->num = 1;
	else
		s->params2d = initialise_2d((unsigned int*)s->img.addr, 1.0);
}

void	open_all(char type, double re, double im)
{
	t_sack			sj;
	t_sack			sm;
	t_sack			s3d;
	t_cloud			cloud;

	sj.mlx = mlx_init();
	sm.mlx = sj.mlx;
	s3d.mlx = sm.mlx;
	sm.cloud = &cloud;
	sj.other3d = (void*)&s3d;
	sm.other3d = (void*)&s3d;
	sm.other = (void*)&sj;
	sj.other = (void*)&sm;
	if ('m' == type)
		initialise_s(&sm, "Mandelbrot");
	initialise_s(&sj, "Julia");
	if ('j' == type)
		initialise_s(&sm, "Mandelbrot");
	initialise_s(&s3d, "3D");
	sj.params2d.center = q_zero();
	sj.params2d.center.j = re;
	sj.params2d.center.k = im;
	show_image(&sj);
	show_image(&sm);
	mlx_loop(sj.mlx);
}
