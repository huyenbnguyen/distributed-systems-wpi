mention:
- max socket num
- socket change mechanism
- how to exit server


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

#### Prerequisites
* Ubuntu 16.04 LTS
* gcc

#### Installing

Compile the files

```
cd distributed-shell
make 
```

Run ```./server``` to start the server 

Run ```./server -h``` for help menu  


Run ```./client -s 127.0.0.1 -c "ls -l"``` run the command ```ls -l``` on the local server    

Run ```./client -h``` for help menu   

#### Running the tests
*Note: Since the test script includes uploading files to the Amazon EC2 instance, you want to comment out lines 21 to 33 in ```test/test_script.sh```.*

```
cd distributed-shell/test
./test_script.sh
```

#### Shutting Down Server
Run ```./client -s 127.0.0.1 -c "exit"```

#### How Server Changes Port 
* The server tries to bind socket numbers from 1024 to 65536 using a for loop
* If the port number changes, the client *must* specify a port number (e.g. ```./client -s 127.0.0.1 -c "ls -l" -p 1025```