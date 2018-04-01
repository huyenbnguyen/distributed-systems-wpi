#!/bin/bash

# compile time measuring program
gcc measure_time.c -o measure_time

current_path=`pwd`
program_path=$current_path/../

# echo "*********************"
# echo "Starting server"
# $program_path/server &
echo "*********************"
echo "Running empty command 10 times"
for i in {1..2}
do
	echo ""
	echo $i
	./measure_time $program_path/client -s 127.0.0.1 -c "" empty_command.txt
done
echo "Shutting down server..."
./measure_time $program_path/client -s 127.0.0.1 -c "exit" 
