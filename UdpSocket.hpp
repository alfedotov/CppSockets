/*
 * Class for UDP sockets
 *
 * Copyright (C) 2019 Simon D. Levy
 *
 * MIT License
 */

#pragma once

#include "SocketCompat.hpp"

class UdpSocket : public Socket {

    protected:

        struct sockaddr_in _si_other;
        socklen_t _slen = sizeof(_si_other);

    public:

        void setTimeout(uint32_t msec)
        {
            if (msec > 0) {
                Socket::setUdpTimeout(msec);
            }
        }

        void sendData(const void * buf, size_t len)
        {
            sendto(_sock, (const char *)buf, (int)len, 0, (struct sockaddr *) &_si_other, (int)_slen);

        }

        void sendDataTo(const char* host, const uint16_t port, const void* buf, size_t len)
        {
            struct sockaddr_in _si;

            memset((char*)&_si, 0, sizeof(_si));
            _si.sin_family = AF_INET;
            _si.sin_port = htons(port);
            Socket::inetPton(host, _si);

            sendto(_sock, (const char*)buf, (int)len, 0, (struct sockaddr*)&_si, (int)_slen);
        }

        int receiveData(void * buf, size_t len)
        {
            return recvfrom(_sock, (char *)buf, (int)len, 0, (struct sockaddr *) &_si_other, &_slen);
        }

        int receiveDataFrom(void* buf, size_t len, const char* remote_host)
        {
            int result;
            struct sockaddr_in _si_remote;

            memset((char*)&_si_remote, 0, sizeof(_si_remote));
            _si_remote.sin_family = AF_INET;
            Socket::inetPton(remote_host, _si_remote);

            result = recvfrom(_sock, (char*)buf, (int)len, 0, (struct sockaddr*)&_si_other, &_slen);

            if (_si_other.sin_addr.s_addr != _si_remote.sin_addr.s_addr)
                result = 0;

            return result;
        }

};
