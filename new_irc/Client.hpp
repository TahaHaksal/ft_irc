#ifndef CLIENT_HPP
# define CLIENT_HPP

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

class Client
{
    private:
        int _fd;
        int _port;

        std::string _username;
        std::string _nickname;
        std::string _realname;
        std::string _hostname;

        // Channel     *_channels;
    public:
        Client();
        ~Client();

        void    setNickname(std::string const &nickname) { _nickname = nickname; }
        void    setRealname(std::string const &username) { _username = username; }
};

#endif
