#include "../headers/Server.hpp"

static inline void serverInfo(const std::string &message)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);

	std::cout << ltm->tm_mday << "." << ltm->tm_mon+1 << "." << ltm->tm_year+1900;
  	std::cout << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
	std::cout << "[" << message << "]" << std::endl;
};

Server::Server(char **av)
{
    _port = std::atoi(av[1]);
    _password = av[2];
    _socket_fd = createSocket();
}

Server::~Server()
{
    
}

int Server::createSocket()
{
	int (temp) = 1;
	struct sockaddr_in server_address;
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	memset((char *)&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(_port);

	if (socket_fd == -1)
		throw std::runtime_error("Error: failed to create socket.");

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp)) == -1)
		throw std::runtime_error("Error: failed to setsockopt.");

	if (bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1)
		throw std::runtime_error("Error: failed to bind.");

	if (listen(socket_fd, 1024) == -1)
		throw std::runtime_error("Error: failed to listen.");

	serverInfo("ircserver listening...");
	return socket_fd;
}

