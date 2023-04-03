/*
*	@returns socket_fd
// */
// int	init_socket(struct sockaddr_in *addr, int port){

// 	//AF_INET = ipv4
// 	//SOCK_STREAM = TCP

// 	return (server_fd);
// }
#include "../headers/ft_irc.hpp"
int	main(int ac, char **av){
	// if (ac != 3)
	// 	exitErr("Usage: ./ft_irc <port> <password>");
	// int port = errCheck(0, atoi(av[1]), "Not a usable port!");
	// struct sockaddr_in	addr;
	// socklen_t			addr_len = sizeof(addr);
	// int	server_fd = errCheck(-1, socket(AF_INET, SOCK_STREAM, 0), "Socket failed");
	// addr.sin_family = AF_INET;
	// addr.sin_addr.s_addr = INADDR_ANY;
	// addr.sin_port = htons(port);
	// errCheck(-1, bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)), "Bind failed");
	// errCheck(-1, listen(server_fd, 42), "Listen failed");

	// struct pollfd poll_fds[USER_SIZE];
	// while (1){
	// 	struct sockaddr_in client_addr;
	// 	socklen_t client_addr_len = sizeof(client_addr);
	// 	int client_fd = errCheck(-1, accept(server_fd, (struct sockaddr*)&addr, &addr_len), "Accept failed");

	// 	char buffer[BUFFER_SIZE];
	// 	int nbytes = errCheck(-1, recv(client_fd, buffer, sizeof(buffer), 0), "Error receiving data");

	// 	std::cout << buffer << std::endl;

	// 	std::string response = "Hello client!";
	// 	errCheck(-1, send(client_fd, response.c_str(), response.length(), 0), "Error sending data");

	// 	std::cout << "Message sent!\n";
	// }

	// close(server_fd);
	// close(client_fd);
}
