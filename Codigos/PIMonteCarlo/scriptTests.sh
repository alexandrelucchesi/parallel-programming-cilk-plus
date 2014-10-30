#!/bin/bash
#
#  Compile the program with GCC.
#
echo "Start"
g++ -fopenmp -O1 -o cilkPI cilkPIMonteCarlo.cpp
g++ -fopenmp -O1 -o openmpPI openmpPIMonteCarlo.cpp
elements=1000000000
echo "$elements elements"
for i in {1..5}
do
	echo ""
	echo "$i time"
	echo "threads openmp cilk" > time/times-$i.txt
	for j in {1..5}
	do
		echo "With $j threads or workers"
		echo -n "$j " >> time/times-$i.txt
		tempo=`./openmpPI $j $elements`
		echo "openmp $tempo"
		echo -n " " >> time/times-$i.txt
		echo -n "$tempo " >> time/times-$i.txt
		tempo=`./cilkPI $j $elements`
		echo "cilk $tempo"
                echo -n " " >> time/times-$i.txt
                echo -n "$tempo " >> time/times-$i.txt
		echo "" >> time/times-$i.txt
	done
done
