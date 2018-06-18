/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/15 20:35:45 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/15 20:35:55 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include "mlx.h"
#include <libft.h>
#include <stdio.h>

int		draw_line(int x, int side, t_view *v)
{
	int y_start;
	int y_end;
	int d;

	if ((y_start = -(int)(v->s_height / v->z_buffer[x])
								/ 2 + v->s_height / 2) < 0)
		y_start = 0;
	if ((y_end = (int)(v->s_height / v->z_buffer[x])
								/ 2 + v->s_height / 2) >= v->s_height)
		y_end = v->s_height - 1;
	while (y_start < y_end)
	{
		d = y_start * 256 - v->s_height * 128 +
							(int)(v->s_height / v->z_buffer[x]) * 128;
		v->tex_y = ((d * v->t_height) /
						(int)(v->s_height / v->z_buffer[x])) / 256;
		if (side == 2 || side == 3)
			v->alpha = 1;
		fill_image(side, x, y_start, v);
		y_start++;
	}
	return (0);
}

int		draw_floor(int x, double floor_x, double floor_y, t_view *v)
{
	double	weight;
	double	floor_x_tmp;
	double	floor_y_tmp;
	int		y;

	if ((y = (int)(v->s_height / v->z_buffer[x]) /
						2 + v->s_height / 2) >= v->s_height)
		y = v->s_height - 1;
	if (y++ < 0)
		y = v->s_height;
	while (y < v->s_height)
	{
		weight = (v->s_height / (2.0 * y - v->s_height)) / v->z_buffer[x];
		floor_x_tmp = weight * floor_x + (1.0 - weight) * v->pos_x;
		floor_y_tmp = weight * floor_y + (1.0 - weight) * v->pos_y;
		v->tex_x = (int)(floor_x_tmp * v->t_width) % v->t_width;
		v->tex_y = (int)(floor_y_tmp * v->t_height) % v->t_height;
		v->alpha = 1;
		fill_image(4, x, y, v);
		v->alpha = 1;
		fill_image(6, x, (v->s_height - y), v);
		y++;
	}
	return (0);
}

void	sprites_helper(t_view *v, int sprite_x, t_list *s_list, double tr_y)
{
	int sprite_h;
	int sprite_w;

	sprite_h = ABS((int)(v->s_height / (tr_y)));
	sprite_w = ABS((int)(v->s_width / (tr_y)));
	if ((v->d_start_y = -sprite_h / 2 + v->s_height / 2) < 0)
		v->d_start_y = 0;
	if ((v->d_end_y = sprite_h / 2 + v->s_height / 2) >= v->s_height)
		v->d_end_y = v->s_height - 1;
	if ((v->d_start_x = -sprite_w / 2 + sprite_x) < 0)
		v->d_start_x = 0;
	if ((v->d_end_x = sprite_w / 2 + sprite_x) >= v->s_width)
		v->d_end_x = v->s_width - 1;
	while (v->d_start_x < v->d_end_x)
	{
		v->tex_x = (int)(256 * (v->d_start_x - (-sprite_w / 2 + sprite_x))
												* v->t_width / sprite_w) / 256;
		render_sprites(v, tr_y, s_list);
		v->d_start_x++;
	}
}

void	draw_sprites(t_view *v, double inv_det)
{
	t_list	*s_list;
	t_list	*tmp;
	t_coord	trans;
	int		c;

	s_list = NULL;
	c = 0;
	while (c < v->num_sprites)
	{
		v->sprites[c].distance =
			((v->pos_x - v->sprites[c].x) * (v->pos_x - v->sprites[c].x)
				+ (v->pos_y - v->sprites[c].y) * (v->pos_y - v->sprites[c].y));
		ft_lstpushback(&(s_list), &(v->sprites[c++]), sizeof(t_sprite));
	}
	s_list = ft_lstsort(s_list, compare);
	while (s_list != NULL)
	{
		trans = get_transform(v, inv_det, s_list);
		if ((tmp = s_list) && trans.y > 0)
			sprites_helper(v, (int)((v->s_width / 2) * (1 + trans.x / trans.y)),
				s_list, trans.y);
		free(s_list->content);
		free(s_list);
		s_list = tmp->next;
	}
}

int		draw_fight(t_view *v, int sprite_x, int sprite_w, int sprite_h)
{
	int	stripe;
	int	y;

	if ((v->d_start_y = v->s_height - sprite_h) < 0)
		v->d_start_y = 0;
	if ((v->d_end_y = v->s_height) >= v->s_height)
		v->d_end_y = v->s_height - 1;
	if ((stripe = -sprite_w / 2 + sprite_x) < 0)
		stripe = 0;
	if ((v->d_end_x = sprite_w / 2 + sprite_x) >= v->s_width)
		v->d_end_x = v->s_width - 1;
	while (stripe < v->d_end_x)
	{
		v->tex_x = (int)(256 * (stripe++ - (-sprite_w / 2 + sprite_x))
									* 250 / sprite_w) / 256;
		y = v->d_start_y;
		while (y < v->d_end_y)
		{
			v->tex_y = (int)(256 * (y++ - (v->s_height - sprite_h))
									* 150 / sprite_h) / 256;
			v->alpha = 2;
			fill_image(v->fight_tex, stripe, y - 1, v);
		}
	}
	return (0);
}
