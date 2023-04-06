#include "../headers/Server.hpp"

Server::Server(char **av) : _usrCount(1)
{
    _port = std::atoi(av[1]);
    _password = av[2];
	memset((char *)&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);
	_addrLen = sizeof(_addr);
    _socketFd = createSocket();
	_map["QUIT"] = &Server::quit;
	_map["JOIN"] = &Server::join;
}

Server::~Server()
{

}

int Server::createSocket()
{
	int t = 1;

	_socketFd = errCheck(-1, socket(AF_INET, SOCK_STREAM, 0), "Error: failed to create socket.");
	errCheck(-1, setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t)), "Error: failed to setsockopt.");
	errCheck(-1, bind(_socketFd, (struct sockaddr *) &_addr, (socklen_t)_addrLen), "Error: failed to bind.");
	errCheck(-1, listen(_socketFd, 1024), "Error: failed to listen.");

	_pollFds[0].fd = _socketFd;
	_pollFds[0].events = POLLIN;
	for (int i = 1; i < USER_MAX; i++)
		_pollFds[i].fd = -1;
	serverInfo("ircserver listening...");
	return _socketFd;
}

void	Server::loop()
{
	while (1){
		errCheck(-1, poll(_pollFds, _usrCount, -1), "Poll Failed");
		if (_pollFds[0].revents == POLLIN)
		{
			//New User connection
			int clientFd = errCheck(-1, accept(_socketFd, (struct sockaddr *) &_addr, (socklen_t*)&_addrLen), "Accept Failed");
			for (int i = 1; i < USER_MAX; i++){
				if (_pollFds[i].fd == -1){
					_pollFds[i].fd = clientFd;
					_pollFds[i].events = POLLIN;
					_usrCount++;
					break ;
				}
			}
		}
		else
		{
			for (int i = 1; i < USER_MAX; i++){
				if (_pollFds[i].fd == -1)
					break ;
				if (_pollFds[i].revents & POLLIN){
					std::string msg = readMessage(_pollFds[i].fd);
					std::cout << msg << std::endl;
					std::vector<std::string> token = tokenize(msg);
					if (_map.find(token[0]) != _map.end())
						(this->*_map[token[0]])(i, token);
					for (int i = 0; i < token.size(); i++){
						std::cout << token[i] << std::endl;
					}
				}
			}
		}
	}
}

void	Server::quit(int fd, std::vector<std::string> token){
	close(_pollFds[fd].fd);
	_pollFds[fd].fd = -1;
	_usrCount--;
	std::cout << "Successfully quitted\n";
}

std::string Server::readMessage(int fd){
	std::string msg;
	char		buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	while (!std::strstr(buffer, "\r\n")){
		memset(buffer, 0, BUFFER_SIZE);
 
		errCheck(-1, recv(fd, buffer, BUFFER_SIZE, 0), "Error receiving the message");
		msg.append(buffer);
	}
	return msg;
}
void    Server::serverInfo(std::string message)
{
    time_t now = time(0);
	tm *ltm = localtime(&now);

	std::cout << ltm->tm_mday << "." << ltm->tm_mon+1 << "." << ltm->tm_year+1900;
  	std::cout << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
	std::cout << "[" << message << "]" << std::endl;
}

//Getters and Setters

int                 Server::getPort() {return _port;}
int                 Server::getServerfd() {return _socketFd;}
char*               Server::getPassword() {return _password;}
int			        Server::getAddrlen() {return _addrLen;}
struct sockaddr_in  Server::getAddr() {return _addr;}

void                Server::setPort(int port){_port = port;}
void                Server::setServerfd(int server_fd){_socketFd = server_fd;}
void                Server::setPassword(char* password){_password = password;}
void                Server::setAddrlen(int addrLen){_addrLen = addrLen;}
void                Server::setAddr(struct sockaddr_in addr){_addr = addr;}
