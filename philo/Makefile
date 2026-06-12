NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror
RM = rm -f

SRC_MANDATORY = actions.c \
				clean.c \
                data_parse.c \
                data.c \
                main.c

HEADERS_MANDATORY = philo.h

OBJS_MANDATORY = $(SRC_MANDATORY:.c=.o)


all: $(NAME)

$(NAME): $(OBJS_MANDATORY)
	$(CC) $(FLAGS) -o $(NAME) $(OBJS_MANDATORY)

%.o: %.c $(HEADERS_MANDATORY)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS_MANDATORY)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY:clean
