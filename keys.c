/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 02:55:57 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/18 02:55:59 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include "mlx.h"
#include <libft.h>
#include <math.h>

void	move_keys(t_view *v, int key, double time)
{
	double			tmp;

	time *= -1 + 2 * (key == 126 || key == 123);
	if ((key == 125 || key == 126))
	{
		if (v->map.values[(int)(v->pos_x + v->dir_x * time * 15)]
												[(int)(v->pos_y)] == 0)
			v->pos_x += v->dir_x * time * 15;
		if (v->map.values[(int)(v->pos_x)]
						[(int)(v->pos_y + v->dir_y * time * 15)] == 0)
			v->pos_y += v->dir_y * time * 15;
	}
	if (key == 124 || key == 123)
	{
		tmp = v->dir_x;
		v->dir_x = v->dir_x * cos(time * 5) - v->dir_y * sin(time * 5);
		v->dir_y = tmp * sin(time * 5) + v->dir_y * cos(time * 5);
		tmp = v->plane_x;
		v->plane_x = v->plane_x * cos(time * 5) - v->plane_y * sin(time * 5);
		v->plane_y = tmp * sin(time * 5) + v->plane_y * cos(time * 5);
	}
}

