/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 00:53:19 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/18 00:53:21 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include "mlx.h"
#include <libft.h>
#include <stdio.h>
#include <math.h>

int		dda(double rx, double ry, t_view *v)
{
	t_coord screen;
	int		side;

	screen.x = ((-1 + 2 * (rx < 0)) * (v->pos_x - v->map_x) +
										!(rx < 0)) * ABS(1 / rx);
	screen.y = ((-1 + 2 * (ry < 0)) * (v->pos_y - v->map_y) +
										!(ry < 0)) * ABS(1 / ry);
	while (!v->map.values[v->map_x][v->map_y])
	{
		if (screen.x < screen.y)
		{
			screen.x += ABS(1 / rx);
			v->map_x += -1 + 2 * !(rx < 0);
			v->map_x = (v->map_x >= v->map.w) ? v->map.w - 1 : v->map_x;
			side = !(rx > 0);
		}
		else
		{
			screen.y += ABS(1 / ry);
			v->map_y += (-1 + 2 * !(ry < 0));
			v->map_y = (v->map_y >= v->map.h) ? v->map.h - 1 : v->map_y;
			side = !(ry > 0) + 2;
		}
	}
	return (side);
}

void	main_cast(t_view *v, int x, double rx, double ry)
{
	double	wall_x;
	t_coord	floor_w;
	int		side;

	v->map_x = (int)(v->pos_x);
	v->map_y = (int)(v->pos_y);
	side = dda(rx, ry, v);
	if (side == 0 || side == 1)
		v->z_buffer[x] = (v->map_x - v->pos_x + (1 - !(rx < 0))) / rx;
	else
		v->z_buffer[x] = (v->map_y - v->pos_y + (1 - !(ry < 0))) / ry;
	if (side == 0 || side == 1)
		wall_x = v->pos_y + v->z_buffer[x] * ry;
	else
		wall_x = v->pos_x + v->z_buffer[x] * rx;
	wall_x -= floor((wall_x));
	v->tex_x = (int)(wall_x * (double)(v->t_width));
	if ((side == 0 && rx > 0) || (side == 3 && ry < 0))
		v->tex_x = v->t_width - v->tex_x - 1;
	floor_w.x = v->map_x + wall_x * !(side == 0 || side == 1)
							+ (rx < 0) * (side == 0 || side == 1);
	floor_w.y = v->map_y + (ry < 0) * !(side == 0 || side == 1)
							+ wall_x * (side == 0 || side == 1);
	draw_line(x, side, v);
	draw_floor(x, floor_w.x, floor_w.y, v);
}

void	render_sprites(t_view *v, double tr_y, t_list *s_list)
{
	int y;
	int d;
	int sprite_h;

	sprite_h = ABS((int)(v->s_height / (tr_y)));
	if ((v->d_start_x) > 0 < v->s_width && tr_y <
							v->z_buffer[(v->d_start_x)])
	{
		y = v->d_start_y;
		while (y < v->d_end_y)
		{
			d = (y++) * 256 - v->s_height * 128 + sprite_h * 128;
			v->tex_y = ((d * v->t_height) / sprite_h) / 256;
			v->alpha = 2;
			fill_image(v->sprites[((t_sprite *)s_list->content)->index].texture,
								v->d_start_x, y - 1, v);
		}
	}
}

t_coord	get_transform(t_view *v, double inv_det, t_list *s_list)
{
	t_coord		screen;
	t_coord		trans;

	screen.x = v->sprites[((t_sprite *)s_list->content)->index].x - v->pos_x;
	screen.y = v->sprites[((t_sprite *)s_list->content)->index].y - v->pos_y;
	trans.x = inv_det * (v->dir_y * screen.x - v->dir_x * screen.y);
	trans.y = inv_det * (-v->plane_y * screen.x + v->plane_x * screen.y);
	return (trans);
}
