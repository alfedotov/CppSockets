/*
 * General-purpose socket server class
 *
 * Copyright (C) 2019 Simon D. Levy
 *
 * MIT License
 */

#pragma once

#include "TcpSocket.hpp"

class TcpServerSocket : public TcpSocket {

    public:

        TcpServerSocket(const char * host, uint16_t port)
            : TcpSocket(host, port)
        {
            if(_sock != INVALID_SOCKET)
            {
                // allow reuse
                int option = 1;
                setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

                // assign IP, PORT
                struct sockaddr_in servaddr;
            	servaddr.sin_family = AF_INET;
            	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
            	servaddr.sin_port = htons(port);
                // Bind socket to address
                //if (bind(_sock, _addressInfo->ai_addr, (int)_addressInfo->ai_addrlen) == SOCKET_ERROR) {
                // quick fix for bind failing
                if (bind(_sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
                    closesocket(_sock);
                    _sock = INVALID_SOCKET;
                    sprintf_s(_message, "bind() failed");
                    return;
                }
            }
        }

        void acceptConnection(void)
        {
            // Listen for a connection, exiting on failure
            if (listen(_sock, 1)  == -1) {
                sprintf_s(_message, "listen() failed");
                return;
            }

            // Accept connection, exiting on failure
            printf("Waiting for client to connect on %s:%s ... ", _host, _port);
            fflush(stdout);
            _conn = accept(_sock, (struct sockaddr *)NULL, NULL);
            if (_conn == SOCKET_ERROR) {
                sprintf_s(_message, "accept() failed");
                return;
            }
            printf("connected\n");
            fflush(stdout);
        }
};
