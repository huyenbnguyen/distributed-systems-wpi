# Distributed Systems WPI

This repo contains the projects for Distributed Systems course at WPI.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
Set the DUMPSTER environment variable.

```
DUMPSTER="/Users/huyennguyen/Documents/"
export DUMPSTER
``` 

### Installing

A step by step series of examples that tell you have to get a development env running

Compile the files

```
cd dumpster-diving
make 
```

Run ```./rm``` to move files to the dumpster

```
./rm file1.txt 
```
Run ```./dv``` to move files from the dumpster to the current working directory
```
./dv file1.txt
```
Run ```./dump``` to remove files from the dumpster permanently
## Running the tests

```
cd dumpster-diving/test
./test_script.sh
```

### Break down into end to end tests

These tests measures the time taken to move files within the same and across different partitions.  
*Note: you will want to change environment variables DUMPSTER and PARTITION at the top of test_script.sh before running it. DUMPSTER is the path to the dumpster. PARTITION is the path of the other partition. Also, you want to be in the test directory.*
See ```dumpster-diving/dumpster-diving-report.pdf``` for more details.an example