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

int draw_line(int x, int texX, int side, t_view *view)
{
	int y_start;
	int y_end;
	int d;
	int texY;	

    if((y_start = -(int)(view->s_height / view->z_buffer[x]) / 2 + view->s_height / 2) < 0)
    	y_start = 0;
    if((y_end = (int)(view->s_height / view->z_buffer[x]) / 2 + view->s_height / 2) >= view->s_height)
    	y_end = view->s_height - 1;
	while (y_start < y_end)
	{
		d = y_start * 256 - view->s_height * 128 + (int)(view->s_height / view->z_buffer[x]) * 128;
        texY = ((d * view->texHeight) / (int)(view->s_height / view->z_buffer[x])) / 256;
        if(side == 2 || side == 3)
        	fill_image(side, x, y_start, texX, texY, 1, view);
        else
        	fill_image(side, x, y_start, texX, texY, 0, view);
		y_start++;
	}
	return(0);
}

int draw_floor(int x, double floorXWall, double floorYWall, t_view *view)
{
	double	weight;
	int		floorTexX;
	int		floorTexY;
	double currentFloorX;
	double currentFloorY;
	int y;

	if((y = (int)(view->s_height / view->z_buffer[x]) / 2 + view->s_height / 2) >= view->s_height)
    	y = view->s_height - 1;	
	if (y++ < 0)
		y = view->s_height;	
	while (y < view->s_height)
	{
		weight = (view->s_height / (2.0 * y - view->s_height))/view->z_buffer[x];
		currentFloorX = weight * floorXWall + (1.0 - weight) * view->posX;
		currentFloorY = weight * floorYWall + (1.0 - weight) * view->posY;
		floorTexX = (int)(currentFloorX * view->texWidth) % view->texWidth;
		floorTexY = (int)(currentFloorY * view->texHeight) % view->texHeight;
		fill_image(4, x, y, floorTexX, floorTexY, 1, view);
		fill_image(6, x, (view->s_height - y), floorTexX, floorTexY, 1, view);	
		y++;
	}
	return(0);
}
void draw_sprites2(t_view *view, int spriteScreenX, t_list *s_list, double transformY)
{
	int spriteHeight = ABS((int)(view->s_height / (transformY))); 
	int spriteWidth = ABS( (int)(view->s_width / (transformY)));
	int y;

	if((view->drawStartY = -spriteHeight / 2 + view->s_height / 2) < 0) 
		view->drawStartY = 0;	
	if((view->drawEndY = spriteHeight / 2 + view->s_height / 2) >= view->s_height)
	  	view->drawEndY = view->s_height - 1;
	if((view->drawStartX = -spriteWidth / 2 + spriteScreenX) < 0)
		view->drawStartX = 0;
	if((view->drawEndX = spriteWidth / 2 + spriteScreenX) >= view->s_width)
		view->drawEndX = view->s_width - 1;
	while (view->drawStartX < view->drawEndX)
    {
        int texX = (int)(256 * (view->drawStartX++ - (-spriteWidth / 2 + spriteScreenX)) * view->texWidth / spriteWidth) / 256;
        if((view->drawStartX-1) > 0 < view->s_width && transformY < view->z_buffer[(view->drawStartX-1)])
        {
        	y = view->drawStartY;
        	while(y < view->drawEndY)
	        {
	          int d = (y++) * 256 - view->s_height * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
	          int texY = ((d * view->texHeight) / spriteHeight) / 256;
	          fill_image(view->sprites[((t_sprite *)s_list->content)->index].texture, view->drawStartX, y - 1, texX, texY, 2, view);
		    }
		}
    }
}

int draw_sprites(t_view *view)
{
	t_list *s_list;
	double spriteX;
	double spriteY;
	int c;

	s_list = NULL;
	c = 0;
	while(c < view->num_sprites)
	{
		view->sprites[c].distance = ((view->posX - view->sprites[c].x) * (view->posX - view->sprites[c].x)
									+ (view->posY - view->sprites[c].y) * (view->posY - view->sprites[c].y));
		ft_lstpushback(&(s_list), &(view->sprites[c++]), sizeof(t_sprite));
	}
	s_list = ft_lstsort(s_list, compare);
	while (s_list != NULL)
	{
		spriteX = view->sprites[((t_sprite *)s_list->content)->index].x - view->posX;
		spriteY = view->sprites[((t_sprite *)s_list->content)->index].y - view->posY;		
		double invDet = 1.0 / (view->planeX * view->dirY - view->dirX * view->planeY);
		double transformX = invDet * (view->dirY * spriteX - view->dirX * spriteY);
		double transformY = invDet * (-view->planeY * spriteX + view->planeX * spriteY);

		if(transformY > 0)
		{
	  	  int spriteScreenX = (int)((view->s_width / 2) * (1 + transformX / transformY));
		  draw_sprites2(view, spriteScreenX, s_list, transformY);		  
	    }
	    s_list = s_list->next;
	}
	return(0);
}

int draw_fight(t_view *view)
{
	int stripe;
	int spriteScreenX = (int)(view->s_width / 2);
	int spriteWidth = (int)(view->s_width * 0.75);
	int spriteHeight = (int)(view->s_height / 2);
	int y;

	if((view->drawStartY = view->s_height - spriteHeight) < 0)
		view->drawStartY = 0;
	if((view->drawEndY = view->s_height) >= view->s_height)
		view->drawEndY = view->s_height - 1;
	if((stripe = -spriteWidth / 2 + spriteScreenX) < 0)
		stripe = 0;
	if((view->drawEndX = spriteWidth / 2 + spriteScreenX) >= view->s_width)
		view->drawEndX = view->s_width - 1;
	while(stripe < view->drawEndX)
   	{
		int texX = (int)(256 * (stripe++ - (-spriteWidth / 2 + spriteScreenX)) * 250 / spriteWidth) / 256;
		y = view->drawStartY;
		while(y < view->drawEndY)
	    {
	      	int texY = (int)(256 * (y++ - (view->s_height - spriteHeight)) * 150 / spriteHeight) / 256;
	      	fill_image(view->fight_tex, stripe, y - 1, texX, texY, 2, view);
	    }
    }
	return(0);
}
