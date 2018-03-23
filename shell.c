#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h> 

typedef struct Node Node;
struct Node
{
    char *p;
    Node *next;
};
char *curr_dir;
char *prev_dir;
Node *path;
char* get_dir();
int change_dir(char *path);
char *stack[500];
int top=-1;
int push(char *p);
int pop();
int print_dir();
int add_path(char *new_p);
int del_path(char *del_p);
int disp_path();
int parse_inp(char *inp, char *in[]);
int check_valid(char *p);
int create_process(char *in[],char *search_path);

int main(int argc, char *argv[])
{	
	prev_dir=NULL;
	curr_dir=(char *)malloc(sizeof(char)*500);
	while(1)
	{
		printf(">>");
		curr_dir= get_dir();
		printf("%s>", curr_dir);
		char *inp=(char *)malloc(sizeof(char)*500);
		fgets(inp, 500, stdin);
		inp[strlen(inp)-1]= '\0';
		if(strlen(inp)==0)
			continue;
		char *in[10];
		for(int i=0;i<10;i++)
			in[i]=(char *)malloc(sizeof(char)*500);
		
		int len= parse_inp(inp, in);
		if(strcasecmp(in[0], "cd")==0)
		{
			if(in[1]==NULL||strcasecmp(in[1], "~")==0)
				change_dir("/home/mansi");
			else if(strcasecmp(in[1], "..")==0)
			{	
				int x;
				for(int i=strlen(curr_dir)-1; i>=0; i--)
				{
					if(curr_dir[i]=='/')
					{
						x=i; 
						break;
					}
				}
				char *new_dir=(char *)malloc(sizeof(char)*500);
				for(int i=0; i<x; i++)
					new_dir[i]= curr_dir[i];
				change_dir(new_dir);
			}
			else if(strcasecmp(in[1], "-")==0)
			{	
				if(prev_dir==NULL)
					printf("Old directory not set\n");
				else
				{
					change_dir(prev_dir);
				}
			}
			else
				change_dir(in[1]);
		}
		else if(strcasecmp(in[0], "pushd")==0)
		{
			if(in[1]==NULL)
				printf("Enter any directory");
			else
				push(in[1]);
		}
		else if(strcasecmp(in[0], "popd")==0)
			pop();
		else if(strcasecmp(in[0], "dirs")==0)
			print_dir();
		else if(strcasecmp(in[0], "path")==0)
		{
			if(in[1]==NULL)
				disp_path();
			else if(strcasecmp(in[1], "+")==0&&in[2]!=NULL)
				add_path(in[2]);
			else if(strcasecmp(in[1], "-")==0&&in[2]!=NULL)
				del_path(in[2]);
			else
				printf("Invalid command\n");
		}
		else if(strcasecmp(in[0], "exit")==0)
		{
			printf("----------Goodbye----------\n");
			return 0;
		}
		else
		{
			int r;
			int f2= open (in[0], O_RDONLY);
			if(f2>=0)
			{
				r=create_process(in,in[0]);
			}
			if(f2<0)
				r=-1;
		
			if(r==-1)
			{	
			Node *temp;
			char *search_path=(char *)malloc(sizeof(char)*500);
			temp= path;
			int f=0;
			while(temp!=NULL)
			{
				strcpy(search_path, temp->p);
				strcat(search_path, "/");
				strcat(search_path, in[0]);
				int f1= open (search_path, O_RDONLY);
				if(f1>=0)
				{
				create_process(in,search_path);
				f=1;
				break;
				}
				temp= temp->next;
			}
			if(f==0)
				printf("%s: command not found\n",in[0]);
			}
		}
	}
	return 0;
}
int create_process(char *in[],char *search_path)
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
		
		return(execvp(search_path,in));
		//printf("%d\n",execvp(search_path,in));
	}

	else
	{
		wait(NULL);
	}
return 0;
}

