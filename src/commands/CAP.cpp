#include "../../headers/Server.hpp"

//Düzenlenecek.

void	Server::cap(int fd, std::vector<std::string> tokens) {
	//Gereksizmiş cap ls yalnızca protokol dışında desteklenen komut listesi istiyormuş
	(void)tokens;
	ft_write(fd, "");
	// std::string msg;
	// for (command_map::iterator it = _commands.begin(); it != _commands.end(); it++)
	// 	msg += "/" + it->first + "\n";
	// msg += "\r\n";
	// errCheck(-1, send(fd, msg.c_str(), msg.size(), 0), "Send failed");
}
