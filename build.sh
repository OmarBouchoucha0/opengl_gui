#!/bin/sh

mkdir -p out

gcc -c src/glad.c -Iinclude -Wno-sign-conversion -o out/glad.o

gcc -c src/main.c -Iinclude -Wall -Wextra -Wshadow -Wconversion -Wcast-align \
    -Werror -Wno-unused-function -Wno-unused-variable -fno-exceptions -o out/main.o

gcc out/main.o out/glad.o -o ./out/app -lglfw -lGL -ldl -lpthread
