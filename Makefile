# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cfranco <marvin 42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/21 15:28:39 by cfranco           #+#    #+#              #
#    Updated: 2018/06/11 16:44:52 by cfranco          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf

INC = -I minilibx_macos \
	  -I libft

LIBS = -L minilibx_macos \
	  -L libft -lft -lmlx

FRAMEWORKS = -framework OpenGL \
			 -framework AppKit

FLAGS = -Wall -Werror -Wextra -g

SRC = 	raycast.c \
		animations.c \
		draw.c \
		utils.c \
		hooks.c \
		wolf.c

OBJS = ft_*.o

ifdef DEBUG
	FLAGS += -g -fsanitize=address
endif

all: $(NAME)

$(NAME):
	make re -C libft/
	gcc $(FLAGS) $(INC) $(LIBS) $(FRAMEWORKS) $(SRC) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	make fclean -C libft/
	rm -f $(NAME);

re: fclean all
