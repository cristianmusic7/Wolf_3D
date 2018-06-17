/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 23:27:24 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/11 23:27:27 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include "mlx.h"
#include <libft.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int		fill_image(int index, int x, int y, int dark, t_view *v)
{
	unsigned int color;

	color = *(unsigned int *)(v->textures[index].addr +
					v->textures[index].s_line * v->tex_y +
						(v->tex_x * (v->textures[index].bpp / 8)));
	if (dark == 2 && ((color & 0xFFFFFF) == 0))
		return (0);
	else if (dark == 1)
		color = (color >> 1) & 8355711;
	color = color | (v->health << 16);
	*(int *)(v->img.addr + (x * (v->img.bpp / 8))
									+ (y * v->img.s_line)) = color;
	return (0);
}

int		dda(double rx, double ry, t_view *v)
{
	double	sx;
	double	sy;
	int		side;

	v->mapX = (int)v->posX;
	v->mapY = (int)v->posY;
	sx = ((-1 + 2 * (rx < 0)) * (v->posX - v->mapX) + !(rx < 0)) * ABS(1 / rx);
	sy = ((-1 + 2 * (ry < 0)) * (v->posY - v->mapY) + !(ry < 0)) * ABS(1 / ry);
	while (!v->map.values[v->mapX][v->mapY])
	{
		if (sx < sy)
		{
			sx += ABS(1 / rx);
			v->mapX += -1 + 2 * !(rx < 0);
			side = !(rx > 0);
		}
		else
		{
			sy += ABS(1 / ry);
			v->mapY += (-1 + 2 * !(ry < 0));
			side = !(ry > 0) + 2;
		}
	}
	return (side);
}

void	main_cast(t_view *v, int x, double rx, double ry)
{
	double	wall_x;
	double	floor_x;
	double	floor_y;
	int		side;

	side = dda(rx, ry, v);
	if (side == 0 || side == 1)
		v->z_buffer[x] = (v->mapX - v->posX + (1 - !(rx < 0))) / rx;
	else
		v->z_buffer[x] = (v->mapY - v->posY + (1 - !(ry < 0))) / ry;
	if (side == 0 || side == 1)
		wall_x = v->posY + v->z_buffer[x] * ry;
	else
		wall_x = v->posX + v->z_buffer[x] * rx;
	wall_x -= floor((wall_x));
	v->tex_x = (int)(wall_x * (double)(v->texWidth));
	if ((side == 0 && rx > 0) || (side == 3 && ry < 0))
		v->tex_x = v->texWidth - v->tex_x - 1;
	floor_x = v->mapX + wall_x * !(side == 0 || side == 1)
							+ (rx < 0) * (side == 0 || side == 1);
	floor_y = v->mapY + (ry < 0) * !(side == 0 || side == 1)
							+ wall_x * (side == 0 || side == 1);
	draw_line(x, side, v);
	draw_floor(x, floor_x, floor_y, v);
}

int		paint_world(t_view *v)
{
	int		x;
	double	ray_dir_x;
	double	ray_dir_y;

	x = 1;
	v->z_buffer = (double *)malloc(sizeof(double) * v->s_width);
	while (x <= v->s_width)
	{
		ray_dir_x = v->dirX + v->planeX * (2 * x / (double)v->s_width - 1);
		ray_dir_y = v->dirY + v->planeY * (2 * x / (double)v->s_width - 1);
		main_cast(v, x, ray_dir_x, ray_dir_y);
		x++;
	}
	draw_sprites(v, 1.0 / (v->planeX * v->dirY - v->dirX * v->planeY));
	draw_fight(v, (int)(v->s_width / 2),
						(int)(v->s_width * 0.75), (int)(v->s_height / 2));
	mlx_put_image_to_window(v->mlx_ptr, v->win_ptr, v->img.ptr, 0, 0);
	free(v->z_buffer);
	return (0);
}

void	move_keys(t_view *v, int key, double f_time)
{
	double			tmp;

	f_time *= -1 + 2 * (key == 126 || key == 123);
	if (key == 125 || key == 126)
	{
		if (v->map.values[(int)(v->posX + v->dirX * f_time * 15)]
												[(int)(v->posY)] == 0)
			v->posX += v->dirX * f_time * 15;
		if (v->map.values[(int)(v->posX)]
						[(int)(v->posY + v->dirY * f_time * 15)] == 0)
			v->posY += v->dirY * f_time * 15;
	}
	if (key == 124 || key == 123)
	{
		tmp = v->dirX;
		v->dirX = v->dirX * cos(f_time * 5) - v->dirY * sin(f_time * 5);
		v->dirY = tmp * sin(f_time * 5) + v->dirY * cos(f_time * 5);
		tmp = v->planeX;
		v->planeX = v->planeX * cos(f_time * 5) - v->planeY * sin(f_time * 5);
		v->planeY = tmp * sin(f_time * 5) + v->planeY * cos(f_time * 5);
	}
}

void	init_sprites(t_view *v)
{
	int c;

	c = 0;
	srand((unsigned int)time(NULL));
	v->num_sprites = 19;
	v->sprites = (t_sprite *)malloc(sizeof(t_sprite) * v->num_sprites);
	while (c < v->num_sprites)
	{
		v->sprites[c].x = (float)rand() / (RAND_MAX) * v->map.width;
		v->sprites[c].y = (float)rand() / (RAND_MAX) * v->map.height;
		while (v->map.values[(int)v->sprites[c].x][(int)v->sprites[c].y] != 0)
		{
			v->sprites[c].x = (float)rand() / (RAND_MAX) * v->map.width;
			v->sprites[c].y = (float)rand() / (RAND_MAX) * v->map.height;
		}
		if (c < 8)
			v->sprites[c].texture = 10;
		else if (c < 11)
			v->sprites[c].texture = 9;
		else
			v->sprites[c].texture = 11;
		v->sprites[c].alive = 1;
		v->sprites[c].index = c;
		c++;
	}
}

void	init_view(t_view *v)
{
	v->posX = 22;
	v->posY = 12;
	v->dirX = -1;
	v->dirY = 0;
	v->planeX = 0;
	v->planeY = 0.66;
	v->texWidth = 64;
	v->texHeight = 64;
	v->fight_anim = 0;
	v->fight_tex = 21;
	v->attack = 0;
	v->health = 0;
}

int		main(int argc, char **argv)
{
	t_view view;

	if (argc != 2)
		ft_putendl("usage: ./wolf map_file_name");
	else
	{
		read_input(argv[1], &(view.map));
		init_sprites(&view);
		view.mlx_ptr = mlx_init();
		view.s_width = 1000;
		view.s_height = 500;
		view.win_ptr = mlx_new_window(view.mlx_ptr, view.s_width,
							view.s_height, "cfranco's game");
		view.img.ptr = mlx_new_image(view.mlx_ptr, view.s_width, view.s_height);
		view.img.addr = mlx_get_data_addr(view.img.ptr, &(view.img.bpp),
										&(view.img.s_line), &(view.img.endian));
		load_textures(&view);
		init_view(&view);
		paint_world(&view);
		mlx_hook(view.win_ptr, 2, 0, key_handler, &view);
		mlx_hook(view.win_ptr, 4, 0, mouse_handler, &view);
		mlx_hook(view.win_ptr, 17, 0, exit_handler, &view);
		mlx_loop_hook(view.mlx_ptr, loop_handler, &view);
		mlx_loop(view.mlx_ptr);
	}
	return (0);
}
