#include "../../headers/Server.hpp"

void	Server::join(int fd, std::vector<std::string> token) {
	// std::cout << "JOIN Function started.\n";

	std::string	msg;
	if (token.empty() || token[1].empty() || (token.size() != 2 && token.size() != 3)) {
		//ERR_NEEDMOREPARAMS
		// :server.example.com 461 <nickname> <command> :Not enough parameters
		msg = ":" + _clients[fd]->getPrefixName() + " 461 " + _clients[fd]->getNickName() + " JOIN :Not enough parameters"; ft_write(fd, msg);
		return ;
	}

	if (token[1][0] != '#')
		token[1] = "#" + token[1];

	if (_channels.find(token[1]) == _channels.end())
	{
		// Admin girişi, kanal oluşturuluyor.
		
		msg = token[1] + " kanalı oluşturuluyor"; ft_write(fd, msg);
		msg = "Admin girişi"; ft_write(fd, msg);

		_channels[token[1]] = new Channel(_clients[fd], token[1], token.size() > 2 ? token[2] : "");
		_clients[fd]->_channels.push_back(_channels[token[1]]);

		// _clients[fd]->clientMsgSender(fd, RPL_NAMREPLY(_clients[fd]->getNickName(), token[1], _channels[token[1]]->getUsers()));
		// _clients[fd]->clientMsgSender(fd, RPL_ENDOFNAMES(_clients[fd]->getNickName(), token[1]));
		// ^Names cevapları bize bunlar gelmiyor yapmaya gerek yok.
		
		_clients[fd]->casting(fd, _channels[token[1]]->_channelClients, RPL_JOIN(_clients[fd]->getPrefixName(), token[1]) + " :" + _clients[fd]->getNickName());

		return ;
	}

	for (size_t i = 0 ; i < _clients[fd]->_channels.size() ; i++)
	{
		if (_clients[fd]->_channels[i]->getName() == token[1])
		{
			msg = "Zaten bu kanaldasınız!";
			ft_write(fd, msg);
			return ;
		}
	}
	if (_channels[token[1]]->getClientCount() >= _channels[token[1]]->getMaxClientCount())
		msg = "Kanaldaki kullanıcı sayısı doldu!";
	else if (!(_channels[token[1]]->getPassword().empty()) && _channels[token[1]]->getPassword() != token[2])
		msg = "Kanala girilemedi, parola hatası!";
	else
	{
		// Üye girişi
		// msg = "Üye girişi\r\n";
		_channels[token[1]]->_channelClients.push_back(_clients[fd]);
		_clients[fd]->_channels.push_back(_channels[token[1]]);
		_channels[token[1]]->setClientCount(_channels[token[1]]->getClientCount() + 1);

		// _clients[fd]->clientMsgSender(fd, RPL_NAMREPLY(_clients[fd]->getNickName(), token[1], _channels[token[1]]->getUsers()));
		// _clients[fd]->clientMsgSender(fd, RPL_ENDOFNAMES(_clients[fd]->getNickName(), token[1]));
		// ^Names cevapları bize bunlar gelmiyor yapmaya gerek yok.

		_clients[fd]->casting(fd, _channels[token[1]]->_channelClients, RPL_JOIN(_clients[fd]->getPrefixName(), token[1]) + " :" + _clients[fd]->getNickName());
	}
	std::cout << "Kullanıcı sayısı:	" << _channels[token[1]]->getClientCount() << std::endl;
	ft_write(fd, msg);
}
