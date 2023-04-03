#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"


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
