### FILES AND DIRECTORIES ###
NAME = fractol
GC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c89 -g
INCLUDES = include

DIR_FRACTOL = src/
FILES_FRACTOL = \
	fractol events2D manage2D quatmath imagemath voxels

FILES_SRC += $(addprefix $(DIR_FRACTOL), $(FILES_FRACTOL))

DIR_OBJ = obj/
FILES_OBJ = $(addprefix $(DIR_OBJ), $(addsuffix .o, $(FILES_FRACTOL)))


#####################
### PROGRAM RULES ###
#####################

all: $(NAME)

$(NAME): $(FILES_OBJ)
	$(GC) $(FILES_OBJ) -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)
#	@ar rsc $(NAME) $(FILES_OBJ)
	@echo "SUCCESSFULLY COMPILED $(NAME)"

$(DIR_OBJ)%.o: $(DIR_FRACTOL)%.c | LIB
	@$(GC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@
	@echo "COMPILED OBJECT $@"

LIB: DIRECTORIES
#	@make -C mlx
#	@cp mlx/libmlx.a .
#	@mv libmlx.a $(NAME)



DIRECTORIES:
	@mkdir -p $(DIR_OBJ)
	

bonus: all

###################
### CLEAN RULES ###
###################

clean:
	@make clean -C mlx
	@rm -rf $(DIR_OBJ)
	@echo "-FRACTOL- OBJECTS DELETED. CLEAN SUCCESSFUL!"

fclean: clean
	@rm -rf mlx/libmlx.a
	@rm -rf $(NAME)
	@echo "-MLX- LIBRARY DELETED. CLEAN SUCCESSFUL!"
	@echo "-FRACTOL- LIBRARY DELETED. CLEAN SUCCESSFUL!"

re:	fclean all
