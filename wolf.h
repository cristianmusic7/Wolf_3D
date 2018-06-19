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
	int			w;
	int			h;
	int			max_z;
}				t_map;

typedef struct	s_coord
{
	double		x;
	double		y;
}				t_coord;

typedef struct	s_sprite
{
	double		x;
	double		y;
	int			texture;
	double		distance;
	int			alive;
	int			index;
}				t_sprite;

typedef struct	s_view
{
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	t_image		img;
	int			s_width;
	int			s_height;
	t_map		map;
	int			d_start_x;
	int			d_start_y;
	int			d_end_x;
	int			d_end_y;
	double		pos_x;
	double		pos_y;
	int			map_x;
	int			map_y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
	int			t_width;
	int			t_height;
	int			tex_x;
	int			tex_y;
	int			attack;
	int			damage;
	int			sound;
	int			alpha;
	int			num_sprites;
	t_sprite	*sprites;
	int			fight_tex;
	double		*z_buffer;
	int			fight_anim;
	t_image		*textures;
}				t_view;

int				paint_world(t_view *view);
void			print_error(int error_p);
void			move_keys(t_view *v, int key, double time);
int				key_handler(int key, t_view *view);
int				mouse_handler(int button, int x, int y, t_view *view);
int				motion_handler(int x, int y, t_view *view);
int				loop_handler(t_view *view);
int				exit_handler(t_view *view);
t_coord			get_transform(t_view *v, double inv_det, t_list *s_list);
void			main_cast(t_view *v, int x, double rx, double ry);
void			init_view(t_view *v, int sound);
void			load_textures(t_view *view);
int				draw_line(int x, int side, t_view *view);
int				draw_floor(int x, double floor_x, double floor_y, t_view *view);
void			sprites_helper(t_view *v, int sx, t_list *s_list, double tr_y);
void			render_sprites(t_view *v, double tr_y, t_list *s_list);
void			draw_sprites(t_view *v, double inv_det);
int				draw_fight(t_view *v, int sprite_x, int	sprite_w, int sprite_h);
void			enemy_anim(t_view *view);
void			fight_anim(t_view *view);
int				fill_image(int index, int x, int y, t_view *view);
int				compare(void *content, void *next_content);
void			read_line(t_map *map, char *line, int c);
void			read_input(char *file, t_map *map);

#endif
