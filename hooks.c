/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 22:52:07 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/11 22:52:11 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include "mlx.h"
#include <libft.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int		exit_handler(t_view *view)
{
	mlx_destroy_window(view->mlx_ptr, view->win_ptr);
	exit(0);
	return (0);
}

int		key_handler(int key, t_view *view)
{
	if (key == 53)
		exit_handler(view);
	else if (key == 123 || key == 124 || key == 125 || key == 126)
		move_keys(view, key);
	return (0);
}

int		mouse_handler(int button, int x, int y, t_view *view)
{
	if (button == 1)
		view->fight_anim = 1;
	else if (button == 2)
	{
		view->fight_anim = 2;
		view->fight_tex = 17;
	}
	x = 0;
	y = 0;
	return (0);
}

int		loop_handler(t_view *view)
{
	static long		f_time = 0;
	long			cur;
	struct timeval	t;
	struct timeval	t2;

	gettimeofday(&t2, NULL);
	cur = (t2.tv_sec * 1000) + (t2.tv_usec / 1000);
	if (f_time == 0)
	{
		gettimeofday(&t, NULL);
		f_time = (t.tv_sec * 1000) + (t.tv_usec / 1000);
	}
	else if ((cur - f_time) >= 200)
	{
		enemy_anim(view);
		fight_anim(view);
		paint_world(view);
		f_time = 0;
	}
	view = (t_view *)view;
	return (0);
}
