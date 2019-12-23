/*Code for the back end server */
#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
#include<chrono>
#include<random>
#include<time.h>
#include<cstdio>
#include<fstream>
#include<cstdlib>
#include<string.h>  
#include<unistd.h>
#include<cstdio> 
using namespace std;
#define PORT 8023
/****************************INITIALIZATION***********************************/

//structure to store the client data
struct server_transaction{
  char command[20];
  int acctno;
  double amount;
  char message[20];
}temp;


//global initialization and declaration
struct server_transaction recs[10];

char command1[20]="CREATE";
char command2[20]="UPDATE";
char command3[20]="QUERY";
char reply1[20]="OK Commit";
char reply2[20]="ERR,acct not found";


/***************************MAIN FUNCTION*************************************/
int main(int argc,char** argv)
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
  
   
   

   //connect the socket 
   connect(sock, (struct sockaddr*)&serv_addr, 
                sizeof(serv_addr));


  int j=0;
  while(recv(sock,&temp,sizeof(temp),0)){

    cout<<"BACKEND SERVER"<<endl;


/********************CREATING ACCOUNT********************************************/

  //code for create command , checks the command and 
  //assigns an account number then stores it in the array
  //sends it back to the frontend server
  if(strcmp(temp.command,command1)==0){ 
    recs[j].acctno=temp.acctno;
    recs[j].amount=temp.amount;
    cout<<"OK Commit"<<'\t'<<recs[j].acctno<<'\t'<<recs[j].amount<<endl;
    cout<<".................................."<<endl;
    strcpy(temp.message,reply1);
    send(sock,&temp,sizeof(temp),0);
        
  } 

/****************************UPDATING***************************************/

  //check the command for update
  //assigns the amount and sends it back to the frontend server
  if(strcmp(temp.command,command2)==0){   
    for(int i=0;i<sizeof(recs);i++){
      

      //checks if the acct no matches
      if(recs[i].acctno==temp.acctno){
        recs[i].acctno=temp.acctno;
        recs[i].amount=temp.amount;
        cout<<"OK commit"<<'\t'<<temp.acctno<<'\t'<<temp.amount<<endl;
        cout<<".................................."<<endl;
        strcpy(temp.message,reply1);
        //send(sock,&temp,sizeof(temp),0);
        break;
      }

      else{
        strcpy(temp.message,reply2);
      }
    }
    send(sock,&temp,sizeof(temp),0);

    
    
  }

/**********************************QUERYING*********************************/
//check the command for query
//send the result to the front end server
if(strcmp(temp.command,command3)==0){
  for(int i=0;i<sizeof(recs);i++){
    //verifies the account number
    if(recs[i].acctno==temp.acctno){
      temp.amount=recs[i].amount;
      cout<<"OK Commit"<<endl;
      cout<<".................................."<<endl;
      strcpy(temp.message,reply1);
      send(sock,&temp,sizeof(temp),0);
      break;
    }

    else{
      strcpy(temp.message,reply2);
    }

  }
  

}

j++;

}


}    
 




 





