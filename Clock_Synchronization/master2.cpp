#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include<cstdlib>
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include<iostream>
#include<time.h>
#include<fstream>
#include<pthread.h>
using namespace std;
#define MAX_THREADS 100 
#define PORT 8090




int main(int argc, char* argv[]) 
{
 
  int pc;
  int mc;
  int valread1;
  char sendbuff[1024];
  //generating random number clock for the master
  srand(time(0)); 
  int master_clock=rand()%100;
  
  cout<<"master_clock initial:"<<master_clock<<endl;
  
  int server_fd, new_socket[100], valread,new_socket1; 
  struct sockaddr_in address;
  int addrlen = sizeof(address); 
  
 
 
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET,  
                          SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

  
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT);

   
  
    // Forcefully attaching socket to 
    // the port 8090 
    if (bind(server_fd, (struct sockaddr*)&address,  
                          sizeof(address)) < 0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    //For command line arguments
    int nop;
    if (argc > 1)
        nop = atoi(argv[1]);
    int cc=0;
    

    cout<<"**********Client************"<<'\n';
    // puts the server socket in passive mode 
    if (listen(server_fd, 4) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    
    int clock_1;
    char buffer[250]={0}; 
    string mes="Send the time of your clocks "; 
    char msg[100];
    int num=1;
    int clo[100];

   for(int i=0;i<nop;i++){
    //accepting the client request
    new_socket[i] = accept(server_fd, 
                  (struct sockaddr*)&address, 
                  (socklen_t*)&addrlen) ;
  
}

   for(int i=0;i<nop;i++){
    //sending the initial message to send the time 
    send(new_socket[i], mes.c_str(), sizeof(mes),0); 
    //receives the time from all the clients.
    recv(new_socket[i],&clock_1,sizeof(clock_1),0);
    clo[i]=clock_1;
    cout<<"client"<<clock_1<<endl;
  } 


   
    for(int i=0;i<nop;i++){
    //calculation of berkeley algorithm
    cc=cc+clo[i];
    mc= (cc+master_clock)/(i+2);

    }
   cout<<"master clock:\t"<<mc<<endl;

   //Sending the calculated time to all the clients.
   for(int i=0;i<nop;i++){
   send(new_socket[i],&mc,sizeof(mc),0);
  }
    
    return 0; 
} 



