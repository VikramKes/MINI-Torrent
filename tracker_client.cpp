#include<cstdio>
#include<stdlib.h>
#include<unistd.h>
#include<cstring>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<iostream>
#include<arpa/inet.h>
#include <bits/stdc++.h>
#include<pthread.h> 
#include<fstream>
using namespace std;

struct data
{
short i;
//int j;
char str2[1000];
char str3[1000];
long long int val;
char str5[1000];
}; 

struct server_buff
{
 int i;
 char str2[600];
};

string convertToString(char* a, int size) 
{ 
    int i; 
    string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
} 


char *mystrtok(char str[],char delim)
{
//cout<<"tok calling";
	static char *input=NULL;
	if(str!=NULL)
		input=str;
	if(input==NULL)
		return NULL;

	char *output = new char[strlen(input)+1];
	int i;
	for(i=0;input[i]!='\0';i++)
	{
		if(input[i]!=delim)
			output[i]=input[i];
		else
		{
			output[i]='\0';
			input=input+i+1;
			return output;
		}
	}
	//cout<<"ending is near";
	//output[i]='\0';
	input=NULL;
	return output;
}

void *thread_download(void *dummyptr)
{
struct data *y = (struct data *)dummyptr;
char msg[100]="openfile";
strcat(msg," ");
strcat(msg,y->str2);
int network_socket;
network_socket=socket(AF_INET,SOCK_STREAM,0);

struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(y->i);
server_address.sin_addr.s_addr=INADDR_ANY;

int connection_status=connect(network_socket,(struct sockaddr *) &server_address,sizeof(server_address));

if(connection_status==-1)
{
	cout<<"There is error";
}

FILE *fp = fopen ( y->str3,"wb");
//fseek ( fp , 0 , SEEK_END);
//int size=ftell(fp);
//rewind(fp);

send(network_socket,(char *)msg,100,0);
int BUFF_SIZE=8*1024;
long long int size=y->val;
char Buffer[BUFF_SIZE]; 
int n;
while (size>0)
{
	n=recv(network_socket,Buffer,BUFF_SIZE,0);
	fwrite (Buffer , sizeof(char),n,fp);
	cout<<"Sending file"<<endl;
   	memset(Buffer,'\0',BUFF_SIZE);
	size = size - n ;
cout<<size<<endl;
}

fclose ( fp );

}



void *thread_handler_server(void *dummyptr)
{
	fstream fin,fout;
	char res[256]="";
	struct server_buff *y = (struct server_buff *)dummyptr;
cout<<"THREAD HANDLER SERVER";
	//cout<<y->i<<endl;
	//cout<<y->str2<<endl;
	char *store[10]={};
	string vamp[10]={};
	string msg="";
	char *ptr;
	ptr = mystrtok(y->str2,' ');
	store[0]=ptr;
	int i=1;
	while(ptr!=NULL)
	{
		cout<<"ptr: "<<ptr<<endl;
		ptr=mystrtok(NULL,' ');
		store[i]=ptr;
	i++;
	}


	if(strcmp(store[0],"Upload")==0)
	{
		long long int size=0;
		/*FILE *fp=fopen(store[1],"rb");
	
		fseek(fp,0,SEEK_END);
		int size=ftell(fp);
		rewind(fp);*/
		fin.open(store[1],ios::in|ios::binary);
		if(fin.fail())
		{
			cout<<"Unable to open file";
		}

		else if(fin.bad())
		{
			cout<<"Invalid operation perfomed";
		}

		else if(fin.good())
		{
			fin.seekg(0,ios::end);
			size=fin.tellg();
			fin.seekg(0,ios::beg);
		}
		cout<<"SIZE"<<size<<endl;
		strcat(res,to_string(size).c_str());	
	}

	else if(strcmp(store[0],"Download")==0)
	{
		struct data *down_data = (struct data *)malloc(sizeof(struct data)); 
		
		pthread_t thread_down;

		down_data->i=atoi(store[1]);
		strcpy(down_data->str2,store[2]);
		strcpy(down_data->str3,store[3]);
		down_data->val=atoi(store[4]);
		strcpy(down_data->str5,"Download");
		pthread_create(&thread_down,NULL,thread_download,(void *)down_data);
		pthread_join(thread_down,NULL);
		char temp[100]="msg to you";
		strcat(res,temp);
	}


	else if(strcmp(store[0],"openfile")==0)
	{
		//send(network_socket,&size,sizeof(size),0);
		//int BUFF_SIZE=512;
		//char Buffer[BUFF_SIZE]; 
		FILE *fp = fopen ( store[1],"rb");
		fseek ( fp , 0 , SEEK_END);
		int size=ftell(fp);
		rewind(fp);
		int BUFF_SIZE=8*1024;
		char Buffer[BUFF_SIZE]; 
		int n;
		while (size > 0)
		{
		n=fread(Buffer,sizeof(char),BUFF_SIZE,fp);
		//send(y->i,&size,sizeof(size),0);
		send(y->i,Buffer,n,0);
   		memset(Buffer,'\0',BUFF_SIZE);
		size = size - n ;
		cout<<size<<endl;
		}

		fclose ( fp );
}
	send(y->i,(char *)res,sizeof(res),0);

fin.close();
}




