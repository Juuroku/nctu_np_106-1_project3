#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <regex>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define MAXLINE 10001

/*void init()
{
	setenv()
	return;
}*/

void not_found(int fd)
{
	std::string msg("HTTP/1.1 404 Not Found\r\nSever: NP http server\r\nContent-Type: text/html;charset=utf-8\r\nContent-Length: 5\r\n\r\n404\r\n");
	int n = send(fd, msg.c_str(),strlen(msg.c_str()), 0);
	if(n <= 0) printf("server: send error\n");
	printf("server: send 404\n");
	return;
}

void send_file(int fd,const char *line, char **argv, char **envp, char (*temp)[1001], int cnt)
{
	const char *delim = "?";
	const char *delim2 = ".";
	char *sepstr = strdup(line);
	char *filename;
	char *pre;
	char *sub;
	char *t;
	std::string tmp;
	filename = strsep(&sepstr, delim);
	printf("%s\n",filename);
	t = strsep(&sepstr, delim);
	if(t) tmp = std::string(t);
	else tmp = std::string("");
	std::string query = std::string("QUERY_STRING=") + tmp;
	strcpy(temp[cnt],query.c_str());
	envp[cnt] = temp[cnt];
	cnt++;
	printf("%s\n",query.c_str());
	std::string cl = std::string("CONTENT_LENGTH=") + std::to_string(query.length()-13);
	strcpy(temp[cnt],cl.c_str());
	envp[cnt] = temp[cnt];
	cnt++;
	std::string script = std::string("SCRIPT_NAME=") + std::string(filename);
	strcpy(temp[cnt],script.c_str());
	envp[cnt] = temp[cnt];
	cnt++;
	std::string at("AUTH_TYPE=http");
	strcpy(temp[cnt],at.c_str());
	envp[cnt] = temp[cnt];
	cnt++;
	std::string ru("REMOTE_USER=user");
	strcpy(temp[cnt],ru.c_str());
	envp[cnt] = temp[cnt];
	cnt++;
	std::string ri("REMOTE_IDENT=user");
	strcpy(temp[cnt],ri.c_str());
	envp[cnt] = temp[cnt];
	cnt++;
	char *ss = strdup(filename);
	sub = strsep(&ss, delim2);
	while(sub)
	{
		pre = sub;
		sub = strsep(&ss, delim2);
	}
	printf("%s\n",pre);
	if(!strcmp("cgi",pre))
	{
		int pid, p[2];
		//pipe(p);
		printf("server: fork for cgi\n");
		pid = fork();
		if(pid < 0)
		{
			printf("server: fork error\n");
			//return;
		}
		else if(pid == 0)
		{
			//close(p[0]);
			//fflush(stdout);
			printf("server: execute cgi\n");
			//setenv("QUERY_STRING",query,1);
			//std::string cl = std::to_string(strlen(query));
			//setenv("CONTENT_LENGTH", cl.c_str(),1);
			std::string msg("HTTP/1.1 200 OK\r\nSever: NP http server\r\n");
			int n = send(fd, msg.c_str(),strlen(msg.c_str()), 0);
			if(n <= 0) printf("server: send error\n");
			else printf("server: forward %d character(s)\n%s",n,msg.c_str());
			for(int i = 0;i < cnt;i++) printf("%s\n",envp[i]);
			dup2(fd, STDOUT_FILENO);
			if(execve(filename,argv,envp) == -1)
			{
				close(STDOUT_FILENO);
				printf("server: %s\n",strerror(errno));
				//close(p[1]);
				exit(1);
			}
		}
		else
		{
			//close(p[1]);
			int status;
			wait(&status);
			if(status)
			{
				printf("server: cgi execute error\n");
				not_found(fd);
			}
			else
			{
				//char buff[70001];
				//int n = read(p[0], buff,sizeof(buff));
				//buff[n] = '\0';
				//int i;
				/*for(i = 0;buff[i+1] != '\0';i++)
				{
					if(buff[i] == '\r' && buff[i+1] == '\n')
					{
						if(buff[i+2] == '\n' ||(buff[i+2] == '\r' && buff[i+3] == '\n')) break;
					}
					else if(buff[i] == '\n')
					{
						if(buff[i+1] == '\n' ||(buff[i+1] == '\r' && buff[i+2] == '\n')) break;
					}
				}
				std::string str(buff);
				if(buff[i]!='\0') str = str.substr(i);
				str.substr(str.find_first_not_of(" \t"));
				printf("server: read from pipe\n");*/
				//std::string msg = std::string("HTTP/1.1 200 OK\r\nSever: NP http server\r\nContent-Length: ") + std::to_string(str.length()) + std::string("\r\n") + std::string(buff) + std::string("\r\n");
				//n = send(fd, msg.c_str(),strlen(msg.c_str()), 0);
				//if(n <= 0) printf("server: send error\n");
				//else printf("server: forward %d character(s)\n%s",n,msg.c_str());
			}
			//close(p[1]);
			//return;
		}
	}
	else
	{
		printf("server: try to open %s\n", filename);
		FILE *fp;
		fp = fopen(filename,"r");
		if(fp)
		{
			std::string msg("HTTP/1.1 200 OK\r\nSever: NP http server\r\n");
			int n = send(fd, msg.c_str(),strlen(msg.c_str()), 0);
			if(n <= 0) printf("server: send error\n");
			msg = ("Content-Type: text/html;charset=utf-8\r\n");
			n = send(fd, msg.c_str(),strlen(msg.c_str()), 0);
			if(n <= 0) printf("server: send error\n");
			fseek(fp, 0L, SEEK_END);
			int sz = ftell(fp);
			msg = std::string("Content-Length: ") + std::to_string(sz) + std::string("\r\n\r\n");
			n = send(fd, msg.c_str(),strlen(msg.c_str()), 0);
			if(n <= 0) printf("server: send error\n");
			rewind(fp);
			while(!feof(fp))
			{
				char buff[MAXLINE];
				n = fread(buff, sizeof(char), sizeof(buff), fp);
				printf("server: read %d character(s)\n", n);
				if(n <= 0) printf("server: read file error\n");
				n = send(fd, buff, n, 0);
				if(n <= 0) printf("server: send error\n");
			}
			fclose(fp);
		}
		else
		{
			printf("server: can't open file %s\n", filename);
			not_found(fd);
		}
		//return;
	}
	exit(0);
}

