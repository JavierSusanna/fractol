/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2022/12/12 02:06:04 by fsusanna         ###   ########.fr       */
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

unsigned int	color(t_quaternion point)
{
	unsigned int	i;
	t_quaternion	zc;

	zc = point;
	i = -1;
	while (++i < MAX_ITER && norm2(zc) < 4.0)
		iter(&zc);
	if (MAX_ITER == i)
		return (0x8000d000);
	else
	{
		i = i * 512 / MAX_ITER;
		return ((i & 7) * 32 + (i & 56) * 8192 + (i & 448) * 65536);
		/*return (i * 5 / MAX_ITER * 0x00323232);*/
	}

}

void	do_axis(t_2Dhypersection sect, 
		t_quaternion *x_axis, t_quaternion *y_axis)
{
	double	s;

	s = 4.0 / (WIN_WIDTH * sect.zoom);
	*x_axis = q_by_scalar(sect.x_vector, s);
	*y_axis = q_by_scalar(sect.y_vector, s);
}

int	project2D(t_sack *s)
{
	int				x;
	int				y;
	t_quaternion	x_axis;
	t_quaternion	y_axis;
	t_quaternion	point;
	
	/*falta verificar que x_vect e y_vect son perpendiculares y normales*/
	/*printf("Entra a crear imagen... ");*/
	do_axis(s->params2D, &x_axis, &y_axis);
	y = -1;
	while (++y < WIN_HEIGHT)
	{
		x = -1;
		while (++x < WIN_WIDTH)
		{
			point = q_add(s->params2D.center, q_by_scalar(x_axis, x - WIN_WIDTH / 2));
			point = q_add(point, q_by_scalar(y_axis, y - WIN_HEIGHT / 2));
			*(s->params2D.addr) = color(point);
			(s->params2D.addr)++;
		}
	}
	s->params2D.addr = (unsigned int *)s->img.addr;
	/*printf("centro c.Im: %f\n", s->params2D.center.k);*/
	mlx_put_image_to_window(s->mlx, s->mlx_win, s->img.img, 0, 0);
	point.j = s->params2D.center.k / 30;
	point.k = - s->params2D.center.j / 30;
	s->params2D.center.j += point.j;
	s->params2D.center.k += point.k;
	s->params2D.center.j /= 1.0005554;
	s->params2D.center.k /= 1.0005554;
	return (0);
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

t_2Dhypersection	initialise_2D(unsigned int *addr, double zoom)
{
	t_2Dhypersection	ret;

	ret.addr = addr;
	ret.x_vector.r = 0;
	ret.x_vector.i = 0;
	ret.x_vector.j = 0;
	ret.x_vector.k = 0;
	ret.y_vector.r = 0;
	ret.y_vector.i = 0;
	ret.y_vector.j = 0;
	ret.y_vector.k = 0;
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

int closewin(int keycode)
{
	printf("Código de tecla: %d\n", keycode);
	if (53 == keycode)
		exit(EXIT_SUCCESS);
	printf("No cerrando ventana\n");
	return (0);
}

int	main(int nargs, char **args)
{
	t_sack		s;
	t_complex	z;
	t_complex	c;

	if (nargs > 1)
	{
		initialise_s(&s);
		printf("bpp: %d\nline_length: %d\nendian: %d\n", s.img.bits_per_pixel, 
				s.img.line_length, s.img.endian);
		c.Re = 0;
		c.Im = 0;
		z.Re = 0;
		z.Im = 0;
		if (nargs > 3)
		{
			c.Re = ft_strtof(args[2]);
			c.Im = ft_strtof(args[3]);
		}
		if (c.Re * c.Re < 4 && c.Im * c.Im < 4)
		{
			s.params2D.center.r = z.Re;
			s.params2D.center.i = z.Im;
			s.params2D.center.j = c.Re;
			s.params2D.center.k = c.Im;
			printf("2Dhs.addr, img.addr= %p, %p\n", s.params2D.addr, s.img.addr);
			if ('j' == args[1][0])
			{
				s.params2D.x_vector.r = 1.0;
				s.params2D.y_vector.i = 1.0;
			}
			if ('m' == args[1][0])
			{
				s.params2D.x_vector.j = 1.0;
				s.params2D.y_vector.k = 1.0;
			}
			if ('j' == args[1][0] || 'm' == args[1][0])
			{
				mlx_hook(s.mlx_win, 2, 1L<<0, closewin, NULL);
				mlx_loop_hook(s.mlx, project2D, &s);
				printf("Sale de loop_hook\n");
				mlx_loop(s.mlx);
				return (0);
			}
		}
	}
	showhelp();
	return (-1);
}
