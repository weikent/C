/*
 * =====================================================================================
 *
 *    Filename:  AllChange.c
 *
 * Description:  用于批量转换当前目录下的所有html文件到pdf文件。
 *               主要是使用html2pdf这个工具来工作
 *
 *    
 *
 *     Version:  0.1
 *     Created:  12 Jun 2014  13:23:35
 *
 *     Authors:  weishijian
 *     Company:  DingQing Technology, Ltd.
 *    Revision:  
 * ======================================================================================
 * @0.1   weishijian   12 Jun 2014  13:23:35 , create orignal file
 * ======================================================================================
 * Copyright (c) , DingQing Technology, Ltd.
 * ======================================================================================
 */

/*----------------Includes---------------*/ 
//包含的头文件 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------Local Variables----------- */ 
//定义一些本地变量 



/*------Local Structures and Typedefs ---*/ 
//要使用的一些数据结构 



/*-----------Extern Variables -----------*/ 
//使用到的一些外部变量 



/*-------------Definitions---------------*/ 
//一些#defines及具体的函数实现


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

int len = 0;
int trave_dir(char* path)
{
    DIR *d; 
    struct dirent *file; 
    struct stat buf;    
    
    if(!(d = opendir(path)))
    {
        printf("error opendir %s!!!\n",path);
        return -1;
    }
    chdir(path);//Add this, so that it can scan the children dir(please look at main() function)
    while((file = readdir(d)) != NULL)
    {
       
        if(strncmp(file->d_name, ".", 1) == 0)
            continue;
        if(stat(file->d_name, &buf) >= 0 && !S_ISDIR(buf.st_mode) )
        {

            if (strstr(file->d_name, ".html"))
            {
                printf("%s\n",file->d_name);
                printf("\tfile size=%d\n",buf.st_size);
                printf("\tfile last modify time=%d\n",buf.st_mtime);
                char command[100] = {0};
                int len = strlen(file->d_name);
                char name[50] = {0};
                strncpy(name, file->d_name, len - 5);

                sprintf(command, "html2pdf %s %s.pdf", file->d_name, name);
                printf ("command = %s\n", command);
                system(command);
                remove(file->d_name);
                printf ("%d\n", len);
            }
        }
    }
    closedir(d);
    return 0;
}
int main()
{
    int i;
    trave_dir(".");//trave_dir("./ChildrenDir");
    return 0;
}