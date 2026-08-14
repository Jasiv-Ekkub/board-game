SRC_PATH := ./source
INC_PATH := ./include

SRC_FILES := $(wildcard $(SRC_PATH)/*.c)
INC_FILES := $(wildcard $(INC_PATH)/*)

.PHONY: run check-leaks

run: program
	clear
	./program

check-leaks: program
	clear
	valgrind --leak-check=full ./program

program: $(SRC_FILES) $(INC_FILES)
	clear
	gcc -o $@ $(SRC_FILES) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -I $(INC_PATH) -Wall -Werror -Wno-unused-function
