/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/09 13:54:15 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

unsigned int	color(int scheme, t_quaternion point)
{
	unsigned int	i;
	t_quaternion	zc;

	if (-1 == scheme)
		return (0);
	zc = point;
	i = -1;
	while (++i < MAX_ITER && norm2(zc) < 4.0)
		iter(&zc);
	if (0 == scheme)
	{
		if (MAX_ITER == i)
			return (0xfe000000);
		else
		{
			i = i * 5 / MAX_ITER;
			if (3 < i)
				return (0x00ffff00);
			else if (3 == i)
				return (0x00ff0000);
			else
				return (0xff000000);
			/*return ((i & 7) * 32 + (i & 56) * 8192 + (i & 448) * 65536
				   + ((255 - ((1024 + i)>>2))<<24));*/
			/*return (i * 5 / MAX_ITER * 0x00323232);*/
		}
	}
	else	
	{
		if (MAX_ITER == i)
			return (0x00000000);
		else
		{
			i = i * 512 / MAX_ITER;
			return ((i & 7) * 32 + (i & 56) * 8192 + (i & 448) * 65536);
			/*return (i * 5 / MAX_ITER * 0x00323232);*/
		}
	}
}

int	project2D(t_sack s, int colors)
{
	int				x;
	int				y;
	t_quaternion	x_axis;
	t_quaternion	y_axis;
	t_quaternion	x0;
	
	/*falta verificar que x_vect e y_vect son perpendiculares y unitarios*/
	pixel_axis(s.params2D, &x_axis, &y_axis);
	y = -1;
	while (++y < WIN_HEIGHT)
	{
		x0 = q_add(s.params2D.center, q_by_scalar(x_axis, -1 - WIN_WIDTH / 2));
		x0 = q_add(x0, q_by_scalar(y_axis, y - WIN_HEIGHT / 2));
		x = -1;
		while (++x < WIN_WIDTH)
			*(s.params2D.addr++) = color(colors, q_add(x0, q_by_scalar(x_axis, x)));
	}
/*	s->params2D.addr = (unsigned int *)s->img.addr;*/
	mlx_put_image_to_window(s.mlx, s.mlx_win, s.img.img, 0, 0);
	mlx_do_sync(s.mlx);
/*	point.j = s->params2D.center.k / 30;
	point.k = - s->params2D.center.j / 30;
	s->params2D.center.j += point.j;
	s->params2D.center.k += point.k;
	s->params2D.center.j /= 1.0005554;
	s->params2D.center.k /= 1.0005554;*/
	return (0);
}

void	pile3D(t_sack s)
{
	int				z;
	t_quaternion	z_axis;

	z_axis.r = -5.0 * s.params2D.x_vector.j;
	z_axis.i = 0;
	z_axis.j = 0;
	z_axis.k = -s.params2D.x_vector.r;

	project2D(s, -1);
	z_axis = q_by_scalar(z_axis, 4.0 / (WIN_WIDTH * s.params2D.zoom));
	z = 0;
	while (z < WIN_HEIGHT / 5)
	{
		printf("z.k = %f", s.params2D.center.k);
		printf("\r");
		project2D(s, 0);
		z++;
		s.params2D.center = q_add(s.params2D.center, z_axis);
	}
}

void	zoom_at(int x, int y, double zf, t_sack *s)
{
	t_quaternion	x_axis;
	t_quaternion	y_axis;

	pixel_axis(s->params2D, &x_axis, &y_axis);
	s->params2D.center = q_add(s->params2D.center,
			q_by_scalar(x_axis, (x - WIN_WIDTH / 2)));
	s->params2D.center = q_add(s->params2D.center,
			q_by_scalar(y_axis, (y - WIN_HEIGHT / 2)));
	if (zf != 1.0)
	{
		s->params2D.center = q_add(s->params2D.center,
				q_by_scalar(x_axis, (WIN_WIDTH / 2 - x) / zf));
		s->params2D.center = q_add(s->params2D.center,
				q_by_scalar(y_axis, (WIN_HEIGHT / 2 - y) / zf));
	}
	s->params2D.zoom *= zf;
}
