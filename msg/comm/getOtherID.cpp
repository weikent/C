/* 
 *     Created:  15 May 2013 10:13:19
 *     Author:   weishijian@ubuntu
 Description:  
*/

//#include <config.h>
#include "getOtherID.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


#define MAXLINE 1024
getOtherID::getOtherID()
{
}

getOtherID::~getOtherID()
{
}

int getOtherID::isdir(const char *path)
{
    struct stat buf;
    if(lstat(path,&buf)<0)
    {
//       printf("lstat err for %s \n",path);
	return 0;
    }
    if(S_ISDIR(buf.st_mode)||S_ISLNK(buf.st_mode))
    {
	return 1;
    }
    return 0;
}

/********************************/
/* 用来得到字符串path中的文件名 */
/********************************/
char * getOtherID::getfile(const char *path)
{
    static char filename[MAXLINE];
    char *tmp1,*tmp2,filepath[MAXLINE];


    if(path==NULL)return NULL;


    memset(filepath,0,sizeof(filepath));
    strcpy(filepath,path);
    tmp2=tmp1=strtok(filepath,"/");

    //此while循环执行道最后的时候tmp1会被赋值为NULL。所以需要tmp2来记录最后一个 "/" 后面的字符串
    while((tmp1=strtok(NULL,"/")))
    {
	tmp2=tmp1;
    }


    memset(filename,0,sizeof(filename));

    if(tmp2)
    {
	strcpy(filename,tmp2);
	tmp1=filename;
	while(*tmp1&&*tmp1++!=' ');
	*(tmp1)='\0';
    }
    else return NULL;
    return filename;
}
/* char* getOtherID::readstatus(const char *filename) */
/* { */
/*     FILE *file; */
/*     static char buf[MAXLINE]; */
/*     file=fopen(filename,"r"); */
/*     if(!file)return NULL; */
/*     while(fgets(buf,sizeof(buf),file)) */
/*     { */
/* 	if(strncmp(buf,"State",5)==0) */
/* 	{ */
/* //	    printf("%s\n",buf); */
/* 	    break; */
/* 	} */
/*     } */
/*     fclose(file); */
/* } */



/*********************/
/* 获取进程ID	     */
/* filename:进程名称 */
/* 返回进程ID	     */
/*********************/
int getOtherID::getID(const char *filename)
{
    char buf1[MAXLINE],buf2[MAXLINE];
    char path1[MAXLINE],path2[MAXLINE],*ptr;
    DIR *db,*directory;
    struct dirent *p;

    db=opendir("/proc/");

    if(db==NULL)
    {
	return -1;
    }

    while ((p=readdir(db)))
    {
	if((strcmp(p->d_name,".")==0)||(strcmp(p->d_name,"..")==0))
	    continue;
	else
	{
	    memset(buf1,0,sizeof(buf1));
	    sprintf(buf1,"/proc/%s",p->d_name);

	    if(isdir(buf1))
	    {
		memset(buf2,0,sizeof(buf2));
		sprintf(buf2,"%s/exe",buf1);
		if(access(buf2,F_OK)==0) // 文件存在 或者是有权限访问
		{
		    memset(path1,0,sizeof(path1));
		    if(readlink(buf2,path1,sizeof(path1))==-1)
			continue;
		    ptr=getfile(path1);

		    if(strcmp(ptr,filename)==0)
		    {
			/* memset(path2,0,sizeof(path2)); */
			/* sprintf(path2,"%s/status",buf1); */
			/* ptr=readstatus(path2); */
			sprintf(buf1,"%s",p->d_name);

			int i;
			i = atoi(buf1);
			return i;
		    }
		}
	    }
	}
    }
    closedir(db);
}
