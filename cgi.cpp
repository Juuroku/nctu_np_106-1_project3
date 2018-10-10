#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/type.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>

#include "client.h"
#include "htmli.h"

#define MAXCLI 5

int main()
{
	std::vector<client> cli_list;
	int cl = getenv("CONTENT_LENGTH");
	if(cl > 0)
	{
		std::string query = std::string(getenv("QUERY_STRING"));
		while(cl)
		{
			client newcli;
			size_t f1 = query.find('=');
			if(f1 >= 0) query = query.substr(f1+1);
			size_t f2 = query.find('&');
			if(f2 >= 0)
			{
				newcli.addr = query.substr(0,f2);
				query = query.substr(f2+1);
			}
			f1 = query.find
		}
	}
	
	std::vector<int> sockfd;
	int n = sockfd.size() > MAXCLI ? MAXCLI : sockfd.size();
	int flag[MAXCLI];

	for(int i = 0;i < n;i++)
	
}
