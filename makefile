SRC_PATH := ./source
INC_PATH := ./include

SRC_FILES := $(wildcard $(SRC_PATH)/*.c)

.PHONY: run

run: program
	clear
	./program

program: $(SRC_FILES)
	clear
	gcc -o $@ $^ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -I $(INC_PATH) -Wall -Werror
