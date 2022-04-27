#include<iostream>
#include<vector>
using namespace std;
struct ClientInfo{
        int id;
        int port;
        char fileVector[64];
        ClientInfo(){
         
        }
        void print(){
                cout<<"\nID : "<<id;
                cout<<"\nPort : "<<port;
        }
        void printData(){
                for(int i = 0; i< 64; i++){
                        cout<<"\n["<<i<<"] : "<<fileVector[i];
                }
        }
};