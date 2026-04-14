##
## EPITECH PROJECT, 2025
## makefile
## File description:
## makefile
##

CC	=	epiclang
CC_TESTS	=	clang
OBJ_DIR	=	obj
STC_DIR	=	src

SRC	=	$(shell find $(STC_DIR) -name "*.c")
SRC_NO_MAIN	=	$(filter-out $(STC_DIR)/main.c, $(SRC))
TESTS_SRC	=	$(shell find tests -name "*.c")
INC_DIR	=	-Iinclude
OBJ	=	$(patsubst $(STC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

DEPS	=	$(OBJ:.o=.d)

FLAGS	=	-Wall -Wextra -W -Werror -MMD
CRITERION	=	-lcriterion --coverage
BUILD	=	42sh
CRITERION_EXEC	=	unit_tests

all:	$(BUILD)

$(BUILD):	$(OBJ)
	$(CC) -o $(BUILD) $(OBJ) $(FLAGS) $(INC_DIR)

-include $(DEPS)

$(OBJ_DIR)/%.o:	$(STC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -rf $(BUILD)

re:	fclean all

tests_run: $(BUILD) tests_clean
	$(CC_TESTS) -o $(CRITERION_EXEC) $(SRC_NO_MAIN) $(TESTS_SRC) $(CRITERION) $(FLAGS) $(INC_DIR)
	./$(CRITERION_EXEC)

tests_clean:
	rm -rf $(CRITERION_EXEC)*
	rm -rf *.gcda *.gcno