char* get_dir()
{
	char *curr=(char *)malloc(sizeof(char)*500);
	getcwd(curr, 500);
	return curr;
}
int check_valid(char *p)
{
	DIR* dir = opendir(p);
	if (dir!=NULL)
	{
		closedir(dir);
		return 1;
	}
	else return -1;
}
int change_dir(char *p)
{
	if(check_valid(p)==1)
	{
		chdir(p);
		if(prev_dir==NULL&&strcasecmp(curr_dir,p)!=0)
			prev_dir=(char *)malloc(sizeof(char)*500);
		if(prev_dir!=NULL&&strcasecmp(curr_dir,p)!=0)
			strcpy(prev_dir,curr_dir); 		
		return 0;
    }
	else
	{
		printf("Invalid path\n");
		return -1;
	}
}
int push(char *p)
{
	if(check_valid(p)==1)
	{
	if(top==99)
		return -1;
	top=top+1;
	stack[top]=(char *)malloc(sizeof(char)*500);
	strcpy(stack[top], curr_dir);
	change_dir(p);
	}
	else
		printf("Invalid path\n");
	
	return 0;
}
int pop()
{
	if(top==-1)
	{
		printf("No directory stored in stack\n");
		return -1;
	}
	change_dir(stack[top]);
	top--;
}
int print_dir()
{
	if(top==-1)
	{
		printf("No directory stored in stack\n");
		return -1;
	}
	printf("Contents of stack:\n");
	for(int i=0; i<=top; i++)
		printf("%s\n", stack[i]);
	return 0;
}

int add_path(char *new_p)
{
	if(check_valid(new_p)==1)
	{
		Node *new, *temp;
		new= (Node *) malloc(sizeof(Node));
		new->next= (Node *) malloc(sizeof(Node));
		temp= (Node *) malloc(sizeof(Node));
		new->p=(char *)malloc(sizeof(char)*500);
		strcpy(new->p,new_p); 
	    new->next = NULL;
	    if(path==NULL)
	    {
		path=new;
	    	printf("Path added successfully\n");
		    return 0;
	    }
	    temp= path;
	    while(temp->next!=NULL&&temp->p!=new->p)
	    	temp= temp->next;
	    if(strcasecmp(temp->p, new->p)==0)
	    {
		printf("Path already exists\n");
		return 0;
	    }
	    temp->next= new;
	    printf("Path added successfully\n");
	}
	else
	{
		printf("Invalid path\n");
			
	}
    return 0;
}

int del_path(char *del_p)
{
	if(check_valid(del_p)==1)
	{

		Node *prev=NULL, *temp;
		temp= (Node *) malloc(sizeof(Node));
		temp= path;
		if(temp==NULL)
		{
			printf("Path variable is empty\n");
			return 0;
		}
		while(temp!=NULL)
		{
		
			if(strcasecmp(del_p, temp->p)==0)
			{
		 		if(prev!= NULL)
		 			prev->next= temp->next;
		 		else
		 			path= temp->next;
		 		free(temp);
		 		printf("Path successfully deleted\n");
		 		return 0;
			}
			prev= (Node *) malloc(sizeof(Node));
			prev= temp;
			temp= temp->next;
		}
		printf("Path doesn't exist\n");
	}
	else
		printf("Invalid path\n");
}


int disp_path()
{
	Node *temp;
	//temp= (Node *) malloc(sizeof(Node));
	temp= path;
	if(temp==NULL)
	{
		printf("Path variable is empty\n");
		return 0;
	}
	printf("All paths are: \n");
	while(temp!=NULL)
	{
		printf("%s\n",temp->p);
		temp= temp->next;
	}
}
int parse_inp(char *inp, char *in[])
{
	int k=0, j=0;
	for(int i=0;i<strlen(inp);i++)
	{     
        if(inp[i]==' ')
        {
            in[k][j]='\0';
            j=0;
            k++;
        }
        else
            in[k][j++]=inp[i];    
    }
    in[k][j]='\0';
    in[k+1]=NULL;
    return k+1;
}
