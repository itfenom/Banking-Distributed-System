#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<iostream>
#include <chrono>
#include<random>
#include <time.h>
#include<cstdio>
#include<fstream>
#include<cstdlib>
#include <string.h>  
#include <unistd.h>
#include<cstdio> 
using namespace std;



//structure to store the receiving input data and 
//pass it onto the server for transaction
struct transaction{
char command[20];
int acctno;
double amount;
char message[20];
}temp;


//Main function
int main(int argc,char** argv)
{

  struct transaction trans;

  int sock = 0, valread; 
  char recvbuffer[1024];
  struct sockaddr_in serv_addr; 
  int port= atoi(argv[1]);
  const char *ip_addr=argv[2];

  //initializing commands for comparison
  char command1[20]="CREATE";
  char command2[20]="QUERY";
  char command3[20]="QUIT";
  char command4[20]="UPDATE";

  
    // Creating socket file descriptor 
    if ((sock = socket(AF_INET, 
                       SOCK_STREAM, 0)) 
        < 0) { 
      cout<<"\n Socket creation error \n"; 
        return -1; 
    } 
  
    //memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
    serv_addr.sin_addr.s_addr=INADDR_ANY;
  
    // Convert IPv4 and IPv6 addresses from 
    // text to binary form 127.0.0.1 is local 
    // host IP address, this address should be 
    // your system local host IP address 
    if (inet_pton(AF_INET, ip_addr, 
                  &serv_addr.sin_addr) 
        <= 0) { 
      cout<<"\nAddress not supported \n"; 
        return -1; 
    } 
  
   //connect the socket 
   connect(sock, (struct sockaddr*)&serv_addr, 
                sizeof(serv_addr));

    
   
    
  while(1){
    cout<<"MAIN CLIENT"<<endl;
    //Getting the command i.e CREATE / UPDATE / QUIT /QUERY
    cout<<"Enter a command"<<endl;
    cin>>temp.command;

    //conversion of lowercase strings to uppercase for the string command
    for(int i=0;i<sizeof(temp.command);i++){
       if(temp.command[i]>=97 && temp.command[i]<=122)
      {
        temp.command[i]=temp.command[i]-32;
      }
    }

    //checks for the command update and gets the required input
    if(strcmp(temp.command,command4)==0){
    cout<<"enter acctno"<<endl;
    cin>>temp.acctno;
    cout<<"enter amount"<<endl; 
    cin>>temp.amount;  

    

    //Client sends the query to the servers
    send(sock,&temp, sizeof(temp),0);
    recv(sock,&trans,sizeof(trans),0);
    cout<<trans.command<<'\t'<<trans.message<< '\t'<<trans.acctno<<'\t'<<trans.amount<<endl;
    cout<<"****************************************"<<endl;
   
    }

    //Checks the command "QUERY"
    if(strcmp(temp.command,command2)==0){
    cout<<"enter acct no"<<endl; 
    cin>>temp.acctno; 
    
    //Client sends the query to the servers
    send(sock,&temp, sizeof(temp),0);
    recv(sock,&trans,sizeof(trans),0);
   cout<<trans.command<<'\t'<<trans.message<< '\t'<<trans.acctno<<'\t'<<trans.amount<<endl;
    cout<<"****************************************"<<endl;
   
    }
    //checks for the command "CREATE"
    if(strcmp(temp.command,command1)==0){
      cout<<"Enter the amount"<<endl;
      cin>>temp.amount;
      cout<<temp.command<<'\t'<<temp.amount<< '\t'<<temp.acctno<<endl;

      //Client sends the query to the servers
      send(sock,&temp, sizeof(temp),0);
      recv(sock,&trans,sizeof(trans),0);
      cout<<trans.command<<'\t'<<trans.message<< '\t'<<trans.acctno<<'\t'<<trans.amount<<endl;
      cout<<"****************************************"<<endl;
   
    }

    //checks for the command "QUIT"
    if(strcmp(temp.command,command3)==0){
      send(sock,&temp, sizeof(temp),0);
      break;
    }
 }  
}



 





