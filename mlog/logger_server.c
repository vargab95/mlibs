#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <threads.h>

#include "data.h"

#define MLOG_USE_UDP

typedef struct {
	int id;
	thrd_t thread;
} mlog_worker_thread_info;

mlog_worker_thread_info processing_threads[256];
int serverSocket;

int process_input(void * info) {
	mlog_worker_thread_info * worker_info = info;
    int clientSocket;
    socklen_t clientLength;
    struct sockaddr clientAddress;
	int receivedMessageSize;
    mlog_session_data buffer;
    for (;;) {
        clientLength = sizeof(clientAddress);

#ifdef MLOG_USE_UDP
		if ((receivedMessageSize = recvfrom(serverSocket,
										    &buffer,
											sizeof(buffer),
   											0,
										    (struct sockaddr *) &clientAddress,
											&clientLength)) < 0) {
			fputs("Receiving was failed.\n", stderr);
		}
#else
        if ((clientSocket = accept(serverSocket, &clientAddress, &clientLength)) < 0) {
            fputs("Accept was failed.\n", stderr);
            exit(-4);
        }

        recv(clientSocket, &buffer, sizeof(buffer), MSG_WAITALL);
#endif

		printf("Thread id: %d\n", worker_info->id);
        mlog_print_session_data(&buffer);
		mlog_add_session_data(&buffer);
    }
}

int main()
{
#ifdef MLOG_USE_UDP
    if ((serverSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
#else
    if ((serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
#endif
        fputs("Socket creation was failed.\n", stderr);
        exit(-1);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        fputs("Cannot bind socket to address.\n", stderr);
        exit(-2);
    }

#ifndef MLOG_USE_UDP
    if (listen(serverSocket, 5) < 0) {
        fputs("Unsuccessful listen on socket.\n", stderr);
        exit(-3);
    }
#endif

    int clientSocket;
    socklen_t clientLength;
    struct sockaddr clientAddress;
	int receivedMessageSize;
    mlog_session_data buffer;

	for (int i = 0; i < 256; i++) {
		processing_threads[i].id = i;
		thrd_create(&(processing_threads[i].thread), process_input, &processing_threads[i]);
	}

	int return_code;
	for (int i = 0; i < 256; i++) {
		thrd_join(processing_threads[i].thread, &return_code);
	}
}

