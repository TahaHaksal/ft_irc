#include "../../headers/Server.hpp"
/**
 * @brief 
 * 
 * @param name name of the user
 * @return int returns the fd value of the said user or returns -1 if none found
 */
int Server::findUserByName(std::string name)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end();it++)
    {
        if (it->second->getNickName() == name)
            return it->first;
    }
    return -1;
}

void    Server::broadcast(const std::vector<Client *> &clientList, std::string msg, int excludeFd)
{
    for (size_t i = 0; i < clientList.size(); i++)
    {
        if (clientList[i]->getFd() == excludeFd)
            continue ;
        ft_write(clientList[i]->getFd(), ":" + _clients[excludeFd]->getPrefixName() + " PRIVMSG " + _clients[excludeFd]->getNickName() + " :" + msg);
    }
}

void    Server::privmsg(int fd, std::vector<std::string> token)
{
    if (token[token.size() - 1][0] != ':' || token.size() < 3)
    {
        ft_write(fd, ":" + _clients[fd]->getPrefixName() + " 461 " + _clients[fd]->getNickName() + " PRIVMSG :Not enough parameters\r\n");
        return ;
    }
    int nickFd = findUserByName(token[1]);
    if (_clients.find(nickFd) != _clients.end())
    {
        //FOUND USER
        std::cout << "Found user\n";
        ft_write(_clients[nickFd]->getFd(), ":" + _clients[fd]->getPrefixName() + " PRIVMSG " + token[1] + " :" + token[2]);
        return ;
    }
    if (_channels.find(token[1]) != _channels.end())
    {
        //FOUND CHANNEL
        std::cout << "Found channel\n";
        broadcast(_channels.find(token[1])->second->_channelClients, token[2], fd);
        return ;
    }
    //NO USER OR CHANNEL MATCHES
    std::cout << "Couldn't find user or channel\n";
    ft_write(fd, ":" + _clients[fd]->getPrefixName() + " 401 " + token[1] + ": No such Nick/Channel\r\n");
}