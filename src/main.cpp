#include "../headers/ft_irc.hpp"

int	main(int ac, char **av){
	if (ac != 3)
		exit_err("Usage: ./ft_irc <port> <password>");
	int port = err_check(0, atoi(av[1]), "Not a usable port!");
	//AF_INET = ipv4
	//SOCK_STREAM = TCP
	int	server_fd = err_check(-1, socket(AF_INET, SOCK_STREAM, 0), "Socket failed");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	struct sockaddr addr2;
	err_check(-1, bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)), "Bind failed");
	err_check(-1, listen(server_fd, 42), "Listen failed");
	err_check(-1, accept(server_fd, &addr2, (socklen_t *)sizeof(addr2)), "Accept failed");
}
