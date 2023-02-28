### FILES AND DIRECTORIES ###
NAME = fractol
GC = gcc
CFLAGS = -std=c89 -g #-Wall -Werror -Wextra -g # -Imlx # -fsanitize=address

DIR_FRACTOL = src
DIR_OBJ = obj

FILES_FRACTOL = \
	fractol init quatmath events2d events2d2 events3d manage2d create2d \
	create3d cloud

FILES_SRC += $(addprefix $(DIR_FRACTOL)/, $(FILES_FRACTOL))
FILES_OBJ = $(addprefix $(DIR_OBJ)/, $(addsuffix .o, $(FILES_FRACTOL)))

MLX_NAME_LINUX = libmlx_Linux

MLX_NAME_MAC = libmlx_Mac

RM		= rm -rf

OS		:= $(shell uname -s)

ifeq (${OS},Linux)
	CFLAGS += -D LINUX
	MLX = lib/minilibx-linux/
	MLX_NAME = ${MLX_NAME_LINUX}.a
	LIBS = -lm -lmlx_Linux -lXext -lX11 -lz
endif
ifeq (${OS},Darwin)
	CFLAGS += -D OSX
	MLX	= lib/minilibx-mac/
	MLX_NAME = ${MLX_NAME_MAC}.a
	LIBS = -lmlx -framework OpenGL -framework AppKit
endif

MLX_LNK	= -L ${MLX}

INCLUDES = -I ./${MLX} -I ./inc

#####################
### PROGRAM RULES ###
#####################

all: $(NAME)

${DIR_OBJ}/%.o: ${DIR_FRACTOL}/%.c
	${GC} ${CFLAGS} ${INCLUDES} -c $< -o $@ ${LIBS}
	@echo "COMPILED OBJECT $@"

$(NAME): ${MLX_NAME} ${DIR_OBJ} ${FILES_OBJ}
	${GC} ${CFLAGS} ${FILES_OBJ} -o ${NAME} ${MLX_LNK} ${LIBS}
	@echo "SUCCESSFULLY COMPILED $(NAME)"

${MLX_NAME} :
	make -C ${MLX}
	cp ${MLX}/${MLX_NAME} .

$(DIR_OBJ):
	mkdir -p $(DIR_OBJ) 2> /dev/null

bonus: all

###################
### CLEAN RULES ###
###################

clean:
	@make clean -C ${MLX}
#	@${RM} *.a ${DIR_OBJ}/*.o *.dSYM
	@${RM} *.a ${DIR_OBJ} *.dSYM
	@echo "-FRACTOL- OBJECTS DELETED. CLEAN SUCCESSFUL!"

fclean: clean
	@${RM} ${MLX_NAME}
	@${RM} ${NAME}
	@echo "GRAPHICS LIBRARY DELETED. CLEAN SUCCESSFUL!"
	@echo "-FRACTOL- LIBRARY DELETED. CLEAN SUCCESSFUL!"

re:	fclean all

.PHONY:	clean re fclean all
