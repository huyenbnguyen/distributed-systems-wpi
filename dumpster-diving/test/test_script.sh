#!/bin/bash


# change these before you run the script
DUMPSTER="/Users/huyennguyen/Documents/Documents/distributed-systems-wpi/dumpster-diving/dumpster"
PARTITION="/Volumes/Kindle/documents"


export DUMPSTER
export PARTITION

# compile time measuring program
gcc measure_time.c -o measure_time

current_path=`pwd`
rm_script_path=$current_path/../rm

echo "*********************"
echo "Testing ./rm on SAME partition in milliseconds for files"
# create files
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=10 bs=64
dd if=/dev/zero of=file3.txt count=100 bs=64
dd if=/dev/zero of=file4.txt count=1000 bs=64
dd if=/dev/zero of=file5.txt count=10000 bs=64
dd if=/dev/zero of=file6.txt count=100000 bs=64
dd if=/dev/zero of=file7.txt count=1000000 bs=64   

# measure time
./measure_time $rm_script_path file1.txt
./measure_time $rm_script_path file2.txt
./measure_time $rm_script_path file3.txt
./measure_time $rm_script_path file4.txt
./measure_time $rm_script_path file5.txt
./measure_time $rm_script_path file6.txt
./measure_time $rm_script_path file7.txt

echo "*********************"
echo "Testing ./rm on DIFFERENT partition"
cd $PARTITION
echo "Testing ./rm on DIFFERENT partition in milliseconds for an empty directory"
mkdir empty
$current_path/measure_time $rm_script_path -r empty
echo "Testing ./rm on DIFFERENT partition in milliseconds for files"
# create files
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=10 bs=64
dd if=/dev/zero of=file3.txt count=100 bs=64
dd if=/dev/zero of=file4.txt count=1000 bs=64
dd if=/dev/zero of=file5.txt count=10000 bs=64
dd if=/dev/zero of=file6.txt count=100000 bs=64
dd if=/dev/zero of=file7.txt count=1000000 bs=64   
          
# measure time
$current_path/measure_time $rm_script_path file1.txt
$current_path/measure_time $rm_script_path file2.txt
$current_path/measure_time $rm_script_path file3.txt
$current_path/measure_time $rm_script_path file4.txt
$current_path/measure_time $rm_script_path file5.txt
$current_path/measure_time $rm_script_path file6.txt
$current_path/measure_time $rm_script_path file7.txt

