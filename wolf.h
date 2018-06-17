/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 23:27:32 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/11 23:27:34 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H

# define ABS(x) (((x)<0)?-(x):(x))
# define MAX(x, y) x > y ? x : y
# define NUM_THREADS 8

# include <libft.h>

typedef struct	s_image
{
	void		*ptr;
	char		*addr;
	int			bpp;
	int			s_line;
	int			endian;
}				t_image;

typedef struct	s_map
{
	int			**values;
	int			width;
	int			height;
	int			max_z;	
}				t_map;

typedef struct	s_complex
{
	double		re;
	double		im;
}				t_complex;

typedef struct	s_sprite
{
  double		x;
  double		y;
  int			texture;
  double		distance;
  int 			index;
}				t_sprite;

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
	t_map		map;
	int			drawStartX;
	int			drawStartY;
	int			drawEndX;
	int			drawEndY;

	double		posX;
	double		posY;
	int			mapX;
	int			mapY;
	double		dirX;
	double		dirY;
	double		planeX;
	double		planeY;
	int			texWidth;
	int			texHeight;
	int			tex_x;
	int			tex_y;
	
	int			num_sprites;
	t_sprite    *sprites;
	int			fight_tex;
	double		*z_buffer;
	int			fight_anim;	
	t_image     textures[21];
}				t_view;

typedef struct	s_thread
{
	t_view		*view;
	int			total;
	int			cur;
}				t_thread;


int				paint_world(t_view *view);
void			print_error();
void			combSort(int* order, double* dist, int amount);

void			move_keys(t_view *view, int key);
int				key_handler(int key, t_view *view);
int				mouse_handler(int button, int x, int y, t_view *view);
int				motion_handler(int x, int y, t_view *view);
int				loop_handler(t_view *view);
int				exit_handler(t_view *view);

void			init_images(t_view *view);
int				draw_line(int x, int side, t_view *view);
int				draw_floor(int x, double floorXWall, double floorYWall, t_view *view);
void			draw_sprites2(t_view *view, int spriteScreenX, t_list *s_list, double transformY);
int				draw_sprites(t_view *v, double invDet);
int				draw_fight(t_view *v, int sprite_x, int	sprite_w, int sprite_h);
void			enemy_anim(t_view *view);
void			fight_anim(t_view *view);
int				fill_image(int index, int x, int y, int dark, t_view *view);
int				compare(void *content, void *next_content);

void			read_line(t_map *map, char *line, int c);
void			read_input(char *file, t_map *map);
void			print_error();
void			create_point(char *input, t_map *map);

#endif
