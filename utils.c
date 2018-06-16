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
	int w_count = 0;

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
	while (*input)
	{
		map->values[c][w_count] = ft_atoi(*input);
		free(*input);
		input++;
		w_count++;
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
	int 	l_count = 0;

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

int compare(void *content, void *next_content)
{
	if (((t_sprite *)content)->distance < ((t_sprite *)next_content)->distance)
		return (0);
	else
		return (1);
}