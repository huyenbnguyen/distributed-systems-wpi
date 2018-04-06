#!/bin/bash

# compile time measuring program
gcc measure_time.c -o measure_time

current_path=`pwd`
program_path=$current_path/../

rm empty_command.txt
rm transfer_file.txt

echo "*********************"
echo "Measure the amount of time required (the latency, in milliseconds) to setup a connection to the server, authenticate, and tear it down: Running empty command 10 times"
for i in {1..10}
do
	echo ""
	echo $i
	./measure_time $program_path/client -s 54.149.136.240 -c "" empty_command.txt
done

echo "Measure the maximum throughput (in bits per second) from the server to the client"
for i in {1..10}
do
	echo $i
	echo "Creating file"
	dd if=/dev/zero of=file$i.txt count=$i bs=4 
	echo "Copying to server"
	cd ..
	scp -i distributed-systems-wpi.pem test/file.txt ubuntu@ec2-54-149-136-240.us-west-2.compute.amazonaws.com:/home/ubuntu/distributed-systems-wpi/distributed-shell
	echo "Measure time"
	cd test
	./measure_time $program_path/client -s 54.149.136.240 -c "cat file$i.txt" transfer_file.txt
done

echo "Shutting down server..."
$program_path/client -s 54.149.136.240 -c "exit" 
