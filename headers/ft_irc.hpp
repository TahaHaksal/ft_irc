#pragma once
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#define BUFFER_SIZE 4096

void	*err_check(void *cnd, void *res, std::string msg);
int	err_check(int cnd, int res, std::string msg);
void	exit_err(std::string msg);
