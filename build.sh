#!/bin/sh

mkdir -p out

g++ -c src/glad.c -Iinclude -Wno-sign-conversion -g -o out/glad.o

g++ -c src/main.cpp -Iinclude -g -o out/main.o

g++ out/main.o out/glad.o -o ./out/app -lglfw -lGL -lm -ldl -lpthread
