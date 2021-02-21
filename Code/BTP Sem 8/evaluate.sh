#! /bin/bash
g++ RANDOM.cpp
./a.out < $1
rm a.out

g++ GREEDY.cpp
./a.out < $1
rm a.out

g++ EDDA.cpp
./a.out < $1
rm a.out

g++ EDDNSTE.cpp
./a.out < $1
rm a.out



