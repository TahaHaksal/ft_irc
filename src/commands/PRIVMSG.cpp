#include "../../headers/Server.hpp"

void    Server::privmsg(int fd, std::vector<std::string> token)
{
    // std::cout << "sa\n";
    if (token[token.size() - 1][0] != ':')
    {
        //:irc.example.com NOTICE sender :No text to send
        ft_write(fd, RPL_NOTICE(_clients[fd]->getPrefixName(), _clients[fd]->getNickName(), "No one to send!"));
        return ;
    }
    int i = checkUsr(token[1]);
    if (i == -1)
        return ;
    ft_write(_clients[i]->getFd(), token[token.size() - 1]);
    //:irc.example.com NOTICE ClientNick :Your message
    ft_write(fd, RPL_NOTICE(_clients[fd]->getPrefixName(), _clients[fd]->getNickName(), "Message sent!"));
    // _clients[fd]->clientMsgSender(fd, RPL_NOTICE(_clients[fd]->getNickName(), token[i], "Message sent!"));
}