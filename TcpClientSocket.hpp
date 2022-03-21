/*
 * Class for TCP client sockets
 *
 * Copyright (C) 2019 Simon D. Levy
 *
 * MIT License
 */

#pragma once

#include "TcpSocket.hpp"

class TcpClientSocket : public TcpSocket {

    public:

        TcpClientSocket(const char * host, const uint16_t port)
            : TcpSocket(host, port)
        {
        }

        void openConnection(void)
        {
            // Connect to server, returning on failure
            if (connect(_sock, _addressInfo->ai_addr, (int)_addressInfo->ai_addrlen) == SOCKET_ERROR) {
                closesocket(_sock);
                _sock = INVALID_SOCKET;
                sprintf_s(_message, "connect() failed; please make sure server is running");
                return;
            }

            // For a client, the connection is the same as the main socket
            _conn = _sock;

            // Success!
            _connected = true;
        }
};
