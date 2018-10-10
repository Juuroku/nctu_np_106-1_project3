#include <stdio.h>
#include <unistd.h>
#include "htmli.h"

int main()
{
	std::vector<client> cli_list;
	cli_list.clear();
	client cli;
	cli.addr = std::string("some address");
	cli.port = 8888;
	cli_list.push_back(cli);
	cli_list[0].id = 0;
	html_init(cli_list);
	std::string msg("hello\n");
	for(int i = 0;i < 3; i++)
	{
		sleep(2);
		printf("<script>document.getElementById(\"0\").innerHTML += \"hello\";</script>");
	}
	return 0;
}
