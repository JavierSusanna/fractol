/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/16 20:53:23 by fsusanna         ###   ########.fr       */
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

void	initialise_s(t_sack *s, char *win_name)
{
	s->type = win_name[0];
	s->mlx_win = mlx_new_window(s->mlx, WIN_WIDTH, WIN_HEIGHT, win_name);
	s->img.img = mlx_new_image(s->mlx, WIN_WIDTH, WIN_HEIGHT);
	s->img.addr = mlx_get_data_addr(s->img.img, &s->img.bits_per_pixel, 
			&s->img.line_length, &s->img.endian);
	(s->cloud)->points = 0;
	if ('3' == s->type)
		s->num = 1;
	else
		s->params2D = initialise_2D((unsigned int*)s->img.addr, 1.0);
}

int	main(int nargs, char **args)
{
	t_sack			sm;
	t_sack			sj;
	t_sack			s3D;
	t_quaternion	zc;
	t_cloud			cloud;

	if (nargs > 1)
	{
		sm.mlx = mlx_init();
		sj.mlx = sm.mlx;
		s3D.mlx = sm.mlx;
		sm.cloud = &cloud;
		sj.cloud = &cloud;
		s3D.cloud = &cloud;
		sj.other3D = (void*)&s3D;
		sm.other3D = (void*)&s3D;
		sm.other = (void*)&sj;
		sj.other = (void*)&sm;
		initialise_s(&sm, "Mandelbrot");
		initialise_s(&sj, "Julia");
		initialise_s(&s3D, "3D");
/*		printf("bpp: %d\nline_length: %d\nendian: %d\n", s.img.bits_per_pixel, 
				s.img.line_length, s.img.endian);*/
		zc = q_by_scalar(sm.params2D.x_vector, 0);
		if (nargs > 3)
		{
			zc.j = ft_strtof(args[2]);
			zc.k = ft_strtof(args[3]);
		}
		sj.params2D.center = zc;
		show_image(&sm);
		show_image(&sj);
		mlx_loop(sm.mlx);
	}
	showhelp();
	return (-1);
}
