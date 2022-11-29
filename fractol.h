/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:24:05 by fsusanna          #+#    #+#             */
/*   Updated: 2022/11/29 17:54:03 by fsusanna         ###   ########.fr       */
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


# define WIN_WIDTH 500
# define WIN_HEIGHT 300
# define MAX_ITER 50


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

typedef struct	s_canvas
{
	unsigned int	width;
	unsigned int	height;
	t_pixel			**img;
}				t_canvas;

typedef struct	s_2Dhypersection
{
	t_quaternion	center;
	t_quaternion	x_vector;
	t_quaternion	y_vector;
	double			zoom;

}				t_2Dhypersection;

typedef struct	s_
{
}				t_;


#endif
