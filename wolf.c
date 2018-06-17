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

int fill_image(int index, int x, int y, int dark, t_view *v)
{
	unsigned int color;

	color = *(unsigned int *)(v->textures[index].addr +
        			v->textures[index].s_line * v->tex_y +
        				(v->tex_x * (v->textures[index].bpp / 8)));
	if (dark == 2 && ((color & 0xFFFFFF) == 0))
		return (0);
	else if (dark == 1)
		color = (color >> 1) & 8355711;
	*(int *)(v->img.addr + (x * (v->img.bpp / 8)) + (y * v->img.s_line)) = color;
	return (0);
}

int		dda(int x, double rayDirX, double rayDirY, t_view *view)
{
	double sideDistX;
	double sideDistY;
    int side;

    sideDistX = ((-1 + 2 * (rayDirX < 0))*(view->posX - view->mapX) + !(rayDirX < 0)) * ABS(1 / rayDirX);
    sideDistY = ((-1 + 2 * (rayDirY < 0))*(view->posY - view->mapY) + !(rayDirY < 0)) * ABS(1 / rayDirY);
    while (!view->map.values[view->mapX][view->mapY])
    {
		if (sideDistX < sideDistY)
		{
			sideDistX += ABS(1 / rayDirX);
			view->mapX += -1 + 2 * !(rayDirX < 0);
			side = !(rayDirX > 0);
      	}
      	else
      	{
			sideDistY += ABS(1 / rayDirY);
			view->mapY += (-1 + 2 * !(rayDirY < 0));
			side = !(rayDirY > 0) + 2;
      	}
    }
    if (side == 0 || side == 1) 
    	view->z_buffer[x] = (view->mapX - view->posX + (1 - (-1 + 2 * !(rayDirX < 0))) / 2) / rayDirX;
    else
    	view->z_buffer[x] = (view->mapY - view->posY + (1 - (-1 + 2 * !(rayDirY < 0))) / 2) / rayDirY;
    return (side);
}

void	main_cast(t_view *v, int x, double	rayDirX, double	rayDirY)
{
	double	wallX;	
	double	floorXWall;
	double	floorYWall;
	int		side;

	v->mapX = (int)v->posX;
	v->mapY = (int)v->posY;
	side = dda(x, rayDirX, rayDirY, v);
    if (side == 0 || side == 1)
    	wallX = v->posY + v->z_buffer[x] * rayDirY;
    else
    	wallX = v->posX + v->z_buffer[x] * rayDirX;
    wallX -= floor((wallX));
    v->tex_x = (int)(wallX * (double)(v->texWidth));
    if((side == 0 && rayDirX > 0) || (side == 3 && rayDirY < 0))
    	v->tex_x = v->texWidth - v->tex_x - 1;    
    floorXWall = v->mapX + wallX * !(side == 0 || side == 1)
    						+ (rayDirX < 0) * (side == 0 || side == 1);
    floorYWall = v->mapY + (rayDirY < 0) * !(side == 0 || side == 1)
    						+ wallX * (side == 0 || side == 1);
    draw_line(x, side, v);
    draw_floor(x, floorXWall, floorYWall, v);	       	  
}

int paint_world(t_view *v)
{
	int x;
	double	rayDirX;
	double	rayDirY;	

	x = 1;
	while(x <= v->s_width)
	{
		rayDirX = v->dirX + v->planeX * (2 * x / (double)v->s_width - 1);
		rayDirY = v->dirY + v->planeY * (2 * x / (double)v->s_width - 1);
		main_cast(v, x, rayDirX, rayDirY);
	    x++;
    }
	draw_sprites(v, 1.0 / (v->planeX * v->dirY - v->dirX * v->planeY));
	draw_fight(v, (int)(v->s_width / 2), (int)(v->s_width * 0.75), (int)(v->s_height / 2));
	mlx_put_image_to_window(v->mlx_ptr, v->win_ptr, v->img.ptr, 0, 0);
	return(0);
}

