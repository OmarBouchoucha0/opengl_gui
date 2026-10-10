#!/bin/sh

gcc -Wall -Wextra -Wshadow -Wconversion -Wcast-align -Werror -Wno-unused-function \
    -Wno-unused-variable -fno-exceptions \
    main.c -o ./out/app -lglfw
