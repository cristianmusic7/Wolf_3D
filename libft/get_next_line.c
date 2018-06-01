/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 16:04:59 by cfranco           #+#    #+#             */
/*   Updated: 2018/03/12 16:05:01 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

t_list	*handle_main_lst(t_list **files, const int fd)
{
	t_list	*tmp;
	t_lines cur_file;

	cur_file.fd = fd;
	cur_file.remainder = NULL;
	tmp = NULL;
	if (!*files)
		ft_lstpushback(files, &cur_file, sizeof(t_lines));
	else
	{
		tmp = *files;
		while (tmp->next != NULL)
		{
			if (((t_lines *)tmp->content)->fd == fd)
				break ;
			tmp = tmp->next;
		}
		if (!(((t_lines *)tmp->content)->fd == fd))
		{
			ft_lstpushback(files, &cur_file, sizeof(t_lines));
			tmp = tmp->next;
		}
		return (tmp);
	}
	return (*files);
}

int		handle_buf(char **line, char *buf, char **remainder, char *rem_tmp)
{
	char *tmp;

	if (!*line)
		*line = ft_strdup("");
	tmp = *line;
	if ((rem_tmp = ft_strchr(buf, '\n')))
	{
		*rem_tmp = '\0';
		if (*remainder)
			free(*remainder);
		*remainder = ft_strdup(rem_tmp + 1);
		if (*buf == '\n')
			return (0);
		*line = ft_strjoin(*line, buf);
		free(tmp);
		return (0);
	}
	else
	{
		*line = ft_strjoin(*line, buf);
		free(*remainder);
		*remainder = NULL;
	}
	free(tmp);
	return (1);
}

int		handle_file(const int fd, char **line, char **remainder, char *rem_tmp)
{
	char	*buf;
	int		ret;
	int		res;

	buf = (char *)ft_memalloc(sizeof(char) * (GNL_BUFF_SIZE + 1));
	res = 1;
	while ((ret = read(fd, buf, GNL_BUFF_SIZE)) && ret != -1)
	{
		buf[ret] = '\0';
		if (!(handle_buf(line, buf, remainder, rem_tmp)))
			break ;
	}
	if (ret == -1 || (!ret && *buf == '\0'))
	{
		if (!rem_tmp && *remainder && **remainder)
		{
			*remainder = NULL;
			return (1);
		}
		res = ret;
	}
	free(buf);
	return (res);
}

int		get_next_line(const int fd, char **line)
{
	int				res;
	char			*rem_tmp;
	t_list			*l_tmp;
	static t_list	*files;

	rem_tmp = NULL;
	if (read(fd, 0, 0) < 0 || !line)
		return (-1);
	l_tmp = handle_main_lst(&files, fd);
	*line = NULL;
	if (((t_lines *)l_tmp->content)->remainder)
	{
		if ((rem_tmp = ft_strchr(((t_lines *)l_tmp->content)->remainder, '\n')))
		{
			*rem_tmp = '\0';
			*line = ft_strjoin("", ((t_lines *)l_tmp->content)->remainder);
			ft_strcpy(((t_lines *)l_tmp->content)->remainder, rem_tmp + 1);
			return (1);
		}
		else
			*line = ft_strjoin("", ((t_lines *)l_tmp->content)->remainder);
	}
	res = handle_file(fd, line,
		&((t_lines *)l_tmp->content)->remainder, rem_tmp);
	return (res);
}
