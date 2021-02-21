#! /bin/bash
g++ ./Methods/RANDOM.cpp
./a.out < $1
rm a.out

g++ ./Methods/GREEDY.cpp
./a.out < $1
rm a.out

g++ ./Methods/EDDA.cpp
./a.out < $1
rm a.out

g++ ./Methods/EDDNSTE.cpp
./a.out < $1
rm a.out



