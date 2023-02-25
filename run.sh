#!/bin/bash
gcc -c  threadpool.c -lpthread
gcc -c main.c -lpthread
gcc -o main main.o threadpool.o -lpthread
rm -rf threadpool.o
rm -rf main.o
./main
