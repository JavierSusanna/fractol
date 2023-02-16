/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:24:05 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/16 20:25:49 by fsusanna         ###   ########.fr       */
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
# define CHG_BASE 21
# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_LEFT 123
# define KEY_RIGHT 124
# define LEFT_CTRL 256
# define LEFT_CAPS 257
# define RIGHT_CAPS 258
# define PR_LCAPS	0b0000000001000000
# define OTHER_IMG	0b0000000010000000
# define DRAG_IMG	0b0000000100000000
# define MAX_ITER 50
# define WIN_WIDTH 400
# define WIN_HEIGHT 240
# define MAX_POINTS 230987
# define ZOOM_FACTOR 2
# define Q0 (t_quaternion){0, 0, 0, 0}
# define QR (t_quaternion){1, 0, 0, 0}
# define QI (t_quaternion){0, 1, 0, 0}
# define QJ (t_quaternion){0, 0, 1, 0}
# define QK (t_quaternion){0, 0, 0, 1}


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

typedef struct	s_base
{
	t_quaternion	x;
	t_quaternion	y;
	t_quaternion	z;

}				t_base;

typedef struct	s_2dhypersection
{
	unsigned int	*addr;
	t_quaternion	center;
	t_base			base;
	int				max_i;
	double			zoom;

}				t_2dhypersection;

typedef struct	s_pixel
{
	int	x;
	int	y;
}				t_pixel;

typedef struct	s_line
{
	t_pixel	p0;
	t_pixel	p1;
}				t_line;

typedef struct	s_user_state
{
	unsigned int	buttons;
	t_line			ln;
}				t_user_state;

typedef struct	s_cloud
{
	int				points;
	t_quaternion	rot;
	t_quaternion	center;
	t_quaternion	voxels[232000];
}				t_cloud;

typedef struct	s_sack
{
	void				*mlx;
	void				*mlx_win;
	t_data				img;
	char				type;
	t_2dhypersection	params2d;
	t_user_state		user;
	t_cloud				*cloud;
	void				*other;
	void				*other3d;
}				t_sack;


/*othermath.c*/
void				q_unit(t_quaternion *q);
void				zoom_at(t_pixel p, double zf, t_sack *s);
void				plot(t_sack s, int paint);
t_quaternion		rotate(t_quaternion p, t_quaternion rot);

/*imagemath.c*/
void				pixel_axis(t_2dhypersection sect,
						t_quaternion *x_axis, t_quaternion *y_axis,
						t_quaternion *z_axis);
t_quaternion		pixel_noise_quat(t_pixel p, t_sack s);
t_quaternion		pixel_to_quat(t_pixel p, t_sack s);
t_quaternion		pass_center(t_pixel p, t_sack s);
t_quaternion		pxl_other(t_pixel p, t_sack s);
void				iter(t_quaternion *zc);

/*manage3d.c*/
void				px_to_cloud(t_pixel p, t_sack s);
void				find_border(t_pixel p, t_sack s);
void				center_cloud(t_sack s);
void				pile3d(t_sack s);
void				open_cloud(t_sack *s);

/*manage2d.c*/
void				clear_img(t_data img);
unsigned int		color(int scheme, t_quaternion point, t_sack s);
int					project2d(t_sack s, int colors);
void				draw_ln(t_line l, t_sack s);


/*events3d.c*/
int 				vkey_press(int keycode, t_sack *s);
int					vmouse_press(int button, int x, int y, t_sack *s);
int					vmouse_release(int button, int x, int y, t_sack *s);
int					vmouse_move(int x, int y, t_sack *s);

/*events2d2.c*/
void				chg_base(t_sack *s);
void				chg_view(t_sack *s, int key);
void				chg_iter(t_sack *s, int key);

/*events2d.c*/
int 				key_press(int keycode, t_sack *s);
int 				key_release(int keycode, t_sack *s);
int 				mouse_press(int button, int x, int y, t_sack *s);
int 				mouse_release(int button, int x, int y, t_sack *s);
int 				mouse_move(int x, int y, t_sack *s);

/*quatmath.c*/
t_quaternion		q_add(t_quaternion q1, t_quaternion q2);
t_quaternion		q_by(t_quaternion q1, t_quaternion q2);
double				dot_prod(t_quaternion q1, t_quaternion q2);
t_quaternion		q_by_scalar(t_quaternion q1, double s);
t_quaternion		q_star(t_quaternion q);

/*init.c*/
void				initialise_base(t_sack *s);
void				show_image(t_sack *s);
t_2dhypersection	initialise_2D(unsigned int *addr);
void				initialise_s(t_sack *s, char *win_name);
void				open_all(char type, double re, double im);

/*fractol.c*/
double				ft_strtof(char *str);
void				showhelp();
int					main(int nargs, char **args);

#endif
