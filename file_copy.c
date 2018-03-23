#include<sys/types.h>
#include<string.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>  
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>

int copy(char *argv[]);
int check_valid(char *p);

int main(int argc, char *argv[])
{
	int x;
	//printf("Inside\n");
	if(argc==1)
		printf("Enter file name and destination path\n");
	else if(argc==2)
		printf("File name or destination path is missing\n");
	else
		x=copy(argv);
	
	if(x==0)
	{
		int r= remove(argv[1]);
		if(r==0)
			printf("File deleted\n");
	}
	return 0;
}
int copy(char *argv[])
{
	int flag=0;
	for(int i=strlen(argv[1])-1;i>=0;i--)
	{
		if(argv[1][i]=='/')
		{
			flag=i+1;
			break;
		}
	}
	char *file_name=(char *)malloc(sizeof(char)*100);
		
	for(int i=0;i<strlen(argv[1])-flag;i++)
	{
		file_name[i]=argv[1][i+flag];
	}	
	
	int f1= open (argv[1], O_RDONLY);
	if(f1<0)
	{
		printf("File to be copied does not exists\n");
		return -1;
	}
	
	if(check_valid(argv[2])==-1)
	{
		printf("Invalid destination path\n");
 		return -1;
   	}
	
	char *file_path= argv[2];
	strcat(file_path, "/");
	strcat(file_path, file_name);
	
	int f2= open(file_path, O_RDONLY);
	if(f2>=0)
	{
		printf("File already exists in the destination\n");
		return -1;
	}
	char *buff=(char *)malloc(sizeof(char)*10);
	f2= open(file_path,  O_WRONLY | O_CREAT,0666);
	int r=read(f1, buff, 10);
	while(r!=0)
	{
		int w=write(f2, buff, r);
		r=read(f1, buff, 10);
	}
	printf("File copied\n");
	close(f1);
	close(f2);
	return 0;
}
int check_valid(char *p)
{
	DIR* dir = opendir(p);
	if (dir)
	{
		closedir(dir);
		return 1;
	}
	else return -1;
}
