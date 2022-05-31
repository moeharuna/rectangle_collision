CC=clang++
CFLAGS=-Wall -std=c++17 -g
SOURCE= src/main.cpp
OUTPUT = rectangle_collision
all: $(SOURCE)
	$(CC) $(CFALGS) -o $(OUTPUT) $(SOURCE)
clean:
	$(RM) $(OUTPUT)


# end
