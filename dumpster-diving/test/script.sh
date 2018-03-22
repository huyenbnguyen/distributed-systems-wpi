#!/bin/bash

# compile time measuring program
gcc measure_time.c -o measure_time


echo "*********************"
echo "Testing ./rm on same partition in milliseconds"
# create files
dd if=/dev/zero of=file1.txt count=10 bs=1024
dd if=/dev/zero of=file2.txt count=100 bs=1024
dd if=/dev/zero of=file3.txt count=1000 bs=1024
dd if=/dev/zero of=file4.txt count=10000 bs=1024
# dd if=/dev/zero of=file5.txt count=5 bs=1024
# dd if=/dev/zero of=file6.txt count=6 bs=1024            
# measure time
./measure_time file1.txt
sync
./measure_time file2.txt
sync
./measure_time file3.txt
sync
./measure_time file4.txt
sync
# ./measure_time file5.txt
# ./measure_time file6.txt

echo "*********************"
echo "Testing ./rm on different partition"
# cd /Volumes/Kindle/documents

