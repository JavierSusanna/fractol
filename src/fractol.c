/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2022/11/29 18:26:44 by fsusanna         ###   ########.fr       */
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

t_quaternion	q_by_s(t_quaternion q1, double s)
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
	int				i;
	t_quaternion	zc;

	zc = point;
	i = -1;
	while (++i < MAX_ITER && norm2(zc) < 4.0)
		iter(&zc);
	if (i < MAX_ITER)
		return (0x00000000);
	else
		return (i * 0x00050200);

}

void	do_axis(t_2Dhypersection sect, 
		t_quaternion *x_axis, t_quaternion *y_axis)
{
	double	s;

	s = 4.0 / (WIN_WIDTH * sect.zoom);
	*x_axis = q_by_s(sect.x_vector, s);
	*y_axis = q_by_s(sect.y_vector, s);
}

void	project2D(t_2Dhypersection sect, unsigned int *img_addr)
{
	int				x;
	int				y;
	t_quaternion	x_axis;
	t_quaternion	y_axis;
	t_quaternion	point;

	/*falta verificar que x_vect e y_vect son perpendiculares y normales*/
	do_axis(sect, &x_axis, &y_axis);
	y = -1;
	while (++y < WIN_HEIGHT)
	{
		x = -1;
		while (++x < WIN_WIDTH)
		{
			point = q_add(sect.center, q_by_s(x_axis, x - WIN_WIDTH / 2));
			point = q_add(point, q_by_s(y_axis, y - WIN_HEIGHT / 2));
			*img_addr = color(point);
			img_addr++;
		}
	}
}

void	julia(unsigned int *img_addr, t_complex c, 
		t_complex center, double zoom)
{
	t_2Dhypersection	sect;

	sect.center.r = center.Re;
	sect.center.i = center.Im;
	sect.center.j = c.Re;
	sect.center.k = c.Im;
	sect.x_vector.r = 1.0;
	sect.x_vector.i = 0;
	sect.x_vector.j = 0;
	sect.x_vector.k = 0;
	sect.y_vector.r = 0;
	sect.y_vector.i = 1.0;
	sect.y_vector.j = 0;
	sect.y_vector.k = 0;
	sect.zoom = zoom;
	project2D(sect, img_addr);
}

void	mandel(t_complex center, double zoom)
{
	t_2Dhypersection	sect;

	sect.center.r = 0;
	sect.center.i = 0;
	sect.center.j = center.Re;
	sect.center.k = center.Im;
	sect.x_vector.r = 0;
	sect.x_vector.i = 0;
	sect.x_vector.j = 1;
	sect.x_vector.k = 0;
	sect.y_vector.r = 0;
	sect.y_vector.i = 0;
	sect.y_vector.j = 0;
	sect.y_vector.k = 1;
	sect.zoom = zoom;
	write(1, "Aquí Mandel\n", 13);
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

void	gradient(unsigned int *img_addr)
{
	int		x;
	int		y;

	y = -1;
	while (++y < 300)
	{
		x = -1;
		while (++x < 500)
		{
			*img_addr = x * 256 / 500 * 0x10000 + 
				y * 256 / 300 * 0x100;
			img_addr++;
		}
	}
}

int	main(int nargs, char **args)
{
	void	*mlx;
	t_data	img;
	void	*mlx_win;
	t_complex	c;
	t_complex	z;

	if (nargs > 1)
	{
		mlx = mlx_init();
		mlx_win = mlx_new_window(mlx, WIN_WIDTH, WIN_HEIGHT, "Prueba");
		img.img = mlx_new_image(mlx, WIN_WIDTH, WIN_HEIGHT);
		img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, 
				&img.line_length, &img.endian);
		printf("bpp: %d\nline_length: %d\nendian: %d\n", img.bits_per_pixel, 
				img.line_length, img.endian);
/*		gradient((unsigned int*)img.addr);*/
		if ('j' == args[1][0])
		{
			c.Re = ft_strtof(args[2]);
			c.Im = ft_strtof(args[3]);
			z.Re = 0;
			z.Im = 0;
			if (c.Re != 12345 && c.Im != 12345)
			{
				julia((unsigned int*)img.addr, c, z, 1.0);
				mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
				mlx_loop(mlx);
				return (0);
			}
		}
		else if ('m' == args[1][0])
		{
			c.Re = 0;
			c.Im = 0;
			mandel(c, 256);
			return (0);
		}
	}
	showhelp();
	return (-1);
}
