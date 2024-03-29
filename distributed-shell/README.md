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


```
cd distributed-shell/test
./test_script.sh
```
If you want to run the test using a local server, you should change the server IP address on lines 18 and 36. You can also change the command you want to run.

*Note: Since the test script includes uploading files to the Amazon EC2 instance, you want to comment out lines 21 to 33 in ```test/test_script.sh```.*

#### Shutting Down Server
Run ```./client -s 127.0.0.1 -c "exit"```

#### How Server Changes Port 
* The default port number is 1024, however the server can change it dynamically without asking the client by running a for loop from 1024 to 65536.
* If the port number changes, the client *must* specify a port number (e.g. ```./client -s 127.0.0.1 -c "ls -l" -p 1025```