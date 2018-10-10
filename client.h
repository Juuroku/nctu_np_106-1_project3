#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class client
{
	public:
	int fd;
	std::string addr;
	int port;
	std::string filename;
	int id;
	FILE *fp;
	int rflg;
};

#endif
