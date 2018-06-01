/* ************************************************************************** */
/*		                                                                */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/08 17:21:08 by cfranco           #+#    #+#             */
/*   Updated: 2018/05/08 17:21:11 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "wolf.h"
#include "mlx.h"
#include <libft.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int draw_line(int x, int d_start, int d_end, int color, t_view *view)
{
	int y = d_start;
	while (y < d_end)
	{
		mlx_pixel_put(view->mlx_ptr, view->win_ptr, x, y, color);
		y++;
	}
	return(0);
}

int paint_world(t_view *view)
{
	int x = 1;	

	while(x <= view->s_width)
		{
			//calculate ray position and direction
			double cameraX = 2 * x / (double)view->s_width - 1; //x-coordinate in camera space
			double rayDirX = view->dirX + view->planeX * cameraX;
			double rayDirY = view->dirY + view->planeY * cameraX;
			//which box of the map we're in
			int mapX = view->posX;
			int mapY = view->posY;

			//length of ray from current position to next x or y-side
			double sideDistX;
			double sideDistY;

			int color = 0;

			//length of ray from one x or y-side to next x or y-side
	      double deltaDistX = ABS(1 / rayDirX);
	      double deltaDistY = ABS(1 / rayDirY);
	      double perpWallDist;

	      //what direction to step in x or y-direction (either +1 or -1)
	      int stepX;
	      int stepY;

	      int hit = 0; //was there a wall hit?
	      int side; //was a NS or a EW wall hit?
	      //calculate step and initial sideDist
	      if (rayDirX < 0)
	      {
			stepX = -1;
			sideDistX = (view->posX - mapX) * deltaDistX;
	      }
	      else
	      {
			stepX = 1;
			sideDistX = (mapX + 1.0 - view->posX) * deltaDistX;
	      }
	      if (rayDirY < 0)
	      {
			stepY = -1;
			sideDistY = (view->posY - mapY) * deltaDistY;
	      }
	      else
	      {
			stepY = 1;
			sideDistY = (mapY + 1.0 - view->posY) * deltaDistY;
	      }
	      //perform DDA
	      while (hit == 0)
	      {
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY)
			{
			sideDistX += deltaDistX;
			mapX += stepX;
			side = 0;
	        }
	        else
	        {
			sideDistY += deltaDistY;
			mapY += stepY;
			side = 1;
	        }
	        //Check if ray has hit a wall
	        if (worldMap[mapX][mapY] > 0) hit = 1;
	      }
	      //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
	      if (side == 0) perpWallDist = (mapX - view->posX + (1 - stepX) / 2) / rayDirX;
	      else		perpWallDist = (mapY - view->posY + (1 - stepY) / 2) / rayDirY;

	      //Calculate height of line to draw on screen
	      int lineHeight = (int)(view->s_width / perpWallDist);

	      //calculate lowest and highest pixel to fill in current stripe
	      int drawStart = -lineHeight / 2 + view->s_width / 2;
	      if(drawStart < 0)drawStart = 0;
	      int drawEnd = lineHeight / 2 + view->s_width / 2;
	      if(drawEnd >= view->s_width)drawEnd = view->s_width - 1;

	      //choose wall color
	      switch(worldMap[mapX][mapY])
	      {
	        case 1:  color = 0xFF0000;  break; //red
	        case 2:  color = 0x00FF00;  break; //green
	        case 3:  color = 0x0000FF;   break; //blue
	        case 4:  color = 0xFFFFFF;  break; //white
	        default: color = 0x00FFFF; break; //yellow
	      }

	      //give x and y sides different brightness
	      if (side == 1) {color = color / 2;}

	      //draw the pixels of the stripe as a vertical line
	      draw_line(x, drawStart, drawEnd, color, view);
	      x++;
	    }
	return(0);
}

void	move_keys(t_view *view, int key)
{
	double moveSpeed = 2.0; //the constant value is in squares/second
    double rotSpeed = 0.5; //the constant value is in radians/second
    mlx_clear_window (view->mlx_ptr, view->win_ptr);
	if (key == 125)
	{
		if(worldMap[(int)(view->posX - view->dirX * moveSpeed)][(int)(view->posY)] == 0) view->posX -= view->dirX * moveSpeed;
        if(worldMap[(int)(view->posX)][(int)(view->posY - view->dirY * moveSpeed)] == 0) view->posY -= view->dirY * moveSpeed;
	}
	if (key == 126)
	{
		if(worldMap[(int)(view->posX + view->dirX * moveSpeed)][(int)(view->posY)] == 0) view->posX += view->dirX * moveSpeed;
      	if(worldMap[(int)(view->posX)][(int)(view->posY + view->dirY * moveSpeed)] == 0) view->posY += view->dirY * moveSpeed;
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
	paint_world(view);	
}

int		key_handler(int key, t_view *view)
{
	if (key == 53)
	{
		mlx_destroy_window(view->mlx_ptr, view->win_ptr);
		exit(0);
	}
	else if (key == 123 || key == 124 || key == 125 || key == 126)
		move_keys(view, key);
	else if (key == 69)
		view->max += 10;
	else if (key == 78)
		view->max -= 10;
	else if (key == 67)
	{
		if ((view->sat += 10) > 255)
			view->sat = 0;
	}
	else if (key == 75)
	{
		if ((view->bri += 10) > 255)
			view->bri = 10;
	}
	/*if ((key >= 123 && key <= 126) || key == 69 || key == 78 ||
			key == 67 || key == 75)
		threading(view);*/
	return (1);
}

int			main(/*int argc, char **argv*/void)
{

	t_view view;

	view.mlx_ptr = mlx_init();
	view.s_width = 1000;
	view.s_height = 1000;

	view.world_map = (int **)worldMap;


	//x and y start position
  	//initial direction vector
    //the 2d raycaster version of camera plane
	view.posX = 22;
	view.posY = 12;
	view.dirX = -1;
	view.dirY = 0;
	view.planeX = 0;
	view.planeY = 0.66;

	
	view.win_ptr = mlx_new_window(view.mlx_ptr, view.s_width, view.s_height, "wolf 3D");
	//view.img_ptr = mlx_new_image(view.mlx_ptr, view.s_width, view.s_height);
	/*view.img.addr = mlx_get_data_addr(view.img_ptr, &(view.img.bpp),
								&(view.img.s_line), &(view.img.endian));*/

	/*if ((argc < 2 || argc > 4) || !get_type(argv[1]) || (argc == 3 &&
			!get_type(argv[2])) || (argc == 4 && !get_type(argv[3])))
		ft_putendl("usage: fractal1 [fractal2] [fractal3]\nfractals: \
						\n- julia\n- mandelbrot\n- burning_ship\n- mandelbrat \
							\n- julia3\n- mandelbrot3\n- mandelbar3");
	else
	{*/
		paint_world(&view);		
		mlx_hook(view.win_ptr, 2, 0, key_handler, &view);
		/*mlx_hook(view.win_ptr, 4, 0, mouse_handler, &view);
		mlx_hook(view.win_ptr, 6, 0, motion_handler, &view);*/
		mlx_loop(view.mlx_ptr);
	//}
	return (0);
}
