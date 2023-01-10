/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/09 13:09:07 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

t_quaternion	q_add(t_quaternion q1, t_quaternion q2)
{
	q1.r += q2.r;
	q1.i += q2.i;
	q1.j += q2.j;
	q1.k += q2.k;
	return (q1);
}

t_quaternion	q_by_scalar(t_quaternion q1, double s)
{
	q1.r *= s;
	q1.i *= s;
	q1.j *= s;
	q1.k *= s;
	return (q1);
}

double	norm2(t_quaternion z)
{
	return (z.r*z.r + z.i * z.i);
}

void	iter(t_quaternion *zc)
{
	t_quaternion	tmp;

	tmp = *zc;
	zc->r = tmp.r * tmp.r - tmp.i * tmp.i + tmp.j;
	zc->i = tmp.r * tmp.i * 2 + tmp.k;
}

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

void	pixel_axis(t_2Dhypersection sect, 
		t_quaternion *x_axis, t_quaternion *y_axis)
{
	double	sc;

	sc = 4.0 / (WIN_WIDTH * sect.zoom);
	*x_axis = q_by_scalar(sect.x_vector, sc);
	*y_axis = q_by_scalar(sect.y_vector, sc);
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

double	ft_strtof(char *str)
{
	double	ret;
	double	tmp;
	int		i;
	int		decimals;
	int		sign;

	ret = 0;
	tmp = 0;
	i = 0;
	decimals = 0;
	sign = 1;
	if ('-' == *str)
		sign = -1;
	if ('-' == *str)
		str++;
	while (*str)
	{
		if (decimals)
			decimals++;
		if (('.' == *str) && !decimals)
		{
			str++;
			if (!*str)
				return (ret);
			tmp = ret;
			ret = 0;
			decimals = 1;
		}
		if (*str < '0' || *str > '9')
			return (12345);
		ret = ret * 10 + (*str - '0');
		str++;
	}
	while (decimals--)
		ret /= 10;
	return (sign * (ret + tmp));
}

void	showhelp()
{
	printf("Aquí ayuda\n");
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

int closewin(int keycode)
{
	printf("Código de tecla: %d\n", keycode);
	if (53 == keycode)
		exit(EXIT_SUCCESS);
	printf("No cerrando ventana\n");
	return (0);
}

int mouse_action(int button, int x, int y, t_sack *s)
{
	if (5 == button)
		zoom_at(x, y, ZOOM_FACTOR, s);
	else if (4 == button)
		zoom_at(x, y, 1.0 / ZOOM_FACTOR, s);
	else if (2 == button)
	{
		printf("Zoom: %f ", s->params2D.zoom);
		printf("at %f, %f\n", s->params2D.center.r, s->params2D.center.i);
		pile3D(*s);
		return (0);
	}
	else
		zoom_at(x, y, 1.0, s);
	project2D(*s, 1);
	return (0);
}

t_2Dhypersection	initialise_2D(unsigned int *addr, double zoom)
{
	t_2Dhypersection	ret;

	ret.addr = addr;
	ret.center = q_by_scalar(ret.center, 0);
	ret.x_vector = q_by_scalar(ret.x_vector, 0);
	ret.y_vector = q_by_scalar(ret.y_vector, 0);
	ret.zoom = zoom;
	return (ret);
}

void	initialise_s(t_sack *s)
{
	s->mlx = mlx_init();
	s->mlx_win = mlx_new_window(s->mlx, WIN_WIDTH, WIN_HEIGHT, "Prueba");
	s->img.img = mlx_new_image(s->mlx, WIN_WIDTH, WIN_HEIGHT);
	s->img.addr = mlx_get_data_addr(s->img.img, &s->img.bits_per_pixel, 
			&s->img.line_length, &s->img.endian);
	s->params2D = initialise_2D((unsigned int*)s->img.addr, 1.0);
}

void	show_image(char type, t_sack *s)
{
	if ('j' == type)
	{
		s->params2D.x_vector.r = 1.0;
		s->params2D.y_vector.i = 1.0;
	}
	if ('m' == type)
	{
		s->params2D.x_vector.j = 1.0;
		s->params2D.y_vector.k = 1.0;
	}
	mlx_hook(s->mlx_win, 2, 1L<<0, closewin, NULL);
	mlx_mouse_hook(s->mlx_win, mouse_action, s);
	project2D(*s, 1);
	mlx_loop(s->mlx);
}

int	main(int nargs, char **args)
{
	t_sack			s;
	t_quaternion	zc;

	if (nargs > 1)
	{
		initialise_s(&s);
		printf("bpp: %d\nline_length: %d\nendian: %d\n", s.img.bits_per_pixel, 
				s.img.line_length, s.img.endian);
		zc = q_by_scalar(s.params2D.x_vector, 0);
		if (nargs > 3)
		{
			zc.j = ft_strtof(args[2]);
			zc.k = ft_strtof(args[3]);
		}
		s.params2D.center = zc;
		if ('j' == args[1][0] || 'm' == args[1][0])
			show_image(args[1][0], &s);
	}
	showhelp();
	return (-1);
}
