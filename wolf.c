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

int		fill_image(int index, int x, int y, t_view *v)
{
	unsigned int color;

	color = *(unsigned int *)(v->textures[index].addr +
					v->textures[index].s_line * v->tex_y +
						(v->tex_x * (v->textures[index].bpp / 8)));
	if (v->alpha == 2 && ((color & 0xFFFFFF) == 0))
		return (0);
	else if (v->alpha == 1)
		color = (color >> 1) & 8355711;
	color = color | (v->damage << 16);
	*(int *)(v->img.addr + (x * (v->img.bpp / 8))
									+ (y * v->img.s_line)) = color;
	v->alpha = 0;
	return (0);
}

int		paint_world(t_view *v)
{
	int		x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	buffer[v->s_width];

	x = 1;
	v->z_buffer = buffer;
	while (x <= v->s_width)
	{
		ray_dir_x = v->dir_x + v->plane_x * (2 * x / (double)v->s_width - 1);
		ray_dir_y = v->dir_y + v->plane_y * (2 * x / (double)v->s_width - 1);
		main_cast(v, x, ray_dir_x, ray_dir_y);
		x++;
	}
	draw_sprites(v, 1.0 / (v->plane_x * v->dir_y - v->dir_x * v->plane_y));
	draw_fight(v, (int)(v->s_width / 2),
						(int)(v->s_width * 0.75), (int)(v->s_height / 2));
	mlx_put_image_to_window(v->mlx_ptr, v->win_ptr, v->img.ptr, 0, 0);
	v->z_buffer = NULL;
	return (0);
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
		v->sprites[c].x = (float)rand() / (RAND_MAX) * v->map.w - 1;
		v->sprites[c].y = (float)rand() / (RAND_MAX) * v->map.h - 1;
		while (v->map.values[(int)v->sprites[c].x][(int)v->sprites[c].y] != 0)
		{
			v->sprites[c].x = (float)rand() / (RAND_MAX) * v->map.w - 1;
			v->sprites[c].y = (float)rand() / (RAND_MAX) * v->map.h - 1;
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

void	init_view(t_view *v, int sound)
{
	if ((v->s_width < 200 || v->s_height < 200) ||
							(v->s_width / v->s_height) > 2)
		print_error(2);
	srand((unsigned int)time(NULL));
	v->pos_x = (float)rand() / (RAND_MAX) * v->map.w - 1;
	v->pos_y = (float)rand() / (RAND_MAX) * v->map.h - 1;
	while (v->map.values[(int)v->pos_x][(int)v->pos_y] != 0)
	{
		v->pos_x = (float)rand() / (RAND_MAX) * v->map.w - 1;
		v->pos_y = (float)rand() / (RAND_MAX) * v->map.h - 1;
	}
	v->dir_x = -1;
	v->dir_y = 0;
	v->plane_x = 0;
	v->plane_y = 0.66;
	v->t_width = 64;
	v->t_height = 64;
	v->fight_anim = 0;
	v->fight_tex = 21;
	v->attack = 0;
	v->damage = 0;
	v->alpha = 0;
	if (sound)
		system("afplay -v 5 sounds/laugh.mp3 &");
	system("afplay -v 2 sounds/background.mp3 &");
}

int		main(int argc, char **argv)
{
	t_view v;

	v.s_width = (argc != 4) ? 1000 : ft_atoi(argv[2]);
	v.s_height = (argc != 4) ? 1000 : ft_atoi(argv[3]);
	if (argc != 2 && argc != 4)
		print_error(2);
	else
	{
		read_input(argv[1], &(v.map));
		init_sprites(&v);
		init_view(&v, 0);
		v.mlx_ptr = mlx_init();
		v.win_ptr = mlx_new_window(v.mlx_ptr, v.s_width,
							v.s_height, "Wolf 3d - cfranco");
		v.img.ptr = mlx_new_image(v.mlx_ptr, v.s_width, v.s_height);
		v.img.addr = mlx_get_data_addr(v.img.ptr, &(v.img.bpp),
										&(v.img.s_line), &(v.img.endian));
		load_textures(&v);
		mlx_hook(v.win_ptr, 2, 0, key_handler, &v);
		mlx_hook(v.win_ptr, 4, 0, mouse_handler, &v);
		mlx_hook(v.win_ptr, 17, 0, exit_handler, &v);
		mlx_loop_hook(v.mlx_ptr, loop_handler, &v);
		mlx_loop(v.mlx_ptr);
	}
	return (0);
}
