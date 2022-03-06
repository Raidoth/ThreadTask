#include<iostream>
#include<sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<cstring>
#define PORT 7555
#define ALIQUOT 32
#define MORESYMB 2
using namespace std;

class Server
{
private:
    int server_socket_fd, new_socket;
    struct sockaddr_in address;
    int addr_len = sizeof(address);
    int opt = 1;
    char buffer[30];
public:
    Server()
    {
        if((server_socket_fd=socket(AF_INET,SOCK_STREAM,0))==0)
        {
            cout<<"Socket failed.\n";
            exit(EXIT_FAILURE);
        }
        if(setsockopt(server_socket_fd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt)))
        {
            cout<<"Set socket opt failed.\n";
            exit(EXIT_FAILURE);
        }
        address.sin_family=AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        if(bind(server_socket_fd,(struct sockaddr*)&address,sizeof(address))<0)
        {
            cout<<"Bind failed.\n";
            exit(EXIT_FAILURE);
        }
        if(listen(server_socket_fd,3)<0)
        {
            perror("listen\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            cout<<"Listening..\n";
        }

        while(true)
        {
            if ((new_socket = accept(server_socket_fd, (struct sockaddr *)&address,
                                     (socklen_t*)&addr_len))<0)
            {
                cout<<"Accept failed.";
                exit(EXIT_FAILURE);
            }
            else
            {
                memset(buffer,0,sizeof(buffer));
                read(new_socket, buffer, sizeof(buffer));
                cout<<"Client message: "<<buffer<<endl;
                int count_symb = 0;
                for(int i = 0; i<sizeof(buffer); i++)
                {
                    if((int)buffer[i]==0)
                    {
                        break;
                    }
                    count_symb++;
                }
                int reciv_mess = atoi(buffer);
                if(count_symb>=MORESYMB&&reciv_mess%ALIQUOT==0)
                {
                    cout<<"Data success.\n";
                }
                else
                {
                    cout<<"Data error.\n";

                }
                shutdown(new_socket, SHUT_RDWR);
                close(new_socket);

            }
        }
    }

};


int main()
{
    Server s;


}
