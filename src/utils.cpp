#include "../headers/Utils.hpp"

void    serverInfo(const std::string &message)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);

	std::cout << ltm->tm_mday << "." << ltm->tm_mon+1 << "." << ltm->tm_year+1900;
  	std::cout << " " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << std::endl;
	std::cout << "[" << message << "]" << std::endl;
};

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

std::vector<std::string> tokenize(std::string &msg) {
    std::vector<std::string> tokens;
    std::istringstream iss(msg);
    std::string token;

    while (std::getline(iss, token, ' ') || std::getline(iss, token, '\r') || std::getline(iss, token, '\n')){
		tokens.push_back(token);
		if (token[0] == ':')
		{
			std::cout <<"Broke at " << token <<"\n";
			break ;
		}
	}
    return tokens;
}