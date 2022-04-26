#include <iostream>
#include <pthread.h>
#include <utility>
#include <vector>
#include "tcp_syn.hh"

#define NUM_THREADS 500

struct thread_data {
    int thread_id;
    std::string host;
    unsigned short port;
};

void* thread(void* threadarg)
{
    struct thread_data* my_data;
    my_data = (struct thread_data*)threadarg;
    int status = tcp_syn(my_data->host, my_data->port);
    std::cout << "port: " << my_data->port << "\tstatus:" << status << std::endl;
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    struct thread_data td[NUM_THREADS];

    std::vector<std::pair<std::string, unsigned short>> data = { { "39.104.55.143", 21 }, { "39.104.55.143", 22 }, { "39.104.55.143", 80 }, { "39.104.55.143", 443 }, { "39.104.55.143", 8888 }, { "39.104.55.143", 1234 } };
    for (int i = 1235; i < 2000; ++i) {
        data.push_back(std::make_pair("39.104.55.143", (unsigned short)i));
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        // std::cout << "main() : creating thread, " << i << std::endl;
        td[i].thread_id = i;
        td[i].host = data[i].first;
        td[i].port = data[i].second;
        int status = pthread_create(&threads[i], NULL, thread, (void*)&td[i]);
        if (status) {
            std::cout << "Error:unable to create thread," << status << std::endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
    return 0;
}

