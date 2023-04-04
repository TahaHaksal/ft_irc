#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include "Client.hpp"

class Server
{
    private:
        int _socket_fd;
        int _port;
        std::string _password;

        std::vector<pollfd *> _pollfds;
        std::vector<Client *> _Client;
        // std::vector<Channel *>  _channels;
    public:
        Server(char **av);
        ~Server();

        int createSocket();
};

#endif