/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/07 13:36:21 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

double	ft_strtof(char *str)
{
	double	ret;
	double	sign;

	ret = 0;
	sign = 1;
	if ('-' == *str)
		sign = -1;
	if ('-' == *str)
		str++;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			ret = ret * 10 + (*str - '0');
		else if (!('.' == *str) || (1 > sign && -1 < sign))
			return (12345);
		if ('.' == *str || (1 > sign && -1 < sign))
			sign /= 10;
		str++;
	}
	if (1 > sign && -1 < sign)
			sign *= 10;
	return (sign * ret);
}

void	showhelp(void)
{
	printf("\n***********HELP************\n");
	printf("./fractal m shows Mandelbrot Set\n");
	printf("./fractal j 0.0 0.7 shows Julia Set\n");
	printf("Mouse wheel to zoom in or out.\n");
	printf("Arrows or click and drag mouse to move fractal.\n");
	printf("Left ctrl key and move mouse over a set ");
	printf("to change the other set's parameter.\n");
	printf("Right click for 3D.\n");
	printf("*****************************\n");
}

int	main(int nargs, char **args)
{
	char	type;
	double	re;
	double	im;

	if (nargs > 1)
	{
		type = args[1][0];
		if ('m' == type || 'j' == type)
		{
			re = 0;
			im = 0;
			if (nargs > 3)
			{
				re = ft_strtof(args[2]);
				im = ft_strtof(args[3]);
			}
			if (re * re < 100 && im * im < 100)
				open_all(type, re, im);
		}
	}
	showhelp();
	return (-1);
}
