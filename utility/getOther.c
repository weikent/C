/*
  描述： 获取其他进程的进程id
  
  getOther 进程名称
*/


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
int isdir(char *path)
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
char * getfile(char *path)
{
    static char filename[MAXLINE];
    char *tmp1,*tmp2,filepath[MAXLINE];
    if(path==NULL)return NULL;
    memset(filepath,0,sizeof(filepath));
    strcpy(filepath,path);
    tmp2=tmp1=strtok(filepath,"/");
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
char* readstatus(char *filename)
{
    FILE *file;
    static char buf[MAXLINE];
    file=fopen(filename,"r");
    if(!file)return NULL;
    while(fgets(buf,sizeof(buf),file))
    {
	if(strncmp(buf,"State",5)==0)
        {
	    printf("%s\n",buf);
	    break;
        }
    }
    fclose(file);
}
int main(int argc,char ** argv)
{
    char buf1[MAXLINE],buf2[MAXLINE];
    char path1[MAXLINE],path2[MAXLINE],*ptr,*str;
    DIR *db,*directory;
    struct dirent *p;
    if(argc!=2)
    {
	printf("input file name .\nusage %s : filename \n",argv[0]);
	return 0;
    }

    while(1)
    {
	db=opendir("/proc/");

	if(db==NULL)
	{
	    printf ("aa\n");
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
		//printf ("%s\n",buf1);
		if(isdir(buf1))
		{
		    memset(buf2,0,sizeof(buf2));
		    sprintf(buf2,"%s/exe",buf1);
		    if(access(buf2,F_OK)==0)
		    {
			memset(path1,0,sizeof(path1));
			if(readlink(buf2,path1,sizeof(path1))==-1)
			    continue;
			ptr=getfile(path1);
			//printf ("1111\n");
//					printf ("%s\n",ptr);
			if(strcmp(ptr,argv[1])==0)
			{
//						printf ("2222\n");
			    memset(path2,0,sizeof(path2));
			    sprintf(path2,"%s/status",buf1);
			    ptr=readstatus(path2);
			    sprintf(buf1,"%s",p->d_name);
			    printf ("%s\n",buf1);
			    int i;
			    i = atoi(buf1);
			    printf ("%d\n",i);
			    kill(i, SIGUSR1);
			}
		    }
		}
	    }
	}
	closedir(db);
    }
}