void	move_keys(t_view *v, int key)
{	
	static double	f_time = 0;
	clock_t			t;
	double			tmp;
	int				sign;

	//mlx_clear_window(v->mlx_ptr, v->win_ptr);
	if (key == 125 || key == 126)
	{
		sign = -1 + 2 * (key == 126);
		if(v->map.values[(int)(v->posX + (sign) * v->dirX * f_time * 15)][(int)(v->posY)] == 0)
			v->posX += (sign) * v->dirX * f_time * 15;
	    if(v->map.values[(int)(v->posX)][(int)(v->posY + (sign) * v->dirY * f_time * 15)] == 0)
	    	v->posY += (sign) * v->dirY * f_time * 15;
	}
	if (key == 124 || key == 123)
	{
		sign = -1 + 2 * (key == 123);
		tmp = v->dirX;
		v->dirX = v->dirX * cos(sign * f_time * 5) - v->dirY * sin(sign * f_time * 5);
		v->dirY = tmp * sin(sign * f_time * 5) + v->dirY * cos(sign * f_time * 5);
		tmp = v->planeX;
		v->planeX = v->planeX * cos(sign * f_time * 5) - v->planeY * sin(sign * f_time * 5);
		v->planeY = tmp * sin(sign * f_time * 5) + v->planeY * cos(sign * f_time * 5);
	}
	t = clock();
	paint_world(v);
	f_time = ((double)(clock() - t)) / CLOCKS_PER_SEC;		
}

int			main(int argc, char **argv)
{
	t_view view;

	if (argc != 2)
	{
		ft_putendl("usage: ./wolf map_file_name");
		exit(0);
	}

	view.num_sprites = 19;

	t_sprite tmp_sprites[] =
	{
	  {20.5, 11.5, 10, 0, 0},
	  //green light in front of playerstart
	  //green lights in every room
	  {18.5 ,4.5, 10, 0, 1},
	  {10.0,4.5, 10, 0, 2},
	  {10.0,12.5,10, 0, 3},
	  {3.5, 6.5, 10, 0, 4},
	  {3.5, 20.5,10, 0, 5},
	  {3.5, 14.5,10, 0, 6},
	  {14.5,20.5,10, 0, 7},

	  //row of pillars in front of wall: fisheye test
	  {18.5, 10.5, 9, 0, 8},
	  {18.5, 11.5, 9, 0, 9},
	  {18.5, 12.5, 9, 0, 10},

	  //some barrels around the map
	  {21.5, 2.5, 11, 0, 11},
	  {15.5, 2.5, 11, 0, 12},
	  {16.0, 2.8, 11, 0, 13},
	  {16.2, 1.2, 11, 0, 14},
	  {3.5,  2.5, 11, 0, 15},
	  {9.5, 16.5, 11, 0, 16},
	  {10.0, 15.1,11, 0, 17},
	  {10.5, 15.8,11, 0, 18},
	};

	view.sprites = (t_sprite *)malloc(sizeof(t_sprite) * view.num_sprites);
	view.sprites = tmp_sprites;

	view.mlx_ptr = mlx_init();
	view.s_width = 1000;
	view.s_height = 500;

	read_input(argv[1], &(view.map));

	//view.world_map = (int **)view->map.values;
	init_images(&view);	

	//x and y start position
  	//initial direction vector
    //the 2d raycaster version of camera plane
	view.posX = 22;
	view.posY = 12;
	view.dirX = -1;
	view.dirY = 0;
	view.planeX = 0;
	view.planeY = 0.66;
	view.texWidth = 64;
	view.texHeight = 64;
	view.fight_anim = 0;
	view.fight_tex = 15;
	view.z_buffer = (double *)malloc(sizeof(double) * view.s_width);

	view.win_ptr = mlx_new_window(view.mlx_ptr, view.s_width, view.s_height, "wolf 3D");
	view.img.ptr = mlx_new_image(view.mlx_ptr, view.s_width, view.s_height);
	view.img.addr = mlx_get_data_addr(view.img.ptr, &(view.img.bpp),
									&(view.img.s_line), &(view.img.endian));
	paint_world(&view);
	mlx_hook(view.win_ptr, 2, 0, key_handler, &view);
	mlx_hook(view.win_ptr, 4, 0, mouse_handler, &view);
	mlx_hook(view.win_ptr, 17, 0, exit_handler, &view);
	mlx_loop_hook(view.mlx_ptr, loop_handler, &view);
	mlx_loop(view.mlx_ptr);
	argc = 0;
	return (0);
}
