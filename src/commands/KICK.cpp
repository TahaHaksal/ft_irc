#include "../../headers/Server.hpp"

void	Server::kick(int fd, std::vector<std::string> token)
{
	std::string	msg;
	if (token.size() < 3) {
		msg = "Eksik parametre girdiniz\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	std::string channelName = token[2].substr(token[2][0] == ':', token[2].size());
	std::string target = token[3].substr(token[3][0] == ':', token[3].size());
	std::string reason = "";

	if (token.size() > 4 && (token[4][0] != ':' || token[4].size() > 1)) {
		reason = "";
		for (size_t it = 4; it < token.size() ; it++)
			reason.append(token[it] + " ");
	}

	size_t	channelFinder = 0;
	for (channelFinder = 0 ; channelFinder < _clients[fd]->_channels.size() ; channelFinder++)
		if (_clients[fd]->_channels[channelFinder]->getName() == channelName)
			break;

	if (_clients[fd]->_channels.size() == 0 || channelFinder == _clients[fd]->_channels.size()) {
		msg = "Kanal bulunamadı!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (_clients[fd]->_channels[channelFinder]->getAdmin() != _clients[fd]) {
		msg = "Admin yetkiniz yoktur!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	int	targetClientFd = -1;
	for (size_t i = 0 ; i < _clients[fd]->_channels[channelFinder]->_channelClients.size() ; i++)
		if (_clients[fd]->_channels[channelFinder]->_channelClients[i]->getNickName() == target)
			targetClientFd = _clients[fd]->_channels[channelFinder]->_channelClients[i]->getFd();

	if (targetClientFd == -1) {
		msg = "Aranan nickname ile kullanıcı bulunamadı!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	int	removeChannelIndex = -1;
	for (size_t i = 0 ; i < _clients[targetClientFd]->_channels.size() ; i++)
		if (_clients[targetClientFd]->_channels[i]->getName() == channelName)
			removeChannelIndex = i;

	if (removeChannelIndex == -1) {
		msg = "Kullanıcı kanala üye değil!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return;
	}

	std::cout << std::endl << "Kanalın eski admini:	" << _channels[channelName]->getAdmin()->getNickName() << std::endl;
	for (size_t i = 0 ; i < _channels[channelName]->_channelClients.size() ; i++)
		std::cout << i + 1 << "->	"<< _channels[channelName]->_channelClients[i]->getNickName() << std::endl;
	
	_channels[channelName]->leftTheChannel(_clients[targetClientFd]);
	if (_channels[channelName]->getClientCount() == 0)
	{
		std::cout << channelName << " Kanalı Siliniyor...\n";
		_channels.erase(_channels.find(_channels[channelName]->getName()));
	}

	if (_channels.find(channelName) != _channels.end() && _channels[channelName]->getClientCount() > 0)
	{
		std::cout << std::endl << "Kanalın yeni admini:	" << _channels[channelName]->getAdmin()->getNickName() << std::endl;
		for (size_t i = 0 ; i < _channels[channelName]->_channelClients.size() ; i++)
			std::cout << i + 1 << "->	" << _channels[channelName]->_channelClients[i]->getNickName() << std::endl;
	}
	
	// msg = target + " Kanaldan ayrıldı. " + reason + "\r\n";
	std::cout << "Kicked\n";
	msg = ":" + channelName + " KICK " + target + reason;
	ft_write(fd, msg);
	broadcast(_channels[channelName]->_channelClients, msg, fd);

	// send(fd, msg.c_str(), msg.size(), 0);
	serverInfo(msg);
}
