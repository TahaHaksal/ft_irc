#include "../../headers/Server.hpp"

std::string getJoinName(std::string const &_username, std::string const &_hostname, std::string const &_nickname)
{
	std::string nickname = _nickname;

	if (!(_username.empty()))
		nickname += '!' + _username;
	if (!(_hostname.empty()))
		nickname += '@' + _hostname;
	return (nickname);
}

void ft_write(int _fd, const std::string &message)
{
	std::string buffer = message + "\r\n";

	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error while sending message to client.");
}

void reply(int _fd, std::string const &msg, std::string const &_username, std::string const &_hostname, std::string const &_nickname)
{
	ft_write(_fd , ":" + getJoinName(_username, _hostname, _nickname) + " " + msg);
}

void broadcast(int _fd, std::vector<Client *> _clients, const std::string &message)
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
		ft_write(_fd, message);
}

void	Server::join(int fd, std::vector<std::string> token) {
	// std::cout << "JOIN Function started.\n";

	std::string	msg;
	if (token.empty() || token[1].empty()) {
		msg = "JOIN komutunda yeterli argüman yok!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}

	if (token[1][0] != '#')
		token[1] = "#" + token[1];
	
	if (token[1].size() < 2) // "/join #" kontrolü
	{
		msg = "Kanal belirtmediniz!\r\n";
		send(fd, msg.c_str(), msg.size(), 0);
		return ;
	}
	
	if (_channels.find(token[1]) == _channels.end())
	{
		// Admin girişi, kanal oluşturuluyor.
		msg = token[1] + " kanalı oluşturuluyor\r\n"; send(fd, msg.c_str(), msg.size(), 0);
		msg = "Admin girişi\r\n"; send(fd, msg.c_str(), msg.size(), 0);
		_channels[token[1]] = new Channel(_clients[fd], token[1], token.size() > 2 ? token[2] : "");
		_clients[fd]->_channels.push_back(_channels[token[1]]);


#define RPL_NAMREPLY(source, channel, users)	"353 " + source + " = " + channel + " :" + users
#define RPL_ENDOFNAMES(source, channel)			"366 " + source + " " + channel + " :End of /NAMES list."
#define RPL_JOIN(source, channel)					":" + source + " JOIN :" + channel

std::string users;
for (int i = 0 ; i < _channels[token[1]]->_channelClients.size() ; i++)
	users.append(_channels[token[1]]->_channelClients[i]->getNickName() + " ");

reply(fd, RPL_NAMREPLY(_clients[fd]->getNickName(), token[1], users), _clients[fd]->getUserName(), _clients[fd]->getHostName(), _clients[fd]->getNickName());
reply(fd, RPL_ENDOFNAMES(_clients[fd]->getNickName(), token[1]), _clients[fd]->getUserName(), _clients[fd]->getHostName(), _clients[fd]->getNickName());

broadcast(fd, _channels[token[1]]->_channelClients, RPL_JOIN(getJoinName(_clients[fd]->getUserName(), _clients[fd]->getHostName(), _clients[fd]->getNickName()), token[1]));





		return ;
	}

	for (int i = 0 ; i < _clients[fd]->_channels.size() ; i++)
	{
		if (_clients[fd]->_channels[i]->getName() == token[1])
		{
			msg = "Zaten bu kanaldasınız!\r\n";
			send(fd, msg.c_str(), msg.size(), 0);
			return ;
		}
	}
	if (_channels[token[1]]->getClientCount() >= _channels[token[1]]->getMaxClientCount())
		msg = "Kanaldaki kullanıcı sayısı doldu!\r\n";
	else if (!(_channels[token[1]]->getPassword().empty()) && _channels[token[1]]->getPassword() != token[2])
		msg = "Kanala girilemedi, parola hatası!\r\n";
	else
	{
		// Üye girişi
		msg = "Üye girişi\r\n";
		_channels[token[1]]->_channelClients.push_back(_clients[fd]);
		_clients[fd]->_channels.push_back(_channels[token[1]]);
		_channels[token[1]]->setClientCount(_channels[token[1]]->getClientCount() + 1);
	}
	std::cout << "Kullanıcı sayısı:	" << _channels[token[1]]->getClientCount() << std::endl;
	send(fd, msg.c_str(), msg.size(), 0);
}
