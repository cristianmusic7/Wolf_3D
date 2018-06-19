/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 15:14:24 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/11 15:14:26 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include "mlx.h"
#include <libft.h>
#include <stdio.h>

void	load_textures(t_view *v)
{
	int			c;
	char		*name;
	static char *names[] = {"bluestone.xpm", "colorstone.xpm",
	"purplestone.xpm", "eagle.xpm", "mossy.xpm", "redbrick.xpm", "wood.xpm",
	"greystone.xpm", "barrel.xpm", "pillar.xpm", "greenlight.xpm",
	"devil/0_0.xpm", "devil/0_1.xpm", "devil/0_2.xpm", "devil/0_3.xpm",
	"devil/0_4.xpm", "devil/1_0.xpm", "devil/2_1.xpm", "devil/2_2.xpm",
	"devil/2_3.xpm", "devil/2_4.xpm", "fight/fight_0_3.xpm",
	"fight/fight_0_0.xpm", "fight/fight_0_1.xpm", "fight/fight_0_2.xpm",
	"fight/fight_1_0.xpm", "fight/fight_1_1.xpm", "fight/fight_1_2.xpm"};

	c = 0;
	while (names[c] != '\0')
		c++;
	v->textures = (t_image *)malloc(sizeof(t_image) * --c);
	while ((--c >= 0) && (name = ft_strjoin("textures/", names[c])))
	{
		v->textures[c].ptr = mlx_xpm_file_to_image(v->mlx_ptr,
									name, &v->t_width, &v->t_height);
		v->textures[c].addr = mlx_get_data_addr(v->textures[c].ptr,
														&(v->textures[c].bpp),
					&(v->textures[c].s_line), &(v->textures[c].endian));
		free(name);
	}
}

void	print_error(int error_p)
{
	if (error_p == 1)
		perror("Error");
	else if (error_p == 0)
		ft_putendl("Error: Bad formatted file");
	else if (error_p == 2)
		ft_putendl("usage: ./wolf map_file_path [width] [height] \
		\n- width and height greater than 200\n- aspect ratio less than 3.0");
	exit(0);
}

void	read_line(t_map *map, char *line, int c)
{
	char	**input;
	int		x;
	int		c2;

	x = 0;
	input = ft_strsplit(line, ' ');
	while (input[x])
		map->w = x++;
	if ((++map->w && map->w != x))
		print_error(0);
	map->values[c] = (int *)malloc(sizeof(int) * x);
	x = 0;
	while (input[x])
	{
		c2 = -1;
		while (input[x][++c2])
			if (!(input[x][c2] >= '0' && input[x][c2] <= '9'))
				print_error(0);
		if (!(map->values[c][x] = ft_atoi(input[x])) &&
				(!x || !c || x == map->w - 1 || c == map->h - 1))
			print_error(0);
		free(input[x++]);
	}
	free(input);
	free(line);
}

void	read_input(char *file, t_map *map)
{
	int		fd;
	char	*line;
	int		res;
	int		l_count;

	l_count = 0;
	map->w = 0;
	map->h = 0;
	if ((fd = open(file, O_RDONLY)) == -1)
		print_error(1);
	while ((res = get_next_line(fd, &line)) && res != -1)
	{
		(map->h)++;
		free(line);
	}
	map->values = (int **)malloc(sizeof(int *) * map->h);
	if (close(fd) == -1 || (fd = open(file, O_RDONLY)) == -1)
		print_error(1);
	while ((res = get_next_line(fd, &line)) && res != -1)
		read_line(map, line, l_count++);
	if (res == -1 || close(fd) == -1)
		print_error(1);
	if (!map->w || !map->h)
		print_error(0);
	free(line);
}

int		compare(void *content, void *next_content)
{
	if (((t_sprite *)content)->distance < ((t_sprite *)next_content)->distance)
		return (0);
	else
		return (1);
}
