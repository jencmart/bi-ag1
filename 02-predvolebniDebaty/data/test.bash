#!/bin/bash

echo '########################################## Compile'
g++ -std=c++11 -Wall -Wno-long-long -O2 -pedantic source/main.cpp -g -o bin/main.out

echo '########################################## Run'
echo "### ZAKLADNI TESTY"
./bin/main.out < ./test/01.txt
echo '---------------'
./bin/main.out < ./test/02.txt
echo '---------------'
./bin/main.out < ./test/03.txt


echo "##########################################"
echo "### NEMAM VYSLEDEK"

echo "###### Strom je prazdny"
echo "###### Out of range zleva"
echo "###### Out of range zprava"
echo "###### Jsem uprostred, ale v range nejsou cisla #1"
echo "###### Jsem uprostred, ale v range nejsou cisla #2"
valgrind ./bin/main.out < ./test/test1.txt


echo "##########################################"
echo "### VYSLEDEK - JEDEN PRVEK"

echo "###### Jediny -> 90"
valgrind ./bin/main.out < ./test/test2.txt
echo "###### Jeden na zacatku -> 4"
echo "###### Jeden uprostred  -> 6"
echo "###### Jeden na konci   -> 7"
echo "###### INF az prvni     -> 4"
echo "###### Posledni az INF  -> 7"
valgrind ./bin/main.out < ./test/test3.txt


echo "##########################################"
echo "### VYSLEDEK - PRESNA RANGE"

echo "###### Presny FULL  lichy -> 8"
echo "###### Presny INF L lichy -> 8"
echo "###### Presny LEFT  lichy -> 6"
echo "###### Presny RIGHT lichy -> 10"
echo "###### Presny R INF lichy -> 8"
echo "###### Presny MID   lichy -> 8"
valgrind ./bin/main.out < ./test/test4.txt
echo "###### Presny FULL  sudy -> 8"
echo "###### Presny INF L sudy -> 8"
echo "###### Presny LEFT  sudy -> 6"
echo "###### Presny RIGHT sudy -> 10"
echo "###### Presny R INF sudy -> 8"
echo "###### Presny MID   sudy -> 8"
valgrind ./bin/main.out < ./test/test5.txt

echo "##########################################"
echo "### VYSLEDEK - NEPRESNA RANGE"

echo "###### Nepresne LEFT  lichy -> 6"
echo "###### Nepresne INF L lichy -> 4"
echo "###### Nepresne RIGHT lichy -> 12"
echo "###### Nepresne R INF lichy -> 14"
echo "###### Nepresne MID   lichy -> 10"
valgrind ./bin/main.out < ./test/test6.txt

echo "###### Nepresne INF L sudy -> 6"
echo "###### Nepresne R INF sudy -> 12"
echo "###### Nepresne MID   sudy -> 8"
valgrind ./bin/main.out < ./test/test7.txt

echo "###### Nepresne FULL  lichy -> 8"
valgrind ./bin/main.out < ./test/test8.txt
echo "###### Nepresne FULL  sudy -> 8"
valgrind ./bin/main.out < ./test/test9.txt
