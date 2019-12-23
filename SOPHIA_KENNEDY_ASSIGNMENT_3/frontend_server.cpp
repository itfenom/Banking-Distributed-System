/*CODE FOR THE FRONTEND SERVER */
/* The program basically get the client request and passes it to the backend server */
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include<iostream>
#include<fstream>
#include<pthread.h>
#include<string>
#include<typeinfo>
pthread_mutex_t lock;
using namespace std;
#define MAX_THREADS 100 
#define PORT 8023

//global declarations
int numberoftransaction=0;
pthread_t thread_id[MAX_THREADS]; 
int new_socket_1[20];
int status_arr[3];
int test[2];

char command1[20]="CREATE";
char command2[20]="QUERY";
char command3[20]="UPDATE";
char command4[20]="QUIT";
char reply1[20]="ERR, acct not found";

//structure to receive the data from the client
struct transaction{
char command[20];
int acctno;
double amount;
char message[20];
};

//structure array object declaration
struct transaction recs[10];

/***************************THREAD FUNCTION***********************************/

//thread function for passing the data to backend server
void *getInfo(void *threadarg){
   
  //basic initialization
  int number=100;
  int sock = *(int*)threadarg;
  struct transaction data;
  int j=0;
  socklen_t len = sizeof (test);


  //Recieves the data from the client
  while((recv(sock,&data,sizeof(data),0)>0)){

/*******************************2 phase commit*****************************************/

//has implemented two phase protocol where the frontend server get the message "OK commit.
//from the backend server which is process to the client.
//here I have considered the "OK commit" as a voting mechanism from three backend servers.
//ERR,acct not found as the vote for abort from the backend servers.

/*************************************CODE FOR CREATE*****************************************/
    //looping to create account for the client by checking the command from the client
     if(strcmp(data.command,command1)==0){
        recs[j].acctno= number;
        data.acctno=number;
        recs[j].amount= data.amount;

        //passes data to the backend
        //Using getsockopt for the purpose of fault tolerance
        for(int k=0;k<3;k++){
          if(status_arr[k]==1)
         {
          send(new_socket_1[k],&data,sizeof(data),0);
          getsockopt(new_socket_1[k],SOL_SOCKET,SO_ERROR,&test[k],&len);
        }
          if(test[k]!=0){
            cout<<"backend server"<<k+1<<"crashed"<<endl;
            status_arr[k]=0;
          }
       }

        
        //receives the data after operation from the backend
        for(int k=0;k<3;k++){
        if(status_arr[k]==1)
        recv(new_socket_1[k],&data,sizeof(data),0);
        cout<<data.message<<'\t'<<data.acctno<<endl;
       }
      recs[j].acctno=data.acctno; 
      recs[j].amount=data.amount;
      
      //send back the transaction data and details to the client
      send(sock,&data,sizeof(data),0);
      cout<<".................................."<<endl;
      j++;
      
      }

      number++;

/****************************FAULT TOLERANCE**************************************/
//Used the mechanism of fault tolerance using getsockopt where i have declared and initialized 
//the status array to 1 when connection is availble and sets to zero if connection is not.
//based on the status array the getsockopt handles the fault in the backend servers



/*****************************CODE FOR QUERYING********************************************/

    //for querying the data in the server from the client
    if(strcmp(data.command,command2)==0){
      for(int i=0;i<(sizeof(recs)/sizeof(recs[i]));i++){
          if(recs[i].acctno==data.acctno){
            for(int k=0;k<3;k++){
              if(status_arr[k]==1){
                send(new_socket_1[k],&data,sizeof(data),0);
                getsockopt(new_socket_1[k],SOL_SOCKET,SO_ERROR,&test[k],&len);
              }

              if(test[k]!=0){
                cout<<"backend server"<<k+1<<"crashed"<<endl;
                status_arr[k]=0;
              }
            }

            for(int k=0;k<3;k++){
              if(status_arr[k]==1)
              recv(new_socket_1[k],&data,sizeof(data),0);
              cout<<data.command<<'\t'<<data.message<<'\t'<<data.amount<<'\t'<<data.acctno<<endl;  
            }
            
            break;
          
          }
          if(recs[i].acctno!=data.acctno){
            strcpy(data.message,reply1);
            
          }
        }
        send(sock,&data,sizeof(data),0);
        cout<<".................................."<<endl;
       
      }
    
/*****************************************CODE FOR UPDATING**********************************/

    //for updating the data in the server from the client
    if(strcmp(data.command,command3)==0){
      for(int i=0;i<(sizeof(recs)/sizeof(recs[i]));i++){
          if(recs[i].acctno==data.acctno){
            for(int k=0;k<3;k++){
              if(status_arr[k]==1){
                send(new_socket_1[k],&data,sizeof(data),0);
                getsockopt(new_socket_1[k],SOL_SOCKET,SO_ERROR,&test[k],&len);
              }
              if(test[k]!=0){
                cout<<"backend server"<<k+1<<"crashed"<<endl;
                status_arr[k]=0;
              }

            }

            for(int k=0;k<3;k++){
              if(status_arr[k]==1)
              recv(new_socket_1[k],&data,sizeof(data),0);
              recs[i].amount=data.amount;
              cout<<data.command<<'\t'<<data.message<<'\t'<<data.amount<<'\t'<<data.acctno<<endl;
              
            }

            break;
          
          }
        
          if(recs[i].acctno!=data.acctno){
            strcpy(data.message,reply1);  
        }
        
        cout<<data.command<<'\t'<<data.message<<'\t'<<data.amount<<'\t'<<data.acctno<<endl;
          
        }
        send(sock,&data,sizeof(data),0);
        cout<<".................................."<<endl;
       
      }

/**************************************CODE FOR QUITTING****************************************/    
    //checking the command to quit
    if(strcmp(data.command,command4)==0){
      cout<<"OK shutting down"<<endl;
      //closing the backend socket connections.
      for(int k=0;k<3;k++){
        close(new_socket_1[k]);
      }

      close(sock);
      break;
          
    }
  

}
  

pthread_exit(NULL);   
}
 
/*.......................END OF THREAD FUNCTION...................................*/

/******************************MAIN FUNCTION*******************************/

int main(int argc, char* argv[]) 
{
 
  int pc;
  int opt=1;

  
  int server_fd, new_socket, valread; 
  struct sockaddr_in address;
  int addrlen = sizeof(address); 
  
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET,  
                          SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 


    //Using the setsockopt to reuse the port address
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
  

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to 
    if (bind(server_fd, (struct sockaddr*)&address,  
                          sizeof(address)) < 0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
  int i=0;
  
  while(i<=1000){ 
    cout<<"**********Server************"<<'\n';
    // puts the server socket in passive mode 
    if (listen(server_fd, 4) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    
    //accepting the client requests 
    if ((new_socket = accept(server_fd, 
                  (struct sockaddr*)&address, 
                  (socklen_t*)&addrlen)) < 0) { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 

    //creating the connection for the backend servers
    for(int k=0;k<3;k++){
       new_socket_1[k] = accept(server_fd, 
                  (struct sockaddr*)&address, 
                  (socklen_t*)&addrlen);
       status_arr[k]=1;
    }
    
    //Checking for mutex lock condition
    if (pthread_mutex_init(&lock, NULL) != 0) 
    { 
        printf("\n mutex init has failed \n"); 
        return 1; 
    } 
   
//creating threads for each and every client
pthread_create(&thread_id[i],NULL,getInfo,(void *)&new_socket);
i++;
    } 

close(server_fd);
   
    return 0; 
} 



