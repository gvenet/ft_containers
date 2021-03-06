#!/bin/bash
GRN="\033[1;32m"
RED="\033[1;31m"
BLUE="\033[1;34m"
ENDC="\033[0m"

CHECK_MARK="${GRN}\xE2\x9C\x94${ENDC}"
ERROR_MARK="${RED}\xE2\x9C\x96${ENDC}"

echo -e "${GRN}TEST_VECTOR_FUNCTIONS${ENDC}"
nf=7
n=1
while ( ((n<nf)) ) do 
	id=$(find . -name  "test${n}_*")
	./func.sh $id
	if [[ $? -ne 0 ]]; then
		echo "exit"
		exit 1
	fi
	((n+=1))
done

echo -e "${GRN}\nTEST_VECTOR_LEAKS${ENDC}"
n=1
while ( ((n<nf)) ) do
	./leaks.sh "test$n"
	((n+=1))
done

rm ft std diff_log
rm test1 test2 test3 test4 test5 test6

echo -e "${GRN}\nTEST_VECTOR_SPEED${ENDC}"
echo "./test0_vector_speed.cpp"
echo

clang++ -fsanitize=address test0_vector_speed.cpp -o vector_speed

./vector_speed 10000000 0 0
./vector_speed 10000000 10000000 0
./vector_speed 50000 0 50000
./vector_speed 50000 50000 50000

rm vector_speed
