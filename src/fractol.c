/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/09 19:52:56 by fsusanna         ###   ########.fr       */
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

void	showhelp()
{
	printf("Aquí ayuda\n");
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
