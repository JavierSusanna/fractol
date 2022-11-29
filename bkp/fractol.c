/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2022/11/29 09:07:57 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	julia(t_complex c)
{
	write(1, "Aquí Julia\n", 12);
	printf("c.Re: %.20f\n", c.Re);
	printf("sizeof long double: %lu\n", sizeof(long double));
}

t_canvas	project(t_2Dhypersection sect)
{
	t_canvas	ret;

	ret.width = 256;
	ret.height = 256;
	ret.img = 0;
	sect.zoom++;
	return (ret);
}

void	mandel(t_complex center, float zoom)
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

int	main(int nargs, char **args)
{
	void	*mlx;
	void	*mlx_win;
	t_complex	c;

	if (nargs > 1)
	{
		mlx = mlx_init();
		mlx_win = mlx_new_window(mlx, 256, 256, "Prueba");
		mlx_loop(mlx);
		if ('j' == args[1][0])
		{
			c.Re = ft_strtof(args[2]);
			c.Im = ft_strtof(args[3]);
			if (c.Re != 12345 && c.Im != 12345)
			{
				julia(c);
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
