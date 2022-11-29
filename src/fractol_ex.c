/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2022/11/29 13:17:34 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	julia(unsigned int *img_addr, t_complex c, 
		t_complex center, double zoom)
{
	t_2Dhypersection	sect;

	sect.center.r = c.Re;
	sect.center.i = c.Im;
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
	
}

void	project2D(t_2Dhypersection sect, unsigned int *img_addr)
{
	int				x;
	int				y;
	t_quaternion	vx;
	t_quaternion	vy;

	/*verificar que x_vect e y_vect son perpendiculares y normales*/

	ret.width = 256;
	ret.height = 256;
	ret.img = 0;
	sect.zoom++;
	return (ret);
}

void	mandel(t_complex center, double zoom)
{
	t_canvas			canvas;
	t_2Dhypersection	sect;

	sect.center.r = center.Re;
	sect.center.i = center.Im;
	sect.center.j = 0;
	sect.center.k = 0;
	sect.x_vector.r = 1;
	sect.x_vector.i = 0;
	sect.x_vector.j = 0;
	sect.x_vector.k = 0;
	sect.y_vector.r = 0;
	sect.y_vector.i = 1;
	sect.y_vector.j = 0;
	sect.y_vector.k = 0;
	sect.zoom = zoom;
	canvas = project(sect); 
	write(1, "Aquí Mandel\n", 13);
}

double	ft_strtof(char *str)
{
	double	ret;
	double	tmp;
	int		i;
	int		decimals;

	ret = 0;
	tmp = 0;
	i = 0;
	decimals = 0;
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
	return (ret + tmp);
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

	if (nargs > 1)
	{
		mlx = mlx_init();
		mlx_win = mlx_new_window(mlx, WIN_WIDTH, WIN_HEIGHT, "Prueba");
		img.img = mlx_new_image(mlx, 500, 300);
		img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, 
				&img.line_length, &img.endian);
		printf("bpp: %d\nline_length: %d\nendian: %d\n", img.bits_per_pixel, 
				img.line_length, img.endian);
/*		gradient((unsigned int*)img.addr);*/
		if ('j' == args[1][0])
		{
			c.Re = ft_strtof(args[2]);
			c.Im = ft_strtof(args[3]);
			if (c.Re != 12345 && c.Im != 12345)
			{
				julia((unsigned int*)img.addr, c);
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