echo "Testing ./rm on DIFFERENT partition in milliseconds for LARGE directory"
mkdir large
mkdir large/sublarge1
cd large/sublarge1
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=1 bs=64
dd if=/dev/zero of=file3.txt count=1 bs=64
dd if=/dev/zero of=file4.txt count=1 bs=64
dd if=/dev/zero of=file5.txt count=1 bs=64
dd if=/dev/zero of=file6.txt count=1 bs=64
dd if=/dev/zero of=file7.txt count=1 bs=64
dd if=/dev/zero of=file8.txt count=1 bs=64
dd if=/dev/zero of=file9.txt count=1 bs=64
dd if=/dev/zero of=file10.txt count=1 bs=64
dd if=/dev/zero of=file11.txt count=1 bs=64
cd ..
mkdir sublarge2
cd sublarge2
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=1 bs=64
dd if=/dev/zero of=file3.txt count=1 bs=64
dd if=/dev/zero of=file4.txt count=1 bs=64
dd if=/dev/zero of=file5.txt count=1 bs=64
dd if=/dev/zero of=file6.txt count=1 bs=64
dd if=/dev/zero of=file7.txt count=1 bs=64
dd if=/dev/zero of=file8.txt count=1 bs=64
dd if=/dev/zero of=file9.txt count=1 bs=64
dd if=/dev/zero of=file10.txt count=1 bs=64
dd if=/dev/zero of=file11.txt count=1 bs=64
cd ..
mkdir sublarge3
cd sublarge3
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=1 bs=64
dd if=/dev/zero of=file3.txt count=1 bs=64
dd if=/dev/zero of=file4.txt count=1 bs=64
dd if=/dev/zero of=file5.txt count=1 bs=64
dd if=/dev/zero of=file6.txt count=1 bs=64
dd if=/dev/zero of=file7.txt count=1 bs=64
dd if=/dev/zero of=file8.txt count=1 bs=64
dd if=/dev/zero of=file9.txt count=1 bs=64
dd if=/dev/zero of=file10.txt count=1 bs=64
dd if=/dev/zero of=file11.txt count=1 bs=64
cd ..
mkdir sublarge4
cd sublarge4
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=1 bs=64
dd if=/dev/zero of=file3.txt count=1 bs=64
dd if=/dev/zero of=file4.txt count=1 bs=64
dd if=/dev/zero of=file5.txt count=1 bs=64
dd if=/dev/zero of=file6.txt count=1 bs=64
dd if=/dev/zero of=file7.txt count=1 bs=64
dd if=/dev/zero of=file8.txt count=1 bs=64
dd if=/dev/zero of=file9.txt count=1 bs=64
dd if=/dev/zero of=file10.txt count=1 bs=64
dd if=/dev/zero of=file11.txt count=1 bs=64
cd ..
mkdir sublarge5
cd sublarge5
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=1 bs=64
dd if=/dev/zero of=file3.txt count=1 bs=64
dd if=/dev/zero of=file4.txt count=1 bs=64
dd if=/dev/zero of=file5.txt count=1 bs=64
dd if=/dev/zero of=file6.txt count=1 bs=64
dd if=/dev/zero of=file7.txt count=1 bs=64
dd if=/dev/zero of=file8.txt count=1 bs=64
dd if=/dev/zero of=file9.txt count=1 bs=64
dd if=/dev/zero of=file10.txt count=1 bs=64
dd if=/dev/zero of=file11.txt count=1 bs=64
cd ..
mkdir sublarge6
cd sublarge6
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=1 bs=64
dd if=/dev/zero of=file3.txt count=1 bs=64
dd if=/dev/zero of=file4.txt count=1 bs=64
dd if=/dev/zero of=file5.txt count=1 bs=64
dd if=/dev/zero of=file6.txt count=1 bs=64
dd if=/dev/zero of=file7.txt count=1 bs=64
dd if=/dev/zero of=file8.txt count=1 bs=64
dd if=/dev/zero of=file9.txt count=1 bs=64
dd if=/dev/zero of=file10.txt count=1 bs=64
dd if=/dev/zero of=file11.txt count=1 bs=64
cd ..
mkdir sublarge7
cd sublarge7
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=1 bs=64
dd if=/dev/zero of=file3.txt count=1 bs=64
dd if=/dev/zero of=file4.txt count=1 bs=64
dd if=/dev/zero of=file5.txt count=1 bs=64
dd if=/dev/zero of=file6.txt count=1 bs=64
dd if=/dev/zero of=file7.txt count=1 bs=64
dd if=/dev/zero of=file8.txt count=1 bs=64
dd if=/dev/zero of=file9.txt count=1 bs=64
dd if=/dev/zero of=file10.txt count=1 bs=64
dd if=/dev/zero of=file11.txt count=1 bs=64
cd ..
mkdir sublarge8
cd sublarge8
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=1 bs=64
dd if=/dev/zero of=file3.txt count=1 bs=64
dd if=/dev/zero of=file4.txt count=1 bs=64
dd if=/dev/zero of=file5.txt count=1 bs=64
dd if=/dev/zero of=file6.txt count=1 bs=64
dd if=/dev/zero of=file7.txt count=1 bs=64
dd if=/dev/zero of=file8.txt count=1 bs=64
dd if=/dev/zero of=file9.txt count=1 bs=64
dd if=/dev/zero of=file10.txt count=1 bs=64
dd if=/dev/zero of=file11.txt count=1 bs=64
cd ..
mkdir sublarge9
cd sublarge9
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=1 bs=64
dd if=/dev/zero of=file3.txt count=1 bs=64
dd if=/dev/zero of=file4.txt count=1 bs=64
dd if=/dev/zero of=file5.txt count=1 bs=64
dd if=/dev/zero of=file6.txt count=1 bs=64
dd if=/dev/zero of=file7.txt count=1 bs=64
dd if=/dev/zero of=file8.txt count=1 bs=64
dd if=/dev/zero of=file9.txt count=1 bs=64
dd if=/dev/zero of=file10.txt cosunt=1 bs=64
dd if=/dev/zero of=file11.txt count=1 bs=64
cd ..
mkdir sublarge10
cd sublarge10
dd if=/dev/zero of=file1.txt count=1 bs=64
dd if=/dev/zero of=file2.txt count=1 bs=64
dd if=/dev/zero of=file3.txt count=1 bs=64
dd if=/dev/zero of=file4.txt count=1 bs=64
dd if=/dev/zero of=file5.txt count=1 bs=64
dd if=/dev/zero of=file6.txt count=1 bs=64
dd if=/dev/zero of=file7.txt count=1 bs=64
dd if=/dev/zero of=file8.txt count=1 bs=64
dd if=/dev/zero of=file9.txt count=1 bs=64
dd if=/dev/zero of=file10.txt count=1 bs=64
dd if=/dev/zero of=file11.txt count=1 bs=64
cd ../..

$current_path/measure_time $rm_script_path -r large