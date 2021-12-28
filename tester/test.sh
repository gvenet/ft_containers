#!/bin/bash

GRN="\033[1;32m"
RED="\033[1;31m"
BLUE="\033[1;34m"
ENDC="\033[0m"

CHECK_MARK="${GRN}\xE2\x9C\x94${ENDC}"
ERROR_MARK="${RED}\xE2\x9C\x96${ENDC}"

#TEST1 ==============================================================
clang++ test1_insert_rand_erase_rand.cpp -o "test1"
./test1 5
diff ft.txt  std.txt

d=5
((x+=5))

echo -e "${BLUE}test1_insert_rand_erase_rand"
while ( ((x<=640)) ) do
((c*=0))
	while ( diff ft.txt std.txt > diff_log && ((c<$d)) ) do
		./test1 $x
		(( c+=1 ))
		echo -ne "${BLUE}$c / $d > test $x\r" 
	done
	if (diff ft.txt std.txt > diff_log) then
		echo -e "test $x ${CHECK_MARK}                    "
	else
		echo -e "test $x ${ERROR_MARK}                    "
		exit 1
	fi
((x*=2))
done

leaks -atExit -- ./test1 100
echo -e "${BLUE}leaks 100 ${CHECK_MARK}"

#TEST2 ======================================================================
clang++ test2_map_modifier.cpp -o "test2"
./test2
echo -ne "${BLUE}test2_map_modifier\r"
if (diff ft.txt std.txt > diff_log) then
	echo -e "test2_map_modifier ${CHECK_MARK}"
else
	echo -e "test2_map_modifier ${ERROR_MARK}"
	exit 1
fi

#TEST3 ======================================================================
clang++ test3_map_correction.cpp -o "test3"
./test3
echo -ne "${BLUE}test3_map_correction\r"
if (diff ft.txt std.txt > diff_log) then
	echo -e "test3_map_correction ${CHECK_MARK}"
else
	echo -e "test3_map_correction ${ERROR_MARK}"
	exit 1
fi

#TEST4 ======================================================================
clang++ test4_map_operation.cpp -o "test4"
./test4
echo -ne "${BLUE}test4_map_operation\r"
if (diff ft.txt std.txt > diff_log) then
	echo -e "test4_map_operation ${CHECK_MARK}"
else
	echo -e "test4_map_operation ${ERROR_MARK}"
	exit 1
fi

#TEST5 ======================================================================
clang++ test5_map_access.cpp -o "test5"
./test5
echo -ne "${BLUE}test5_map_access\r"
if (diff ft.txt std.txt > diff_log) then
	echo -e "test5_map_access ${CHECK_MARK}"
else
	echo -e "test5_map_access ${ERROR_MARK}"
	exit 1
fi

#TEST6 ======================================================================
clang++ test6_map_copy.cpp -o "test6"
./test6
echo -ne "${BLUE}test6_map_copy\r"
if (diff ft.txt std.txt > diff_log) then
	echo -e "test6_map_copy ${CHECK_MARK}"
else
	echo -e "test6_map_copy ${ERROR_MARK}"
	exit 1
fi


rm test1 test2 test3 test4 test5 test6 test7 test8