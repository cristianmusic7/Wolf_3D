/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 22:52:07 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/11 22:52:11 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include "mlx.h"
#include <libft.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int exit_handler(t_view *view)
{
	mlx_destroy_window(view->mlx_ptr, view->win_ptr);
	exit(0);
	return(0);
}

int		key_handler(int key, t_view *view)
{
	if (key == 53)
		exit_handler(view);
	else if (key == 123 || key == 124 || key == 125 || key == 126)
		move_keys(view, key);
	return (0);
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

int		loop_handler(t_view *view)
{
	static long f_time = 0;
	long cur;
	struct timeval t;
	struct timeval t2;
	gettimeofday(&t2, NULL);

	cur = (t2.tv_sec * 1000) + (t2.tv_usec/1000);
	if(f_time == 0)
	{		
		gettimeofday(&t, NULL);
		f_time = (t.tv_sec * 1000) + (t.tv_usec/1000);
	}
    else if((cur - f_time) >= 200)
    {
        for(int i = 11; i < view->num_sprites; i++)
	    {	    	
	    	//if (view->sprites[i].chase)
	    	//{
	    		if (view->sprites[i].texture == 14)
		    		view->sprites[i].texture = 11;
		    	else
		    		view->sprites[i].texture += 1;

	    		/*if(view->map.values[(int)(view->sprites[i].x - 1)][(int)view->sprites[i].y] == 0 &&
	    			view->map.values[(int)(view->sprites[i].x + 1)][(int)view->sprites[i].y] == 0)
		    	{*/
		    		if (view->sprites[i].x  > view->posX)
			    		view->sprites[i].x -= 0.1;
			    	else if (view->sprites[i].x  < view->posX)
			    		view->sprites[i].x += 0.1;	
		    	//}		    	

		    	/*if(view->map.values[(int)view->sprites[i].x][(int)(view->sprites[i].y - 1)] == 0 &&
		    			view->map.values[(int)view->sprites[i].x][(int)(view->sprites[i].y + 1)] == 0)
		    	{*/
			    	if (view->sprites[i].y  > view->posY)
			    		view->sprites[i].y -= 0.1;
			    	else if (view->sprites[i].y  < view->posY)
			    		view->sprites[i].y += 0.1;
			    //}
	    	//}	    	
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
