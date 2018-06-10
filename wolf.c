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
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

int worldMap[mapWidth][mapHeight]=
{
  {8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
  {8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
  {8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
  {8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
  {8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
  {8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
  {8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
  {7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
  {7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
  {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
  {7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
  {7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
  {7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
  {2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
  {2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
  {2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
  {1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
  {2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
  {2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
  {2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
  {2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
  {2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
  {2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
};

#define numSprites 19

t_sprite sprite[numSprites] =
{
  {20.5, 11.5, 10, 0}, //green light in front of playerstart
  //green lights in every room
  {18.5,4.5, 10, 0},
  {10.0,4.5, 10, 0},
  {10.0,12.5,10, 0},
  {3.5, 6.5, 10, 0},
  {3.5, 20.5,10, 0},
  {3.5, 14.5,10, 0},
  {14.5,20.5,10, 0},

  //row of pillars in front of wall: fisheye test
  {18.5, 10.5, 9, 0},
  {18.5, 11.5, 9, 0},
  {18.5, 12.5, 9, 0},

  //some barrels around the map
  {21.5, 1.5, 11, 10.0},
  {15.5, 1.5, 11, 20.0},
  {16.0, 1.8, 11, 30.0},
  {16.2, 1.2, 11, 40.0},
  {3.5,  2.5, 11, 50.0},
  {9.5, 16.5, 11, 60.0},
  {10.0, 15.1,11, 70.0},
  {10.5, 15.8,11, 80.0},
};

int spriteOrder[numSprites];
double spriteDistance[numSprites];

//sort algorithm
void combSort(int* order, double* dist, int amount)
{
  int gap = amount;
  int swapped = 0;
  double tmp_dist;
  double tmp_order;
  while(gap > 1 || swapped)
  {
    //shrink factor 1.3
    gap = (gap * 10) / 13;
    if(gap == 9 || gap == 10) gap = 11;
    if (gap < 1) gap = 1;
    swapped = 0;
    for(int i = 0; i < amount - gap; i++)
    {
      int j = i + gap;
      if(dist[i] < dist[j])
      {
      	tmp_dist = dist[i];
      	dist[i] = dist[j];
      	dist[j] = tmp_dist;

      	tmp_order = order[i];
      	order[i] = order[j];
      	order[j] = tmp_order;
        swapped = 1;
      }
    }
  }
}


int draw_line(int x, int d_start, int d_end, int lineHeight, int texX, int side, int texNum, t_view *view)
{
	/*int y = d_start;
	while (y < d_end)
	{
		mlx_pixel_put(view->mlx_ptr, view->win_ptr, x, y, color);
		y++;
	}
	return(0);*/
	texNum = 0;
	int y = d_start;
	while (y < d_end)
	{
		int d = y * 256 - view->s_height * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
        // TODO: avoid the division to speed this up
        int texY = ((d * view->texHeight) / lineHeight) / 256;
        unsigned int color = *(unsigned int *)(view->textures[side].addr +
        			view->textures[side].s_line * texY + (texX * (view->textures[side].bpp / 8)));  
   
        //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        if(side == 2 || side == 3) color = (color >> 1) & 8355711;

        *(int *)(view->img.addr + (x * (view->img.bpp / 8)) + (y * view->img.s_line)) = color;
        //mlx_pixel_put(view->mlx_ptr, view->win_ptr, x, y, color);
		y++;
	}
	return(0);
}

int draw_floor(int x, int drawEnd, double floorXWall, double floorYWall, double perpWallDist, t_view *view)
{
	double distWall, distPlayer, currentDist;

	  distWall = perpWallDist;
	  distPlayer = 0.0;

	  if (drawEnd < 0) drawEnd = view->s_height; //becomes < 0 when the integer overflows

	  //draw the floor from drawEnd to the bottom of the screen

	  int y = drawEnd + 1;
	  while (y < view->s_height)
	  {
	    currentDist = view->s_height / (2.0 * y - view->s_height); //you could make a small lookup table for this instead

	    double weight = (currentDist - distPlayer) / (distWall - distPlayer);

	    double currentFloorX = weight * floorXWall + (1.0 - weight) * view->posX;
	    double currentFloorY = weight * floorYWall + (1.0 - weight) * view->posY;

	    int floorTexX, floorTexY;
	    floorTexX = (int)(currentFloorX * view->texWidth) % view->texWidth;
	    floorTexY = (int)(currentFloorY * view->texHeight) % view->texHeight;

	    unsigned int color_floor = (*(unsigned int *)(view->textures[3].addr +
				view->textures[3].s_line * floorTexY + (floorTexX * (view->textures[3].bpp / 8))) >> 1) & 8355711;

	    *(int *)(view->img.addr + (x * (view->img.bpp / 8)) + (y * view->img.s_line)) = color_floor;

	    //mlx_pixel_put(view->mlx_ptr, view->win_ptr, x, y, color_floor);

	    unsigned int color_ceiling = *(unsigned int *)(view->textures[6].addr +
				view->textures[6].s_line * floorTexY + (floorTexX * (view->textures[6].bpp / 8)));

	    *(int *)(view->img.addr + (x * (view->img.bpp / 8)) + ((view->s_height - y) * view->img.s_line)) = color_ceiling;

	    y++;
	    //mlx_pixel_put(view->mlx_ptr, view->win_ptr, x, view->s_height - y, color_ceiling);
	  }
	return(0);
}

int draw_sprites(t_view *view)
{
	//SPRITE CASTING
	    //sort sprites from far to close
	    for(int i = 0; i < numSprites; i++)
	    {
	      spriteOrder[i] = i;
	      spriteDistance[i] = ((view->posX - sprite[i].x) * (view->posX - sprite[i].x) + (view->posY - sprite[i].y) * (view->posY - sprite[i].y)); //sqrt not taken, unneeded
	    }
	    combSort(spriteOrder, spriteDistance, numSprites);

	    //after sorting the sprites, do the projection and draw them
	    for(int i = 0; i < numSprites; i++)
	    {
	      //translate sprite position to relative to camera
	      double spriteX = sprite[spriteOrder[i]].x - view->posX;
	      double spriteY = sprite[spriteOrder[i]].y - view->posY;

	      //transform sprite with the inverse camera matrix
	      // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
	      // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
	      // [ planeY   dirY ]                                          [ -planeY  planeX ]

	      
	      double invDet = 1.0 / (view->planeX * view->dirY - view->dirX * view->planeY); //required for correct matrix multiplication

	      double transformX = invDet * (view->dirY * spriteX - view->dirX * spriteY);
	      double transformY = invDet * (-view->planeY * spriteX + view->planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D

	      

	      if(transformY > 0)
	      {
	      	  int spriteScreenX = (int)((view->s_width / 2) * (1 + transformX / transformY));

	      	  /*int uDiv = 2;
      		  int vDiv = 2;*/
      		  //int vMove = 640.0;
      		  //int vMoveScreen = (int)(vMove / transformY);
      		  //int vMoveScreen = (int)(64.0 / transformY);

		      //calculate height of the sprite on screen
		      int spriteHeight = ABS((int)(view->s_height / (transformY))); //using "transformY" instead of the real distance prevents fisheye
		      //calculate lowest and highest pixel to fill in current stripe
		      int drawStartY = -spriteHeight / 2 + view->s_height / 2;
		      if(drawStartY < 0) drawStartY = 0;
		      int drawEndY = spriteHeight / 2 + view->s_height / 2;
		      if(drawEndY >= view->s_height) drawEndY = view->s_height - 1;

		      //calculate width of the sprite
		      int spriteWidth = ABS( (int)(view->s_height / (transformY)));
		      int drawStartX = -spriteWidth / 2 + spriteScreenX;
		      if(drawStartX < 0) drawStartX = 0;
		      int drawEndX = spriteWidth / 2 + spriteScreenX;
		      if(drawEndX >= view->s_width) drawEndX = view->s_width - 1;

		      //loop through every vertical stripe of the sprite on screen
		      for(int stripe = drawStartX; stripe < drawEndX; stripe++)
		      {
			        int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * view->texWidth / spriteWidth) / 256;
			        //the conditions in the if are:
			        //1) it's in front of camera plane so you don't see things behind you
			        //2) it's on the screen (left)
			        //3) it's on the screen (right)
			        //4) ZBuffer, with perpendicular distance
			        if(stripe > 0 && stripe < view->s_width && transformY < view->z_buffer[stripe])
			        {
			        	for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
				        {
				          int d = (y) * 256 - view->s_height * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
				          int texY = ((d * view->texHeight) / spriteHeight) / 256;

				          unsigned int color = *(unsigned int *)(view->textures[sprite[spriteOrder[i]].texture].addr +
							view->textures[sprite[spriteOrder[i]].texture].s_line * texY + (texX * (view->textures[sprite[spriteOrder[i]].texture].bpp / 8)));

				          if((color & 0xFFFFFF) != 0)
				    	  		*(int *)(view->img.addr + (stripe * (view->img.bpp / 8)) +
				    	  				(y * view->img.s_line)) = color;


				          //unsigned long color = view->textures[sprite[spriteOrder[i]].texture][view->texWidth * texY + texX]; //get current color from the texture
				          //if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = color; //paint pixel if it isn't black, black is the invisible color
				       }
				   }		        
		      	}
	      }
	      
	  	}
	return(0);
}

int draw_fight(t_view *view)
{
	 int spriteScreenX = (int)(view->s_width / 2);

  	  /*int uDiv = 2;
		  int vDiv = 2;*/
		  //int vMove = 640.0;
		  //int vMoveScreen = (int)(vMove / transformY);
		  //int vMoveScreen = (int)(64.0 / transformY);

      //calculate height of the sprite on screen
      int spriteHeight = 450; //using "transformY" instead of the real distance prevents fisheye
      //calculate lowest and highest pixel to fill in current stripe
      int drawStartY = view->s_height - spriteHeight;
      if(drawStartY < 0) drawStartY = 0;
      int drawEndY = view->s_height;
      if(drawEndY >= view->s_height) drawEndY = view->s_height - 1;

      //calculate width of the sprite
      int spriteWidth = 750;
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if(drawStartX < 0) drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if(drawEndX >= view->s_width) drawEndX = view->s_width - 1;
	  for(int stripe = drawStartX; stripe < drawEndX; stripe++)
      	{
	        int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * 250 / spriteWidth) / 256;
	        //the conditions in the if are:
	        //1) it's in front of camera plane so you don't see things behind you
	        //2) it's on the screen (left)
	        //3) it's on the screen (right)
	        //4) ZBuffer, with perpendicular distance
        	for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
	        {

	        	int texY = (int)(256 * (y - (view->s_height - spriteHeight)) * 150 / spriteHeight) / 256;
	        	/*int d = (y) * 256 - view->s_height * (256); //256 and 128 factors to avoid floats
				int texY = ((d * 150) / spriteHeight) / 256;*/

	          unsigned int color = *(unsigned int *)(view->textures[view->fight_tex].addr +
				view->textures[view->fight_tex].s_line * texY + (texX * (view->textures[view->fight_tex].bpp / 8)));

	          if((color & 0xFFFFFF) != 0)
	    	  		*(int *)(view->img.addr + (stripe * (view->img.bpp / 8)) +
	    	  				(y * view->img.s_line)) = color;
	    	  texY++;

	          //unsigned long color = view->textures[sprite[spriteOrder[i]].texture][view->texWidth * texY + texX]; //get current color from the texture
	          //if((color & 0x00FFFFFF) != 0) buffer[y][stripe] = color; //paint pixel if it isn't black, black is the invisible color
	       }
	       texX++;
      	}
	return(0);
}

int paint_world(t_view *view)
{
	int x = 1;
	view->z_buffer = (double *)malloc(sizeof(double) * view->s_width);

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

			//int color = 0;

			//length of ray from one x or y-side to next x or y-side
	      double deltaDistX = ABS(1 / rayDirX);
	      double deltaDistY = ABS(1 / rayDirY);
	      double perpWallDist;

	      //what direction to step in x or y-direction (either +1 or -1)
	      int stepX;
	      int stepY;

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
	      while (!worldMap[mapX][mapY])
	      {
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				if (rayDirX > 0)
					side = 0;
				else
					side = 1;
	        }
	        else
	        {
				sideDistY += deltaDistY;
				mapY += stepY;
				if (rayDirY > 0)
					side = 2;
				else
					side = 3;
	        }
	      }
	      //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
	      if (side == 0 || side == 1) perpWallDist = (mapX - view->posX + (1 - stepX) / 2) / rayDirX;
	      else		perpWallDist = (mapY - view->posY + (1 - stepY) / 2) / rayDirY;

	      //Calculate height of line to draw on screen
	      int lineHeight = (int)(view->s_height / perpWallDist);

	      //calculate lowest and highest pixel to fill in current stripe
	      int drawStart = -lineHeight / 2 + view->s_width / 2;
	      if(drawStart < 0)drawStart = 0;
	      int drawEnd = lineHeight / 2 + view->s_width / 2;
	      if(drawEnd >= view->s_width)drawEnd = view->s_width - 1;

	      int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

	      //calculate value of wallX
	      double wallX; //where exactly the wall was hit
	      if (side == 0 || side == 1) wallX = view->posY + perpWallDist * rayDirY;
	      else           wallX = view->posX + perpWallDist * rayDirX;
	      wallX -= floor((wallX));

	      //x coordinate on the texture
	      int texX = (int)(wallX * (double)(view->texWidth));
	      if(side == 0 && rayDirX > 0) texX = view->texWidth - texX - 1;
	      if(side == 3 && rayDirY < 0) texX = view->texWidth - texX - 1;

	      //draw the pixels of the stripe as a vertical line
	      draw_line(x, drawStart, drawEnd, lineHeight, texX, side, texNum, view);

	      //FLOOR CASTING
	      double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

	      //4 different wall directions possible
	      if(side == 0 || side == 1)
	      {
	        floorXWall = mapX + (rayDirX < 0);
	        floorYWall = mapY + wallX;
	      }
	      else
	      {
	        floorXWall = mapX + wallX;
	        floorYWall = mapY + (rayDirY < 0);
	      }
	      view->z_buffer[x] = perpWallDist; //perpendicular distance is used 

	      draw_floor(x, drawEnd, floorXWall, floorYWall, perpWallDist, view);	       	  
	      x++;
	    }	 

	draw_sprites(view);
	draw_fight(view);
	free(view->z_buffer);
	mlx_put_image_to_window(view->mlx_ptr, view->win_ptr, view->img.ptr, 0, 0);

	return(0);
}

void	move_keys(t_view *view, int key)
{	
    static double	f_time = 1;
    double moveSpeed = f_time * 15.0; //the constant value is in squares/second
    double rotSpeed = f_time * 5.0; //the constant value is in radians/second
	clock_t			t;
	
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

	t = clock();
	paint_world(view);
	t = clock() - t;
	f_time = ((double)t) / CLOCKS_PER_SEC;		
}

int		loop_handler(t_view *view)
{
	static long f_time = 0;

	//printf("posX:%f - posY:%f\n",view->posX, view->posY);
	long cur;
	struct timeval t2;
	gettimeofday(&t2, NULL);
	cur = (t2.tv_sec * 1000) + (t2.tv_usec/1000);
	if(f_time == 0)
	{
		struct timeval t;
		gettimeofday(&t, NULL);
		f_time = (t.tv_sec * 1000) + (t.tv_usec/1000);
	}
    else if((cur - f_time) >= 200)
    {
        for(int i = 11; i < numSprites; i++)
	    {
	    	if (sprite[i].texture == 14)
	    		sprite[i].texture = 11;
	    	else
	    		sprite[i].texture += 1;

	    	if(worldMap[(int)(sprite[i].x - 32.1)][(int)sprite[i].y] == 0 ||
	    			worldMap[(int)(sprite[i].x + 32.1)][(int)sprite[i].y] == 0)
	    	{
	    		if (sprite[i].x  > view->posX)
		    		sprite[i].x -= 0.1;
		    	else if (sprite[i].x  < view->posX)
		    		sprite[i].x += 0.1;	
	    	}		    	

	    	if(worldMap[(int)sprite[i].x][(int)(sprite[i].y - 32.1)] == 0 ||
	    			worldMap[(int)sprite[i].x][(int)(sprite[i].y + 32.1)] == 0)
	    	{
		    	if (sprite[i].y  > view->posY)
		    		sprite[i].y -= 0.1;
		    	else if (sprite[i].y  < view->posY)
		    		sprite[i].y += 0.1;
		    }
	    }

	    if (view->fight_anim == 1)
	    {
	    	if (view->fight_tex == 17)
	    	{
	    		view->fight_tex = 15;
	    		view->fight_anim = 0;
	    	}
	    	else
	    		view->fight_tex += 1;
	    }
	    else if (view->fight_anim == 2)
	    {	    	
	    	if (view->fight_tex == 20)
	    	{
	    		view->fight_tex = 15;
	    		view->fight_anim = 0;
	    	}
	    	else  		
	    		view->fight_tex += 1;
	    }
	    paint_world(view);	
        f_time = 0;
    }
    view = (t_view *)view;
	return(0);
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

void init_images(t_view *view)
{

	int fight_w = 250;
	int fight_h = 150;

	view->textures[0].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/bluestone.xpm", &view->texWidth, &view->texHeight);
	view->textures[0].addr = mlx_get_data_addr(view->textures[0].ptr, &(view->textures[0].bpp),
						&(view->textures[0].s_line), &(view->textures[0].endian));

	view->textures[1].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/colorstone.xpm", &view->texWidth, &view->texHeight);
	view->textures[1].addr = mlx_get_data_addr(view->textures[1].ptr, &(view->textures[1].bpp),
						&(view->textures[1].s_line), &(view->textures[1].endian));

	view->textures[2].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/greystone.xpm", &view->texWidth, &view->texHeight);
	view->textures[2].addr = mlx_get_data_addr(view->textures[2].ptr, &(view->textures[2].bpp),
						&(view->textures[2].s_line), &(view->textures[2].endian));

	view->textures[3].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/mossy.xpm", &view->texWidth, &view->texHeight);
	view->textures[3].addr = mlx_get_data_addr(view->textures[3].ptr, &(view->textures[3].bpp),
						&(view->textures[3].s_line), &(view->textures[3].endian));

	view->textures[4].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/eagle.xpm", &view->texWidth, &view->texHeight);
	view->textures[4].addr = mlx_get_data_addr(view->textures[4].ptr, &(view->textures[4].bpp),
						&(view->textures[4].s_line), &(view->textures[4].endian));

	view->textures[5].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/redbrick.xpm", &view->texWidth, &view->texHeight);
	view->textures[5].addr = mlx_get_data_addr(view->textures[5].ptr, &(view->textures[5].bpp),
						&(view->textures[5].s_line), &(view->textures[5].endian));

	view->textures[6].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/wood.xpm", &view->texWidth, &view->texHeight);
	view->textures[6].addr = mlx_get_data_addr(view->textures[6].ptr, &(view->textures[6].bpp),
						&(view->textures[6].s_line), &(view->textures[6].endian));

	view->textures[7].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/purplestone.xpm", &view->texWidth, &view->texHeight);
	view->textures[7].addr = mlx_get_data_addr(view->textures[7].ptr, &(view->textures[7].bpp),
						&(view->textures[7].s_line), &(view->textures[7].endian));

	view->textures[8].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/barrel.xpm", &view->texWidth, &view->texHeight);
	view->textures[8].addr = mlx_get_data_addr(view->textures[8].ptr, &(view->textures[8].bpp),
						&(view->textures[8].s_line), &(view->textures[8].endian));

	view->textures[9].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/pillar.xpm", &view->texWidth, &view->texHeight);
	view->textures[9].addr = mlx_get_data_addr(view->textures[9].ptr, &(view->textures[9].bpp),
						&(view->textures[9].s_line), &(view->textures[9].endian));

	view->textures[10].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/greenlight.xpm", &view->texWidth, &view->texHeight);
	view->textures[10].addr = mlx_get_data_addr(view->textures[10].ptr, &(view->textures[10].bpp),
						&(view->textures[10].s_line), &(view->textures[10].endian));

	view->textures[11].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/devil/devil_0_0.xpm", &view->texWidth, &view->texHeight);
	view->textures[11].addr = mlx_get_data_addr(view->textures[11].ptr, &(view->textures[11].bpp),
						&(view->textures[11].s_line), &(view->textures[11].endian));

	view->textures[12].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/devil/devil_0_1.xpm", &view->texWidth, &view->texHeight);
	view->textures[12].addr = mlx_get_data_addr(view->textures[12].ptr, &(view->textures[12].bpp),
						&(view->textures[12].s_line), &(view->textures[12].endian));

	view->textures[13].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/devil/devil_0_2.xpm", &view->texWidth, &view->texHeight);
	view->textures[13].addr = mlx_get_data_addr(view->textures[13].ptr, &(view->textures[13].bpp),
						&(view->textures[13].s_line), &(view->textures[13].endian));

	view->textures[14].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/devil/devil_0_3.xpm", &view->texWidth, &view->texHeight);
	view->textures[14].addr = mlx_get_data_addr(view->textures[14].ptr, &(view->textures[14].bpp),
						&(view->textures[14].s_line), &(view->textures[14].endian));

	view->textures[15].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/fight/fight_0_3.xpm", &fight_w, &fight_h);
	view->textures[15].addr = mlx_get_data_addr(view->textures[15].ptr, &(view->textures[15].bpp),
						&(view->textures[15].s_line), &(view->textures[15].endian));

	view->textures[16].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/fight/fight_0_0.xpm", &fight_w, &fight_h);
	view->textures[16].addr = mlx_get_data_addr(view->textures[16].ptr, &(view->textures[16].bpp),
						&(view->textures[16].s_line), &(view->textures[16].endian));

	view->textures[16].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/fight/fight_0_1.xpm", &fight_w, &fight_h);
	view->textures[16].addr = mlx_get_data_addr(view->textures[16].ptr, &(view->textures[16].bpp),
						&(view->textures[16].s_line), &(view->textures[16].endian));

	view->textures[17].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/fight/fight_0_2.xpm", &fight_w, &fight_h);
	view->textures[17].addr = mlx_get_data_addr(view->textures[17].ptr, &(view->textures[17].bpp),
						&(view->textures[17].s_line), &(view->textures[17].endian));

	view->textures[18].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/fight/fight_1_0.xpm", &fight_w, &fight_h);
	view->textures[18].addr = mlx_get_data_addr(view->textures[18].ptr, &(view->textures[18].bpp),
						&(view->textures[18].s_line), &(view->textures[18].endian));

	view->textures[19].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/fight/fight_1_1.xpm", &fight_w, &fight_h);
	view->textures[19].addr = mlx_get_data_addr(view->textures[19].ptr, &(view->textures[19].bpp),
						&(view->textures[19].s_line), &(view->textures[19].endian));

	view->textures[20].ptr = mlx_xpm_file_to_image(view->mlx_ptr, "textures/fight/fight_1_2.xpm", &fight_w, &fight_h);
	view->textures[20].addr = mlx_get_data_addr(view->textures[20].ptr, &(view->textures[20].bpp),
						&(view->textures[20].s_line), &(view->textures[20].endian));
}

int			main(/*int argc, char **argv*/void)
{
	t_view view;

	view.mlx_ptr = mlx_init();
	view.s_width = 1000;
	view.s_height = 1000;	

	view.world_map = (int **)worldMap;
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

	view.win_ptr = mlx_new_window(view.mlx_ptr, view.s_width, view.s_height, "wolf 3D");

	view.img.ptr = mlx_new_image(view.mlx_ptr, view.s_width, view.s_height);
	view.img.addr = mlx_get_data_addr(view.img.ptr, &(view.img.bpp),
									&(view.img.s_line), &(view.img.endian));
	/*void *img_ptr = mlx_xpm_file_to_image(view.mlx_ptr, "textures/pillar.xpm", &view.texWidth, &view.texHeight);
	mlx_put_image_to_window(view.mlx_ptr, view.win_ptr, img_ptr, 300, 300);
	mlx_put_image_to_window(view.mlx_ptr, view.win_ptr, img_ptr, 364, 300);*/
	/*if ((argc < 2 || argc > 4) || !get_type(argv[1]) || (argc == 3 &&
			!get_type(argv[2])) || (argc == 4 && !get_type(argv[3])))
		ft_putendl("usage: fractal1 [fractal2] [fractal3]\nfractals: \
						\n- julia\n- mandelbrot\n- burning_ship\n- mandelbrat \
							\n- julia3\n- mandelbrot3\n- mandelbar3");
	else
	{*/
	paint_world(&view);		
	mlx_hook(view.win_ptr, 2, 0, key_handler, &view);
	mlx_hook(view.win_ptr, 4, 0, mouse_handler, &view);
	/*mlx_hook(view.win_ptr, 6, 0, motion_handler, &view);*/
	mlx_loop_hook(view.mlx_ptr, loop_handler, &view);
	mlx_loop(view.mlx_ptr);
	//}
	return (0);
}
