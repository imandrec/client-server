#include <unistd.h>
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include "fileStruct.h"


ClientInfo *client[100];
int clientIndex = 0;
const int MaxSize = 2000;
struct sockaddr_in server_addr , client_addr;         //SERVER ADDR will have all the server address
int* findFile(int fd){
        cout<<"\nFile present at these client : \n";
        cout<<"Client#           ID";
        int *clientID = new int [100];
        int i = 0;
        int index_ = 0;
        for(; i< clientIndex; i++){
                if(client[i]->fileVector[fd] == '1'){
                        clientID[index_] = client[i]->id;
                        index_++;
                        cout<<"\n["<<i<<"] \t " << client[i]->id;
                }
        }
        clientID[index_] = INT32_MAX;
        return clientID;
}

void* ThreadConectionHandeler(void* parameters){
        int client_sock = *((int*)parameters);
        
        bool isRead = 0;
        char server_message[MaxSize ], client_message[MaxSize ];                 // Sending values from the server and receive from the server we need this
        //Cleaning the Buffers
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));     // Set all bits of the padding field//

        //Receive the message from the client
        ClientInfo *client_info = new ClientInfo;
        if (recv(client_sock, client_info, sizeof(ClientInfo),0) < 0) {
                cout<<"Receive Failed. Error!!!!!\n";
                return NULL;
        }
        cout<<"\nClient ID : "<<client_info->id;
        cout<<"\nCLient IP : "<< inet_ntoa(client_addr.sin_addr);
        cout<<"\nClient Port NUmber : "<<client_info->port<<endl;
        client[clientIndex] = client_info; clientIndex++;

        while(1){
                int *temp;
                if(client_message[0] != '\0' && strcmp(client_message,"I wishes to quit")==0){
                        cout<<"\nClient ID : "<<client_info->id<<" at IP : "<< inet_ntoa(client_addr.sin_addr)<<" Wishes to quit"<<endl;
                        char serverRely[6] = "quit";
                        if (send(client_sock, serverRely, 7 ,0)<0) {
                                cout<<"Send Failed. Error!!!!!\n";
                                return NULL;
                        }
                        
                        break;
                }
                else{
                        if(isRead){
                                cout<<"\nCan you tell me who has file " << client_message;
                        }
                }
                
                if (recv(client_sock, client_message, 100,0) < 0)
                {
                        cout<<"Receive Failed. Error!!!!!\n";
                        return NULL;
                }
                temp = findFile(atoi(client_message));
                isRead = true;
                
                if (send(client_sock, temp, 100 ,0)<0) {
                        cout<<"Send Failed. Error!!!!!\n";
                        return NULL;
                }
                
        }
        //Closing the Socket
        
}


int main(int argc, char *argv[])
{

        int socket_desc, client_sock;
        socklen_t client_size; 
       
        char server_message[MaxSize ], client_message[MaxSize ];                 // Sending values from the server and receive from the server we need this
        //Cleaning the Buffers
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));     // Set all bits of the padding field//
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                cout<<"Could Not Create Socket. Error!!!!!\n";
                return -1;
        }
        cout<<"Socket Created\n";
        //Binding IP and Port to socket
        server_addr.sin_family = AF_INET;               /* Address family = Internet */
        server_addr.sin_port = htons(atoi(argv[1]));               // Set port number, using htons function to use proper byte order */
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");    /* Set IP address to localhost */
        
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)    // Bind the address struct to the socket.  /
                                        //bind() passes file descriptor, the address structure,and the length of the address structure
        {
                cout<<"Bind Failed. Error!!!!!\n";
                return -1;
        }        
        cout<<"Bind Done\n";
        while(1){ 
                //Put the socket into Listening State        
                if(listen(socket_desc, 1) < 0)                               //This listen() call tells the socket to listen to the incoming connections.
                {
                        cout<<"Listening Failed. Error!!!!!\n";
                        return -1;
                }
                 
                //Accept the incoming Connections
                
                client_size = sizeof(client_addr);        
                client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);          // heree particular client k liye new socket create kr rhaa ha
                
                if (client_sock < 0)
                {
                        cout<<"Accept Failed. Error!!!!!!\n";
                        return -1;
                }
                cout<<"Client Connected with IP: "<<  inet_ntoa(client_addr.sin_addr);
                cout<<" and Port No: "<<ntohs(client_addr.sin_port);

                pthread_t pid;
                if (pthread_create(&pid, NULL, ThreadConectionHandeler, (void*)&client_sock)!=0)
                        cout<<"Error occur during the creating of error";
                pthread_detach(pid);
                sleep(2);
                cout<<"Listening for Incoming Connections.....\n";
        }
        return 0;       
}
