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

void    Server::broadcast(const std::vector<Client *> &clientList, std::string msg, std::string channelName, int excludeFd = -1)
{
    for (size_t i = 0; i < clientList.size(); i++)
    {
        if (clientList[i]->getFd() == excludeFd)
            continue ;
        ft_write(clientList[i]->getFd(), ":" + _clients[excludeFd]->getPrefixName() + " PRIVMSG " + channelName + " " + msg);
    }
}

void    Server::privmsg(int fd, std::vector<std::string> token)
{
    //token[2][0] == '#' kanal mesajı | değilse -> bireysel mesajlaşma (kullanıcı kontrolü)
    if (token[token.size() - 1][0] != ':' || token.size() != 3)
    {
        ft_write(fd, ":" + _clients[fd]->getPrefixName() + " 461 " + _clients[fd]->getNickName() + " PRIVMSG :Not enough parameters\r\n");
        return ;
    }
    int nickFd = findUserByName(token[1]);
    if (_clients.find(nickFd) != _clients.end())
    {
        std::string ping("PING");
        std::string pong("PONG");
        if (token[2].find_first_of(ping) != std::string::npos || token[2].find_first_of(pong) != std::string::npos)
        {
            std::string cmd = token[2].substr(2, 4);
            size_t  pos = token[2].find_first_of(' ');
            std::vector<std::string> newTok;
            newTok.push_back(cmd);
            newTok.push_back(token[2].substr(pos + 1));
            if (_commands.find(newTok[0]) != _commands.end())
                (this->*_commands[newTok[0]])(fd, newTok);
            return ;
        }
        //FOUND USER
        ft_write(_clients[nickFd]->getFd(), ":" + _clients[fd]->getPrefixName() + " PRIVMSG " + token[1] + " :" + token[2]);
        return ;
    }
    else if (_channels.find(token[1]) != _channels.end())
    {
        //FOUND CHANNEL
        broadcast(_channels.find(token[1])->second->_channelClients, token[2], token[1], fd);
        return ;
    }
    //NO USER OR CHANNEL MATCHES
    ft_write(fd, ":" + _clients[fd]->getPrefixName() + " 401 " + token[1] + ": No such Nick/Channel\r\n");
}