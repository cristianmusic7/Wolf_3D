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

int fill_image(int index, int x, int y, int texX, int texY, int dark, t_view *view)
{
	unsigned int color;

	color = *(unsigned int *)(view->textures[index].addr +
        			view->textures[index].s_line * texY + (texX * (view->textures[index].bpp / 8)));
	if (dark == 2 && ((color & 0xFFFFFF) == 0))
		return (0);
	else if (dark == 1)
		color = (color >> 1) & 8355711;
	*(int *)(view->img.addr + (x * (view->img.bpp / 8)) + (y * view->img.s_line)) = color;
	return (0);
}

int dda(int x, double rayDirX, double rayDirY, t_view *view)
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

int paint_world(t_view *view)
{
	int x;
	int side;
	double wallX;
	double rayDirX;
	double rayDirY;
	int texX;

	x = 1;
	while(x <= view->s_width)
	{
		rayDirX = view->dirX + view->planeX * (2 * x / (double)view->s_width - 1);
		rayDirY = view->dirY + view->planeY * (2 * x / (double)view->s_width - 1);
		view->mapX = (int)view->posX;
		view->mapY = (int)view->posY;
		side = dda(x, rayDirX, rayDirY, view);
	    if (side == 0 || side == 1)
	    	wallX = view->posY + view->z_buffer[x] * rayDirY;
	    else
	    	wallX = view->posX + view->z_buffer[x] * rayDirX;
	    wallX -= floor((wallX));
	    texX = (int)(wallX * (double)(view->texWidth));
	    if((side == 0 && rayDirX > 0) || (side == 3 && rayDirY < 0))
	    	texX = view->texWidth - texX - 1;
	    draw_line(x, texX, side, view);
	    double floorXWall, floorYWall;
	    floorXWall = view->mapX + wallX*!(side == 0 || side == 1) + (rayDirX < 0)*(side == 0 || side == 1);
	    floorYWall = view->mapY + (rayDirY < 0)*!(side == 0 || side == 1) + wallX*(side == 0 || side == 1);
	    draw_floor(x, floorXWall, floorYWall, view);	       	  
	    x++;
    }
	draw_sprites(view);
	draw_fight(view);
	mlx_put_image_to_window(view->mlx_ptr, view->win_ptr, view->img.ptr, 0, 0);
	return(0);
}

void	move_keys(t_view *view, int key)
{	
    static double	f_time = 0;
    double			moveSpeed = f_time * 15.0; //the constant value is in squares/second
    double			rotSpeed = f_time * 5.0; //the constant value is in radians/second
	clock_t			t;
	
    mlx_clear_window(view->mlx_ptr, view->win_ptr);
	if (key == 125)
	{
		if(view->map.values[(int)(view->posX - view->dirX * moveSpeed)][(int)(view->posY)] == 0)
			view->posX -= view->dirX * moveSpeed;
        if(view->map.values[(int)(view->posX)][(int)(view->posY - view->dirY * moveSpeed)] == 0)
        	view->posY -= view->dirY * moveSpeed;
	}
	if (key == 126)
	{
		if(view->map.values[(int)(view->posX + view->dirX * moveSpeed)][(int)(view->posY)] == 0)
			view->posX += view->dirX * moveSpeed;
      	if(view->map.values[(int)(view->posX)][(int)(view->posY + view->dirY * moveSpeed)] == 0)
      		view->posY += view->dirY * moveSpeed;
    }		
	if (key == 124)
	{		
      //both camera direction and camera plane must be rotated
      double oldDirX = view->dirX;
      view->dirX = view->dirX * cos(-rotSpeed) - view->dirY * sin(-rotSpeed);
      view->dirY = oldDirX * sin(-rotSpeed) + view->dirY * cos(-rotSpeed);
      double oldPlaneX = view->planeX;
      view->planeX = view->planeX * cos(-rotSpeed) - view->planeY * sin(-rotSpeed);
      view->planeY = oldPlaneX * sin(-rotSpeed) + view->planeY * cos(-rotSpeed);
	}
	if (key == 123)
	{
		//both camera direction and camera plane must be rotated
      double oldDirX = view->dirX;
      view->dirX = view->dirX * cos(rotSpeed) - view->dirY * sin(rotSpeed);
      view->dirY = oldDirX * sin(rotSpeed) + view->dirY * cos(rotSpeed);
      double oldPlaneX = view->planeX;
      view->planeX = view->planeX * cos(rotSpeed) - view->planeY * sin(rotSpeed);
      view->planeY = oldPlaneX * sin(rotSpeed) + view->planeY * cos(rotSpeed);
	}

	t = clock();
	paint_world(view);
	t = clock() - t;
	f_time = ((double)t) / CLOCKS_PER_SEC;		
}

void init_images(t_view *view)
{
	static char* names[] = {"textures/bluestone.xpm", "textures/colorstone.xpm", "textures/purplestone.xpm", "textures/eagle.xpm",
	"textures/mossy.xpm", "textures/redbrick.xpm", "textures/wood.xpm", "textures/greystone.xpm", "textures/barrel.xpm",
	"textures/pillar.xpm", "textures/greenlight.xpm", "textures/devil/devil_0_0.xpm", "textures/devil/devil_0_1.xpm",
	"textures/devil/devil_0_2.xpm", "textures/devil/devil_0_3.xpm", "textures/fight/fight_0_3.xpm", "textures/fight/fight_0_0.xpm",
	"textures/fight/fight_0_1.xpm", "textures/fight/fight_0_2.xpm", "textures/fight/fight_1_0.xpm", "textures/fight/fight_1_1.xpm",
	"textures/fight/fight_1_2.xpm"};

	//check seg fault when I punch really fast
	int c = 0;
	int c2 = 0;

	while (names[c] != '\0')
		c++;
	while (c2 < c -1 )
	{
		if (c2 > 14)
		{
			view->texWidth = 250;
			view->texHeight = 150;
		}
		view->textures[c2].ptr = mlx_xpm_file_to_image(view->mlx_ptr, names[c2], &view->texWidth, &view->texHeight);
		view->textures[c2].addr = mlx_get_data_addr(view->textures[c2].ptr, &(view->textures[c2].bpp),
					&(view->textures[c2].s_line), &(view->textures[c2].endian));
		c2++;
	}
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
