#include "ioUtils.h"

using namespace std;

#define dmax(a,b)  (((a) > (b)) ? (a) : (b))
#define dmin(a,b)  (((a) < (b)) ? (a) : (b))

//获取特定格式的文件名
int readFileList(std::vector<string> &filelist, const char *basePath, string format)
{
    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir=opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8)    //file
        {
            // printf("d_name:%s/%s\n",basePath,ptr->d_name);
            string temp = ptr->d_name;
            //cout  << temp << endl;
            string sub = temp.substr(temp.length() - 4, temp.length()-1);
            //cout  << sub << endl;
            if(sub == format)
            {
                string path = basePath;
                path += "/";
                path += ptr->d_name;
                filelist.push_back(path);
            }
        }
        else if(ptr->d_type == 10)    ///link file
        {
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
        }
        else if(ptr->d_type == 4)    ///dir
        {
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            readFileList(filelist, base, format);
        }
    }
    closedir(dir);
    return 1;
}

//找出目录中所有子目录
int findAllSubDir(std::vector<string> &filelist, const char *basePath, bool recursive=false)
{
    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir=opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8)    //file
        {}
        else if(ptr->d_type == 10)    ///link file
        {}
        else if(ptr->d_type == 4)    ///dir
        {
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            filelist.push_back(base);
            if(recursive)
                findAllSubDir(filelist, base);
        }
    }
    closedir(dir);
    return 1;
}


void findDir(string src, string &facefolder, string &facenameindex, string filePath)
{
    int begin = src.find(filePath) + filePath.size() + 1;
    int end = 0;
    for (int i = src.size() - 1; i >= 0; --i)
    {
        //cout << src[i] << endl;
        if (src[i] == '/')
        {
            end = i;
            break;
        }
    }
    //cout << begin << endl;
    //cout << end << endl;
    facefolder = src.substr(begin, end - 1 - begin + 1);
    facenameindex = src.substr(end + 1, src.size() - 1 - (end + 1) + 1);
}


void GetStringFileName(const string &filePath, string &filename, string &fileformat)
{
    int fileformat_begin = 0;
    int fileformat_end = filePath.length() - 1;
    int filename_begin = 0;
    int filename_end = filePath.length() - 1;
    for (int i = filePath.length() - 1; i >= 0; --i)
    {
        //cout << filePath[i] << endl;
        if (filePath[i] == '.')
        {
            fileformat_begin = i + 1;
            filename_end = i - 1;
        }

        if (filePath[i] == '/')
        {
            filename_begin = i + 1;
            break;
        }
    }
    // cout << filename_begin << endl;
    // cout << filename_end << endl;
    filename = filePath.substr(filename_begin, filename_end - filename_begin + 1);
    fileformat = filePath.substr(fileformat_begin, fileformat_end - fileformat_begin + 1);
}

void StringSplit(const string &src, const char splitchar, vector<string> &dst)
{
    int begin = 1;
    int end = 1;
    int i = 1;
    for (i = 1; i < src.length(); ++i)
    {
        if(src[i] == splitchar)
        {
            end = i - 1;
            dst.push_back(src.substr(begin, end - begin + 1));
            begin = i + 1;
            end = begin;
        }
    }

    //last
    if(i > end)
    {
        end = i - 1;
    }
    dst.push_back(src.substr(begin, end - begin + 1));
}

void create_path(string folder_path)
{
    string command;
    command = "mkdir -p " + folder_path;  
    system(command.c_str());
}

// int main()
// {
//     // sence2();
//     std::vector<string> files;
//     string format = ".jpg";
//     string video_path = "/home/haodong/Data/CSL_Isolated/color_video_125000";
//     findAllSubDir(files, video_path.c_str(), true);
//     sort(files.begin(),files.end());
//     for (int i = 0; i < files.size(); i++)
//     {
//         string file = files[i];
//         std::cout << file << endl;
//     }
    

//     return 0;
// }