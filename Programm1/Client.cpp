#include<iostream>
#include<thread>
#include<algorithm>
#include <mutex>
#include <condition_variable>
#include<sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7555
using namespace std;

class Client
{
private:
    mutex mtx;
    condition_variable mtx_Bell;
    string buffer;
    string input;
    bool not_all_digit = false;
    int sock = 0;
    struct sockaddr_in serv_addr;
    string mess;
public:

    void threads1()
    {
        while (true)
        {
            cin>>input;
            if(input.length()>64)
            {
                cout<<"Overflow input\nTry again."<<endl;
                continue;
            }
            for(size_t i = 0; i<input.length(); i++)
            {
                if(!isdigit(input[i]))
                {
                    not_all_digit=true;
                }
            }
            if(not_all_digit)
            {
                cout<<"Not all digit\nTry again."<<endl;
                not_all_digit=false;
                continue;
            }
            sort(input.begin(),input.end(),std::greater<char>());
            string tmp;
            for(size_t i = 0; i<input.length(); i++)
            {

                if(input[i]%2==0)
                {
                    tmp+='K';
                    tmp+='B';
                    continue;
                }
                tmp+=input[i];
            }
            buffer = tmp;

            mtx_Bell.notify_one();
        }

    }

    void threads2()
    {

        while(true)
        {
            unique_lock<mutex> ul(mtx);
            int sum=0;
            mtx_Bell.wait(ul,[this]()
            {
                return !buffer.empty();
            });
            cout<<"Data buffer: "<<buffer<<endl;
            for(size_t i = 0; i<buffer.length(); i++)
            {
                if(isdigit(buffer[i]))
                {
                    sum+=buffer[i]-'0';
                }
            }
            cout<<"Buffer sum: "<<sum<<endl;
            buffer.clear();

            mess = to_string(sum);
            if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
            {
                cout<<"Socket error. \n";
            }
            serv_addr.sin_family=AF_INET;
            serv_addr.sin_port = htons(PORT);

            if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0)
            {
                cout<<"Invalid address.\n";
            }
            if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
            {
                cout<<"Server offline.\nWait.\n";
                close(sock);
                tryReconnect();
            }
            send(sock,mess.c_str(),mess.length(),0);
            cout<<"Message send. \n";
            shutdown(sock, SHUT_WR);
            close(sock);
        }

    }
    void tryReconnect()
    {
        if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
        {
        }
        else
        {
        }
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_port = htons(PORT);

        if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0)
        {
            cout<<"Invalid address.\n";
        }
        while(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
        {

        }
    }
};

int main()
{
    Client cl;
    thread t1(&Client::threads1,ref(cl));
    thread t2(&Client::threads2,ref(cl));
    t1.join();
    t2.join();

}