void str_echo(int fd,char **argv, char **envp, char (*temp)[1001], int cnt)
{
	char line[MAXLINE];
	char read_buffer[MAXLINE];
	
	memset(line, 0, sizeof(line));
	memset(read_buffer, 0, sizeof(read_buffer));
	int flg;
	flg = 0;
	for(;;)
	{
		int i, j, n;
		n = recv(fd, read_buffer, sizeof(read_buffer), 0);
		if(n <= 0)
		{
			if(errno != EINTR)
			{
				printf("server: recv error\n");
				break;
			}
		}
		else
		{
			//printf("recieved:\n");
			printf("%s\n\n",read_buffer);
			std::regex re("\\s+");
			std::string line(read_buffer);
			size_t f;
			f = line.find_first_not_of(" \t");
			if(f > 0) line = line.substr(f);
			std::vector<std::string> splited_line
			{
				std::sregex_token_iterator(line.begin(), line.end(), re, -1),{}
			};
			for(int k = 0; k < splited_line.size(); k++)
			{
				//printf("%s\n",splited_line[k].c_str());
				if(!strcmp("GET", splited_line[k].c_str()))
				{
					flg = 1;
					std::string rm("REQUEST_METHOD=GET");
					strcpy(temp[cnt],rm.c_str());
					envp[cnt] = temp[cnt];
					cnt++;
					printf("server: unblock flag\n");
				}
				else if(flg)
				{
					flg = 0;
					printf("server: execute GET\n");
					/*if(k+1 >= splited_line.size())
					{
						not_found(fd);
						continue;
					}*/
					std::string filename = splited_line[k];
					if(filename[0] == '/') filename = filename.substr(1);
					int l;
					for(l = 0; l < filename.size(); l++)
					{
						if(filename[l] == '/')
						{
							printf("\'/\' found in no.%d charater\n",l);
							not_found(fd);
							break;
						}
					}
					if(l == filename.size())
					{
						send_file(fd,filename.c_str(),argv,envp,temp,cnt);
					}
				}
				else flg = 0;
			}
		}
	}	
}

int main()
{
	char *argv[257];
	char *envp[257];
	char temp[257][1001];
	int cnt;
	int sockfd, newsockfd, childpid;
	unsigned int clilen;
	struct sockaddr_in cli_addr, serv_addr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) printf("server: can't open stream socket\n");
	
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(9283);

	if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0) printf("server: can't bind local address\n");

	//init();

	listen(sockfd, 5);

	for(;;)
	{
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd,(struct sockaddr*) &cli_addr,&clilen);
		if(newsockfd < 0) printf("server: accept error\n");
		if((childpid = fork()) < 0) printf("server: fork error\n");
		else if(childpid == 0) // child
		{
			close(sockfd);
			cnt = 0;
			memset(argv, 0, sizeof(argv));
			memset(envp, 0, sizeof(envp));
			std::string ra = std::string("REMOTE_ADDR=") + std::string(inet_ntoa(cli_addr.sin_addr));
			strcpy(temp[cnt],ra.c_str());
			envp[cnt] = temp[cnt];
			cnt++;
			std::string rh = std::string("REMOTE_HOST=") + std::string(inet_ntoa(cli_addr.sin_addr));
			strcpy(temp[cnt],rh.c_str());
			envp[cnt] = temp[cnt];
			cnt++;
			std::string port = std::to_string(cli_addr.sin_port);
			std::string rp = std::string("REMOTE_PORT=") + port;
			strcpy(temp[cnt],rp.c_str());
			envp[cnt] = temp[cnt];
			cnt++;
			str_echo(newsockfd,argv,envp,temp,cnt); // execute function
			exit(0);
		}
		close (newsockfd);
		int status;
		wait(&status);
		if(status) printf("server: fork exec error\n");
	}

	return 0;
}
