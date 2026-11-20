#!/bin/sh
set -eu
for n in 01 02 03 04 05 06; do
  g++ -std=c++17 -Wall -Wextra -pedantic "Task$n/main.cpp" -o "Task$n/task$n"
  "./Task$n/task$n"
done
