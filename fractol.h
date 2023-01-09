/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:24:05 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/09 02:32:10 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <mlx.h>


# define WIN_WIDTH 1600
# define WIN_HEIGHT 960
# define MAX_ITER 50
# define ZOOM_FACTOR 2


typedef struct	s_data 
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_pixel
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
}				t_pixel;

typedef struct	s_complex
{
	double	Re;
	double	Im;
}				t_complex;

typedef struct	s_quaternion
{
	double	r;
	double	i;
	double	j;
	double	k;
}				t_quaternion;

typedef struct	s_2Dhypersection
{
	unsigned int	*addr;
	t_quaternion	center;
	t_quaternion	x_vector;
	t_quaternion	y_vector;
	double			zoom;

}				t_2Dhypersection;

typedef struct	s_sack
{
	void				*mlx;
	void				*mlx_win;
	t_data				img;
	t_2Dhypersection	params2D;
	unsigned int		mouse_buttons;
	int					mouse_x;
	int					mouse_y;
	int					points;
	t_quaternion		voxels[10000];
}				t_sack;

typedef struct	s_
{
}				t_;


#endif
