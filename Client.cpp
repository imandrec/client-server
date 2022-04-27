#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <iostream>
#include <sstream>
#include "fileStruct.h"
#include<fstream>
using namespace std;

const int MaxSize = 2000;


void discart(ifstream& fin) { // this function will discart the data untill "=" sign
	char c = ' ';
	while (c != '=') {
		fin >> c;
	}
}

int readFile(string fileName , ClientInfo &client) {
	char fd[100];
	int id;
	int serverPort;
	int MYPORT;
	ifstream fin;
	fin.open(fileName);
	if (fin.is_open()) {
		discart(fin); //We have to discart "FILE_VECTOR"
		fin >> fd; // read file data 
		discart(fin); //We have to discart "CLIENTID"
		fin >> id;
		discart(fin); //We have to discart "Server_port"
		fin >> serverPort;
		discart(fin); //We have to discart "MYPORT"
		fin >> MYPORT;

	}
	else {
		cout << "File not open :-(";
		return -1;
	}
        strcpy(client.fileVector,fd);
        client.id = id;
        client.port = MYPORT;
        strcpy(client.fileVector,fd);
	return serverPort;
}

bool isFilePresent(ClientInfo &client, int index){
        if(client.fileVector[index] == '1'){
                cout<<"\nI already have file "<<index;
                return true;
        }
        return false;
}

int main(int argc, char *argv[])
{
        int socket_desc;
        struct sockaddr_in server_addr;
        char server_message[MaxSize], client_message[MaxSize];
        
        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                cout<<"Could Not Create Socket. Error!!!!!\n";
                return -1;
        }
        cout<<"Socket Created\n";
        
        //Specifying the IP and Port of the server to connect
        ClientInfo client_info;
        int serverPort = readFile(argv[2],client_info);
        if(serverPort<-1) return -1;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(serverPort);
        server_addr.sin_addr.s_addr = inet_addr(argv[1]);
        
        //Now connecting to the server accept() using connect() from client side
        
        if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        {
                cout<<"Connection Failed. Error!!!!!";
                return -1;
        }
        cout<<"Connected\n";
        
        //prepare data for the client
        
        //send client information data toward server
        if(send(socket_desc, &client_info, sizeof(ClientInfo) ,0) < 0)
        {
                cout<<"Send Failed. Error!!!!\n";
                return -1;
        }
        
        char input;
        client_info.printData();
        while(1){
                
                cout<<"\n> ";
                cin>>input;
                if(input =='f' || input=='F'){
                        int index;
                        cout<<"File index that you swants : ";
                        cin>>index;
                        if(!isFilePresent(client_info,index)){
                                //Now send index;
                                stringstream ss;
                                string str;
                                ss<<index;
                                ss>>str;
                                if(send(socket_desc, str.c_str(), 50,0) < 0) {
                                        cout<<"Send Failed. Error!!!!\n";
                                        return -1;
                                }
                                int IdArray[100];
                                if(recv(socket_desc, IdArray, 100, 0) < 0)
                                {
                                        cout<<"Receive Failed. Error!!!!!\n";
                                        return -1;
                                }
                                cout<<"\nFile present in these client data : ";
                                for(int i = 0; IdArray[i] !=INT32_MAX; i++){
                                        cout<<endl<<IdArray[i];
                                }
                        }
                }
                else if(input == 'q' || input =='Q'){
                        char message[100] = "I wishes to quit";
                        if(send(socket_desc, message, 50 ,0) < 0) {
                                cout<<"Send Failed. Error!!!!\n";
                                return -1;
                        }
                        char received[100] = "";
                        if(recv(socket_desc, received, 100, 0) < 0)
                        {      
                                cout<<"Receive Failed. Error!!!!!\n";
                                return -1;
                        }
                        break;
                }
        }
  
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Closing the Socket
        
        close(socket_desc);
        
        return 0;
}