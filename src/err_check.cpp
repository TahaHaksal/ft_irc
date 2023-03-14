#include "../headers/ft_irc.hpp"


void	exit_err(std::string msg){
	std::cerr << msg << "\n";
	exit(1);
}

/*
	@param condition if given parameter matches this param we exit with error
	@param res will be cross-checked with first param if its the same we exit with error
	@param msg error msg
	@ret returns 2nd param
*/
void	*err__check(void *cnd, void *res, std::string msg){
	if (cnd == res)
		exit_err(msg);
	return res;
}

/*
	@param condition if given parameter matches this param we exit with error
	@param res will be cross-checked with first param if its the same we exit with error
	@param msg error msg
	@ret returns 2nd param
*/
int	err_check(int cnd, int res, std::string msg){
	if (cnd == res)
		exit_err(msg);
	return res;
}
