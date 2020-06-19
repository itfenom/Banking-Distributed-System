#include<stdio.h>
#include<sys/socket.h>
#include<string.h>//memset
#include<stdlib.h>//sizeof
#include<netinet/in.h>//INADDR_ANY
#include <arpa/inet.h>
#include<iostream>
#include <chrono>
#include<random>
#include <time.h>
#include<fstream>
#include<cstdlib>
#include <string.h>  
#include <unistd.h> 
using namespace std;
#define PORT 8090 


int main()
{


  int sock = 0, valread; 
  char recvbuffer[1024];
  struct sockaddr_in serv_addr; 
   
  
    // Creating socket file descriptor 
    if ((sock = socket(AF_INET, 
                       SOCK_STREAM, 0)) 
        < 0) { 
      cout<<"\n Socket creation error \n"; 
        return -1; 
    } 
  
    //memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    serv_addr.sin_addr.s_addr=INADDR_ANY;
  
    // Convert IPv4 and IPv6 addresses from 
    // text to binary form 127.0.0.1 is local 
    // host IP address, this address should be 
    // your system local host IP address 
    if (inet_pton(AF_INET, "127.0.0.1", 
                  &serv_addr.sin_addr) 
        <= 0) { 
      cout<<"\nAddress not supported \n"; 
        return -1; 
    } 
  
   // connect the socket 
   connect(sock, (struct sockaddr*)&serv_addr, 
                sizeof(serv_addr));

    //randomly generating a clock counter using rand()
    srand(time(0)); 
    int clock_1=rand()%100;
    int clock_2;
    char buffer[256]={0}; // 1
    char mes[100];

    //Client/Slave process receiving the initial message from the server
    recv(sock, mes, sizeof(mes), 0);
    cout<<mes<<endl;

    //Client sends its own time
    send(sock,&clock_1,sizeof(clock_1),0);

    //The server returns back the calculated time
    recv(sock,&clock_2,sizeof(clock_2),0);
    cout<<"received time:"<<clock_2<<endl;
    
   




 return 0;
}





