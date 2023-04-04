#include "../headers/ft_irc.hpp"

void	exitErr(std::string msg)
{
	std::cerr << msg << "\n";
	exit(1);
}

int	errCheck(int cnd, int res, std::string msg)
{
	if (cnd == res)
		exitErr(msg);
	return res;
}

int	main(int ac, char **av)
{
	if (ac != 3)
	 	exitErr("Usage: ./ft_irc <port> <password>");
	Server server(av);
}
