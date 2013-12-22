#include <stdio.h>
#include <string.h>
struct ss
{
	int a;
	int b;
};

void structss(char *buf)
{
	struct ss *ps;
	ps=(struct ss *)buf;
	printf("ps->a=%d\n",ps->a);
	printf("ps->b=%d\n",ps->b);
	printf("ps+1->a=%d\n",(ps+1)->a);
	printf("ps+1->b=%d\n",(ps+1)->b);

}
int main(char argc,char **argv)
{
	char chs[]={1,0,0,0,2,0,0,0};//ss.a=1,ss.b=2;
	char buf[20];
	struct ss *ps,*ps1,s2[2];
	ps=(struct ss*)chs;
	printf("ps->a=%d\n",ps->a);
	printf("ps->b=%d\n",ps->b);
	//ps1=malloc(sizeof(struct ss));
	s2[0].a=111;
	s2[0].b=222;
	s2[1].a=333;
	s2[1].b=444;
	memcpy(buf,&s2,sizeof(struct ss)*2);
	structss(buf);
	return 0;
}