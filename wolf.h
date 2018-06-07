/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/08 17:35:38 by cfranco           #+#    #+#             */
/*   Updated: 2018/05/08 17:35:40 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H

# define ABS(x) (((x)<0)?-(x):(x))
# define NUM_THREADS 8

# include <libft.h>

#define mapWidth 24
#define mapHeight 24

typedef struct	s_image
{
	void		*ptr;
	char		*addr;
	int			bpp;
	int			s_line;
	int			endian;
}				t_image;

typedef struct	s_complex
{
	double		re;
	double		im;
}				t_complex;

typedef struct	s_view
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	t_image		img;
	double		zoom;
	int			s_width;
	int			s_height;
	double		move_x;
	double		move_y;
	double		move_xz;
	double		move_yz;
	double		zoom_x;
	double		zoom_y;
	double		zoom_hx;
	double		zoom_hy;
	t_complex	c;
	int			max;
	int			sat;
	int			bri;
	int			f_type;

	double		posX;
	double		posY;
	double		dirX;
	double		dirY;
	double		planeX;
	double		planeY;
	int			**world_map;
	int			texWidth;
	int			texHeight;

	t_image     textures[10];
	double		*z_buffer;
}				t_view;

typedef struct	s_sprite
{
  double x;
  double y;
  int texture;
}				t_sprite;

typedef struct	s_thread
{
	t_view		*view;
	int			total;
	int			cur;
}				t_thread;

int				get_type(char *type);
int				hsv_to_rgb(int h, int s, int v);
void			print_error();

int				pixel_calc(t_view *view, t_complex pos, t_complex offset);
int				color_calc(t_view *view, int x, int y);
void			*draw_fractal(void *args);
pthread_t		*create_thread(int c, int total, t_view *view);
void			threading(t_view *view);
void			create_view(void *mlx_ptr, char *type);

void			move_keys(t_view *view, int key);
int				key_handler(int key, t_view *view);
int				mouse_handler(int button, int x, int y, t_view *view);
int				motion_handler(int x, int y, t_view *view);

#endif
