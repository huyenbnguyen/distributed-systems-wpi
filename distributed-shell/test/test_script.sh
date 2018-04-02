#!/bin/bash

# compile time measuring program
gcc measure_time.c -o measure_time

current_path=`pwd`
program_path=$current_path/../

rm empty_command.txt

echo "*********************"
echo "Running empty command 10 times"
for i in {1..2}
do
	echo ""
	echo $i
	./measure_time $program_path/client -s 54.149.136.240 -c "" empty_command.txt
done
echo "Shutting down server..."
$program_path/client -s 54.149.136.240 -c "exit" 
