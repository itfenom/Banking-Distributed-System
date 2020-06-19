#Distibuted banking service 
Project descrition:
Implemented a distributed banking service that permits multiple concurrent operations and exhibits
simple fault tolerance. For this project, three backend servers and one frontend server has been
implemented where two phase commit protocol is used for checking the consistency among the three
backend servers. The fault tolerance is also implemented by creating the replicas of each and every
transaction data in all the three servers. The entire project was implemented using C++ as the
programming language in the Linux operating system.



Execution of the code:
Execute the code in Linux environment.Below are the commands
Make compile
./frontend_server
./main_client 8023 "127.0.0.1"
Python3 backend_pycode.py 3

where ,
8023 – port number assigned to frontend and the backend servers
127.0.0.1 – is the ip address
3- to signify the number of backend process to be executed.
