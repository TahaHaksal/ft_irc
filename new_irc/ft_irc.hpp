#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include "Client.hpp"
#include "Server.hpp"

#define BUFFER_SIZE 4096
#define USER_SIZE 100

void	*errCheck(void *cnd, void *res, std::string msg);
int	    errCheck(int cnd, int res, std::string msg);
void	exitErr(std::string msg);

#endif