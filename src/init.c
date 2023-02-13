/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/13 13:19:15 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	initialise_base(t_sack *s)
{
	if ('J' == s->type)
	{
		s->params2d.base.x = QR;
		s->params2d.base.y = QI;
	}
	if ('M' == s->type)
	{
		s->params2d.base.x = QJ;
		s->params2d.base.y = QK;
	}
}

void	show_image(t_sack *s)
{
	initialise_base(s);
	mlx_hook(s->mlx_win, 2, 1L << 0, key_press, s);
	mlx_hook(s->mlx_win, 3, 1L << 1, key_release, s);
	mlx_hook(s->mlx_win, 4, 1L << 2, mouse_press, s);
	mlx_hook(s->mlx_win, 5, 1L << 3, mouse_release, s);
	mlx_hook(s->mlx_win, 6, 1L << 6, mouse_move, s);
	project2d(*s, 1);
}

t_2dhypersection	initialise_2d(unsigned int *addr)
{
	t_2dhypersection	ret;

	ret.addr = addr;
	ret.center = Q0;
	ret.base.x = Q0;
	ret.base.y = Q0;
	ret.base.z = Q0;
	ret.max_i = MAX_ITER;
	ret.zoom = 1.0;
	return (ret);
}

void	initialise_s(t_sack *s, char *win_name)
{
	int	mult;

	mult = 1;
	s->type = win_name[0];
	if ('3' == s->type)
		mult = 1200 / WIN_HEIGHT;
	if ('M' == s->type)
		((t_sack *)(s->other3d))->cloud = s->cloud;
	if ('J' == s->type)
		s->cloud = ((t_sack *)(s->other))->cloud;
	s->img.width = mult * WIN_WIDTH;
	s->img.height = mult * WIN_HEIGHT;
	s->mlx_win = mlx_new_window(s->mlx, s->img.width, s->img.height, win_name);
	s->img.img = mlx_new_image(s->mlx, s->img.width, s->img.height);
	s->img.addr = mlx_get_data_addr(s->img.img, &s->img.bits_per_pixel,
			&s->img.line_length, &s->img.endian);
	if ('3' != s->type)
	{
		s->cloud->points = 0;
		s->cloud->rot = Q0;
		s->cloud->rot.r = 1;
		s->user.buttons = 0;
		s->params2d = initialise_2d((unsigned int *)s->img.addr);
	}
}

void	open_all(char type, double re, double im)
{
	t_sack			sm;
	t_sack			sj;
	t_sack			s3d;
	t_cloud			cloud;

	sm.mlx = mlx_init();
	sj.mlx = sm.mlx;
	s3d.mlx = sm.mlx;
	sm.cloud = &cloud;
	sj.other3d = (void *)&s3d;
	sm.other3d = (void *)&s3d;
	sm.other = (void *)&sj;
	sj.other = (void *)&sm;
	initialise_s(&s3d, "3D");
	if ('m' == type)
		initialise_s(&sj, "Julia");
	initialise_s(&sm, "Mandelbrot");
	if ('j' == type)
		initialise_s(&sj, "Julia");
	sj.params2d.center = Q0;
	sj.params2d.center.j = re;
	sj.params2d.center.k = im;
	show_image(&sj);
	show_image(&sm);
	mlx_loop(sm.mlx);
}
