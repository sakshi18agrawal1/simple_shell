#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<time.h>

int create_process();
char *in[3];
int main(void)
{
 	int f1=open("one", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	int f2=open("ten", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  	int f3=open("hun", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  	
	int arr[250000]={0};
	write(f1,arr, 1000000);
	for(int i=0;i<10;i++)
		write(f2,arr, 10000000);
	for(int i=0;i<100;i++)
		write(f3,arr, 100000000);

 	close(f1);
 	close(f2);
 	close(f3);

	in[0]="/home/mansi/Documents/os/final/file_copy";
	in[1]="/home/mansi/Documents/os/final/one";
	in[2]="/home/mansi/Documents/os/final/size";

	clock_t start=clock();
	create_process();
	clock_t end=clock();
	double cpu_time=((double)(end-start)*1000000/CLOCKS_PER_SEC);
	printf("time taken to copy 1 MB file %lf\n",cpu_time);

	in[1]="/home/mansi/Documents/os/final/ten";	
	start=clock();
	create_process();
	end=clock();
	cpu_time=((double)(end-start)*1000000/CLOCKS_PER_SEC);
	printf("time taken to copy 10 MB file %lf\n",cpu_time);

	in[1]="/home/mansi/Documents/os/final/hun";
	start=clock();
	create_process();
	end=clock();
	cpu_time=((double)(end-start)*1000000/CLOCKS_PER_SEC);
	printf("time taken to copy 100 MB file %lf\n",cpu_time);

}
int create_process()
{
	pid_t p;
	
	p=fork();
	if(p<0)
	{
		fprintf(stderr,"fork failed\n");
		exit(-1);
	}

	else if(p==0)
	{
		
		return(execvp(in[0],in));
		//printf("%d\n",execvp(search_path,in));
	}

	else
	{
		wait(NULL);
	}
return 0;
}
