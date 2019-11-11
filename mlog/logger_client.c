#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include "data.h"

#define MLOG_USE_UDP

int main()
{
    int clientSocket;
#ifdef MLOG_USE_UDP
    if ((clientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
#else
    if ((clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
#endif
        fputs("Socket creation was failed.\n", stderr);
        exit(-1);
    }

    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddress.sin_port = htons(8082);

    if (bind(clientSocket, (struct sockaddr *) &clientAddress, sizeof(clientAddress)) < 0) {
        fputs("Cannot bind socket to address.\n", stderr);
        exit(-2);
    }

#ifndef MLOG_USE_UDP
    if (listen(clientSocket, 5) < 0) {
        fputs("Unsuccessful listen on socket.\n", stderr);
        exit(-3);
    }
#endif
	mlog_session_data buff = {
		.id = {
			.separated = {
				.client_id = 4,
				.session_id = 3
			}
		},
		.timestamp = 5,
		.data = {
			.severity = 6,
			.message = "SAJT"
		}
	};

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(8080);

	if (sendto(clientSocket, &buff, sizeof(buff), 0, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) != sizeof(buff)) {
		fputs("Error while sending.", stderr);
	}
	mlog_print_session_data(&buff);

	close(clientSocket);
}

