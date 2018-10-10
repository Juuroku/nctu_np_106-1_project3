#ifndef HTMLI_H
#define HTMLI_H

#include <string>
#include <vector>
#include "client.h"

void html_init(std::vector<client>&); //int number of client
void html_write(int,std::string,int); //int id, string msg, int flg

#endif
