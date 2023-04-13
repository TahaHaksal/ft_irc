#include "../headers/Utils.hpp"
#include "../headers/Server.hpp"

void	a(int ac, char **av)
{
	if (ac != 3)
	 	exitErr("Usage: ./ft_irc <port> <password>");
	try {
		Server server(av);
		server.loop();
	}
	catch (std::exception const &msg) {
		std::cerr << msg.what() << std::endl;
	}
}

int	main(int ac, char **av)
{
	a(ac, av);
	while (1)
	{
		/* code */
	}
	
}
