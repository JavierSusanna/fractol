NAME	= so_long

CC		= gcc

CFLAGS	= -Wall -Werror -Wextra  -g3 -fsanitize=address

SRCS	= animation.c move.c map_validator.c utils.c draw_map.c main.c hooks.c \
		  animation_utils.c animation_option.c newtry.c
OBJS	= ${SRCS:.c=.o}

SRCS_DIR = src
OBJS_DIR = obj

LIBFT_NAME	= libft.a

MLX_NAME_MAC = libmlx.a

MLX_NAME_LINUX = libmlx_Linux.a

LIBFT	= lib/libft/

SRCS_PATHS 	= $(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS_PATHS 	= $(addprefix $(OBJS_DIR)/, $(OBJS))

RM		= rm -rf

OS		:= $(shell uname -s)

ifeq (${OS},Linux)
	CFLAGS += -D LINUX
	MLX = lib/minilibx-linux/
	MLX_NAME = ${MLX_NAME_LINUX}
	LIBS = -lft -lmlx_Linux -lXext -lX11 -lm -lz
endif
ifeq (${OS},Darwin)
	CFLAGS += -D OSX
	MLX	= lib/minilibx_macos/
	MLX_NAME = ${MLX_NAME_MAC}
	LIBS = -lft -lmlx -framework OpenGL -framework AppKit
endif

MLX_LNK	= -L ${MLX}

LIB_LNK	= -L ${LIBFT}

INCLUDES = -I ./${LIBFT}inc -I ./${MLX} -I ./inc

all: ${NAME}

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c
	@${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@ 

$(NAME): ${LIBFT_NAME} ${MLX_NAME} ${OBJS_DIR} ${OBJS_PATHS} 
	@${CC} ${CFLAGS} ${LIB_LNK} ${MLX_LNK} ${OBJS_PATHS} -o ${NAME} ${LIBS}

${LIBFT_NAME} :
	make -C ${LIBFT}
	@cp ${LIBFT}/${LIBFT_NAME} .

${MLX_NAME} :
	@make -C ${MLX}
	@cp ${MLX}/${MLX_NAME} .

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR) 2> /dev/null

clean:
		make clean -C ${LIBFT}
		@${RM} *.a ${OBJS_DIR}/*.o *.dSYM

fclean:		clean
		make fclean -C ${LIBFT}
		@${RM} ${NAME}

re:		fclean all

.PHONY:	clean re fclean all
