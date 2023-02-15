/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imagemath2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsusanna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 16:48:14 by fsusanna          #+#    #+#             */
/*   Updated: 2023/02/15 15:26:18 by fsusanna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fractol.h"

void	chg_base(t_sack *s)
{
	t_quaternion	tmp;

	tmp = s->params2d.base.y;
	if ('M' == s->type)
		chg_base((t_sack *)(s->other));
	else if (tmp.k)
	{
		s->params2d.base.y = QI;
		((t_sack *)(s->other))->params2d.base.y = QK;
	}
	else if (tmp.j)
	{
		s->params2d.base.y = QK;
		((t_sack *)(s->other))->params2d.base.x = QJ;
		((t_sack *)(s->other))->params2d.base.y = QI;
	}
	else if (tmp.i)
	{
		s->params2d.base.y = QJ;
		((t_sack *)(s->other))->params2d.base.x = QI;
	}
}

void	chg_view(t_sack *s, int key)
{
	t_pixel	dp;

	dp.x = 0;
	dp.y = 0;
	if (KEY_UP == key)
		dp.y = -10;
	if (KEY_DOWN == key)
		dp.y = 10;
	if (KEY_LEFT == key)
		dp.x = -10;
	if (KEY_RIGHT == key)
		dp.x = 10;
	dp.x += WIN_WIDTH / 2;
	dp.y += WIN_HEIGHT / 2;
	s->params2d.center = pixel_to_quat(dp, *s);
}

void	chg_iter(t_sack *s, int key)
{
	if (KEY_UP == key)
		s->params2d.max_i *= 1.1;
	else if (KEY_DOWN == key && s->params2d.max_i > 10)
		s->params2d.max_i /= 1.1;
}
