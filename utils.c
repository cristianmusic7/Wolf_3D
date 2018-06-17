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
	static	char *names[] = {"bluestone.xpm", "colorstone.xpm", "purplestone.xpm",
	"eagle.xpm", "mossy.xpm", "redbrick.xpm", "wood.xpm", "greystone.xpm", "barrel.xpm",
	"pillar.xpm", "greenlight.xpm", "devil/devil_0_0.xpm", "devil/devil_0_1.xpm",
	"devil/devil_0_2.xpm", "devil/devil_0_3.xpm", "devil/devil_0_4.xpm", "devil/devil_1_0.xpm",
	"devil/devil_2_1.xpm", "devil/devil_2_2.xpm", "devil/devil_2_3.xpm", "devil/devil_2_4.xpm",
	"fight/fight_0_3.xpm", "fight/fight_0_0.xpm", "fight/fight_0_1.xpm", "fight/fight_0_2.xpm", 
	"fight/fight_1_0.xpm", "fight/fight_1_1.xpm", "fight/fight_1_2.xpm"};
	int		c;
	int		c2;
	char	*name;
	//check seg fault when I punch really fast
	c = 0;
	c2 = 0;
	while (names[c] != '\0')
		c++;
	v->textures = (t_image *)malloc(sizeof(t_image) * c - 1);
	while (c2 < c - 1)
	{
		if (c2 > 20)
		{
			v->texWidth = 250;
			v->texHeight = 150;
		}
		name = ft_strjoin("textures/", names[c2]);
		v->textures[c2].ptr = mlx_xpm_file_to_image(v->mlx_ptr, name, &v->texWidth, &v->texHeight);
		v->textures[c2].addr = mlx_get_data_addr(v->textures[c2].ptr, &(v->textures[c2].bpp),
					&(v->textures[c2].s_line), &(v->textures[c2].endian));
		free(name);
		c2++;
	}
}

void	print_error(void)
{
	perror("Error");
	exit(0);
}

void	read_line(t_map *map, char *line, int c)
{
	char	**input;
	char	**in_tmp;
	int		x;

	x = 0;
	input = ft_strsplit(line, ' ');
	in_tmp = input;
	while (input[x])
		x++;
	if (map->width && map->width != x)
		print_error();
	else
		map->width = x;
	map->values[c] = (int *)malloc(sizeof(int) * x);
	x = 0;
	while (*input)
	{
		map->values[c][x] = ft_atoi(*input);
		free(*input);
		input++;
		x++;
	}
	free(*input);
	free(in_tmp);
	free(line);
}

void	read_input(char *file, t_map *map)
{
	int		fd;
	char	*line;
	int		res;
	int		l_count;

	l_count = 0;
	map->height = 0;
	map->width = 0;
	if ((fd = open(file, O_RDONLY)) == -1)
		print_error();
	while ((res = get_next_line(fd, &line)) && res != -1)
		(map->height)++;
	map->values = (int **)malloc(sizeof(int *) * map->height);
	if (close(fd) == -1)
		print_error();
	if ((fd = open(file, O_RDONLY)) == -1)
		print_error();
	while ((res = get_next_line(fd, &line)) && res != -1)
		read_line(map, line, l_count++);
	if (res == -1)
		print_error();
	free(line);
	line = NULL;
	if (close(fd) == -1)
		print_error();
}

int		compare(void *content, void *next_content)
{
	if (((t_sprite *)content)->distance < ((t_sprite *)next_content)->distance)
		return (0);
	else
		return (1);
}
