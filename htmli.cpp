#include <stdio.h>
#include "client.h"
#include "htmli.h"

void html_init(std::vector<client> &cli_list)
{
	printf("Content-Type: text/html\r\n\r\n");
	printf("<html>\r\n");
	printf("<head>\r\n");
	printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=big5\" />");
	printf("<title>Network Programming Homework 3</title>\r\n");
	printf("</head>\r\n");

	printf("<body bgcolor=#336699 id=\"0\"> \r\n");
	//printf("<font face=\"Courier New\" size=2 color=#FFFF99>\r\n");
	
	/*printf("<table width=\"800\" border=\"1\">\r\n");
	printf("<tr>\r\n");
	for(int i = 0; i < cli_list.size();i++)
	{
		printf("<td>%s</td>",cli_list[i].addr.c_str());
	}
	printf("\r\n</tr>\r\n");

	printf("<tr>\r\n");
	for(int i = 0; i < cli_list.size();i++)
	{
		printf("<td valign=\"top\" id=\"%d\"></td>",cli_list[i].id);
	}
	printf("\r\n</tr>\r\n");

	printf("</table>\r\n");*/

	//printf("</font>\r\n");
	printf("</body>\r\n");
	printf("</html>\r\n");

	printf("<script>console.log(\"html_init\")</script>\r\n");

	return;
}

void html_write(int id, std::string msg, int flg)
{
	std::string pre;
	std::string post;
	//printf("<script>console.log(\"%s\")\r\n</script>",msg.c_str());
	for(int i = 0;i < msg.length();i++)
	{
		switch(msg[i])
		{
			case '<':
				pre = msg.substr(0,i);
				post = msg.substr(i+1);
				msg = pre + std::string("&lt;") + post;
				i = i + 3;
				break;
			case '>':
				pre = msg.substr(0,i);
				post = msg.substr(i+1);
				msg = pre + std::string("&gt;") + post;
				i = i + 3;
				break;
			case '\r':
				pre = msg.substr(0,i);
				post = msg.substr(i+1);
				msg = pre + post;
				i --;
				break;
			case '\n':
				pre = msg.substr(0,i);
				post = msg.substr(i+1);
				msg = pre + std::string("<br>") + post;
				i = i + 3;
				break;
			default:
				break;
		}
	}
	if(flg) msg = std::string("<b>") + msg + ("</b>");
	//printf("<script>document.getElementById(%d).innerHTML += \"%s\";</script>\r\n",id,msg.c_str());
	printf("<script>document.getElementById(%d).innerHTML += \"%s\";</script>\r\n",id,msg.c_str());
	printf("<script>console.log(\"%s\")</script>\r\n",msg.c_str());
}
