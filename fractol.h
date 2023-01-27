/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:24:05 by fsusanna          #+#    #+#             */
/*   Updated: 2023/01/27 12:48:57 by fsusanna         ###   ########.fr       */
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


# define ESC 53
# define LEFT_CTRL 256
# define RIGHT_CAPS 258
# define OTHER_IMG	0b0000000001000000
# define DRAG_IMG	0b0000001000000000
# define MAX_ITER 250
# define WIN_WIDTH 800
# define WIN_HEIGHT 480
# define ZOOM_FACTOR 2


typedef struct	s_data 
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}				t_data;

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
	t_quaternion	z_vector;
	int				max_i;
	double			zoom;

}				t_2Dhypersection;

typedef struct	s_user_state
{
	unsigned int	buttons;
	int				x;
	int				y;
}				t_user_state;

typedef struct	s_cloud
{
	int				points;
	t_quaternion	rot;
	t_quaternion	center;
	t_quaternion	voxels[209460];
	unsigned int	color[209460];
}				t_cloud;

typedef struct	s_sack
{
	void				*mlx;
	void				*mlx_win;
	t_data				img;
	char				type;
	t_2Dhypersection	params2D;
	t_user_state		user;
	t_cloud				*cloud;
	double				num;
	void				*other;
	void				*other3D;
}				t_sack;

typedef struct	s_
{
}				t_;

t_quaternion		q_add(t_quaternion q1, t_quaternion q2);
t_quaternion		q_by(t_quaternion q1, t_quaternion q2);
t_quaternion		q_star(t_quaternion q);
t_quaternion		q_by_scalar(t_quaternion q1, double s);
t_quaternion		q_zero(void);
void				q_unit(t_quaternion *q);
double				norm2(t_quaternion z);
void				iter(t_quaternion *zc);
void				pixel_axis(t_2Dhypersection sect,
						t_quaternion *x_axis, t_quaternion *y_axis);
t_quaternion		pixel_to_quat(int x, int y, t_sack s);
void				clear_img(t_data img);
unsigned int		color(int scheme, t_quaternion point, t_sack s);
int					project2D(t_sack s, int colors);
void				pile3D(t_sack s);
void				switch_wins(t_sack *s);
void				zoom_at(int x, int y, double zf, t_sack *s);
int 				key_press(int keycode, t_sack *s);
int 				key_release(int keycode, t_sack *s);
int 				mouse_press(int button, int x, int y, t_sack *s);
int 				mouse_release(int button, int x, int y, t_sack *s);
int 				mouse_move(int x, int y, t_sack *s);
void				show_image(t_sack *s);
/*void				set_axis(t_sack *s);*/
/*void				show_julia(int x, int y, t_sack s);*/
double				ft_strtof(char *str);
void				showhelp();
t_2Dhypersection	initialise_2D(unsigned int *addr, double zoom);
void				initialise_s(t_sack *s, char *win_name);
void				plot(t_sack s, int paint);
t_quaternion		rotate(t_quaternion p, t_quaternion rot);
int					vmouse_press(int button, int x, int y, t_sack *s);
int					vmouse_release(int button, int x, int y, t_sack *s);
int					vmouse_move(int x, int y, t_sack *s);
void				open_cloud(t_sack *s);
int					main(int nargs, char **args);

#endif
