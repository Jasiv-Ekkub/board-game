SRC_PATH := ./source
INC_PATH := ./include

SRC_FILES := $(wildcard $(SRC_PATH)/*.c)
INC_FILES := $(wildcard $(INC_PATH)/*)

.PHONY: run check-leaks ship-linux

run: buisnessland
	clear
	./buisnessland

check-leaks: program
	clear
	valgrind --leak-check=full ./buisnessland

buisnessland: $(SRC_FILES) $(INC_FILES)
	clear
	gcc -o $@ $(SRC_FILES) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -I $(INC_PATH) -Wall -Werror -Wno-unused-function

ship-linux: buisnessland
	zip -r Buisnessland-Linux.zip ./resource ./buisnessland
