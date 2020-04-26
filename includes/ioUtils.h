#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <algorithm>

using namespace std;

#define dmax(a,b)  (((a) > (b)) ? (a) : (b))
#define dmin(a,b)  (((a) < (b)) ? (a) : (b))

int readFileList(std::vector<string> &filelist, const char *basePath, string format);
int findAllSubDir(std::vector<string> &filelist, const char *basePath, bool recursive);
void findDir(string src, string &facefolder, string &facenameindex, string filePath);
void GetStringFileName(const string &filePath, string &filename, string &fileformat);
void StringSplit(const string &src, const char splitchar, vector<string> &dst);
void create_path(string folder_path);