void *thread_server(void *dummyptr)
{
struct data *y = (struct data *)dummyptr;

int server_client_socket=socket(AF_INET,SOCK_STREAM,0);

pthread_t thread_upload[5];

struct sockaddr_in server_address;

server_address.sin_family=AF_INET;

server_address.sin_port=htons(y->i);

server_address.sin_addr.s_addr=INADDR_ANY;

bind(server_client_socket,(struct sockaddr *) &server_address,sizeof(server_address));

listen(server_client_socket,10);

int val;
int server_socket;

int i=0;


while(true)
{
cout<<"call of i:"<<i<<endl;
server_socket=accept(server_client_socket,NULL,NULL);

char buff[256]="";

int bo=recv(server_socket,(char *)buff,sizeof(buff),0);

struct server_buff *args1 =(struct server_buff *)malloc(sizeof(struct server_buff));

args1->i=server_socket;

strcpy(args1->str2,buff);



pthread_create(&thread_upload[i],NULL,thread_handler_server,args1);


i++;

pthread_join(thread_upload[i],NULL);
//i++;
//pthread_detach(thread_id);
//close(client_socket);
}

close(server_socket);

}

void *thread_client(void *dummyptr)
{

struct data *y = (struct data *)dummyptr;

cout<<"what I got is "<<y->i<<endl;
int network_socket=socket(AF_INET,SOCK_STREAM,0);

struct sockaddr_in server_address;

server_address.sin_family=AF_INET;

server_address.sin_port=htons(8083);

//cout << thread_sock->val1;

server_address.sin_addr.s_addr=INADDR_ANY;

int connection_status=connect(network_socket,(struct sockaddr *) &server_address,sizeof(server_address));

if(connection_status==-1)
{
cout<<"Note able to connect";
exit(1);
}
while(1)
{
string str="";
getline(cin,str);

if(str.substr(0,10)=="login_user")
{
	str+=" ";
	str+=to_string(y->i);
	str+=" ";
	str+=y->str2;
}
char inp[60]="";

int i=0;
while(i<str.length())
{
	inp[i]=str[i];
	i++;
}
inp[i]='\0';

char *res=(char *)inp;
cout<<"res "<<res<<endl;
cout<<"value of network_socket"<<network_socket<<endl;
send(network_socket,(char *)res,strlen(inp),0);
char server_response[256];
memset(server_response,0,sizeof(server_response));
recv(network_socket,server_response,sizeof(server_response),0);
cout<<server_response<<endl;
}
//close(network_socket);
}


 
int main(int argc,char *argv[])
{
pthread_t thread_id1;
pthread_t thread_id2;
char val[100]="";

struct data *num = (struct data *)malloc(sizeof(struct data));
cout<<"argv0 "<<argv[1]<<" "<<"argv1 "<<argv[2]<<endl;
int temp=atoi(argv[1]);
num->i=(short)temp;
//num->j=atoi(argv[2]);
strcpy(num->str2,argv[2]);

//cout<<num->i<<endl;
//cout<<num->str2<<endl;
pthread_create(&thread_id1,NULL,thread_client,num);
pthread_create(&thread_id2,NULL,thread_server,num);
pthread_join(thread_id1,NULL);
pthread_join(thread_id2,NULL);
//pthread_detach(thread_id1);
return 0;
}


