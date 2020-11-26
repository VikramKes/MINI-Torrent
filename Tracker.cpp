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
#include<map>
#include<list>
#include<queue>
#include<map>
#include<pthread.h>
#include<fstream>
#include<time.h>
#include <bits/stdc++.h> 
using namespace std;
struct data
{
int val;
char str2[60];
};




struct group
{
	string group_id;
	string groupowner;
	vector <string> client_num;
	vector <string> pending_client;
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

struct file_info
{
string file_path;
size_t file_size;
string group_id;
string groupowner;
int hash;
vector<string> u_id;
};


unordered_map<string,string> umap; 
unordered_map<string,string> name;
unordered_map<string,struct group *> grouping;
unordered_map<string,vector<pair<string,struct file_info *>>> file_det;
unordered_map<string,pair<string,int>> user_info;
unordered_map<string,pair<string,short>> user_config;
class trackerrecoder
{
public:
	string user_id;
	string session_val;
	string group_verification;
	string listreq;
	string listgrp;
	string msg; 

		trackerrecoder()
		{
			user_id="";
			session_val="";
			group_verification="";
			listgrp="";
		}

		int randomgen()
		{
			srand(time(NULL));
			return (rand()%10000);
		}

string revfilename(string name)
	{
			string tempmsg="";
			int len=name.length();
		 int i;
		 for(i=len-1;i>=0;i--)
		 {
		 	tempmsg+=name[i];
		 }
		 //msg[i]='/0';
		 return tempmsg;
		}

string filepathextract(string name)
		{
			string valmsg="";
			int len=name.length();
			int t=len-1;
			int i;
			for(i=t;name[i]!='/';i--)
			{
				valmsg+=name[i];
			}
			//msg[i]='/0';
			cout<<valmsg<<endl;
		string fname=revfilename(valmsg);
		cout<<"REVERSE "<<fname<<endl;
		return fname;
		}

		string add_user(string user,string passwd)
		{
			string session_key="";
			if(umap.count(user)==false)
			{
			umap.insert(make_pair(user,passwd));
			cout<<"username:"<<user<<endl;
			cout<<"Password:"<<passwd<<endl;
			user_id=user;
			name.insert(make_pair(user,session_key));
			msg="the user is successfully created";
			}
			//else if()
		return msg;
		}


		string login_me(string usr,string pass,string port,string ip)
		{
			string msg="";
			if(umap.count(usr)==true && pass==umap[usr])
			{
				string session_key="";	
				session_key+=to_string(randomgen());
				name[usr]=session_key;
				string ip_num=ip;
				int temp_port=stoi(port);
				short port_num=(short)temp_port;
				name.insert(make_pair(usr,session_key));
				user_config.insert(make_pair(usr,make_pair(ip_num,port_num)));
				cout<<endl<<"Sesssion Created"<<endl;
				//flag=true;
				session_val=session_key;
				//cout<<"value of flag"<<flag<<endl;
			 msg="Session Created successfully";
			return msg;
			}

			else
			{
				msg="No Session";
				return msg;
			}

		}

		string join_group(string group_val)
		{
			if(grouping.count(group_val)==true)
			{
				if(session_val==name[user_id])
				{
				vector<string>::iterator it;	
				vector<string>::iterator is;
				it=find(grouping[group_val]->pending_client.begin(), grouping[group_val]->pending_client.end(), user_id);
				is=find(grouping[group_val]->client_num.begin(), grouping[group_val]->client_num.end(), user_id);
				if((it!=grouping[group_val]->pending_client.end()) || (is!=grouping[group_val]->client_num.end()))
				{
					cout<<"user is already being part of group or it's on hold for pending request";
				}
				else
				{
				cout<<"yes it exist"<<endl;
				grouping[group_val]->pending_client.push_back(user_id);
				msg="User Joined";
				}
				}

				else
				{
					cout<<"No such user exist"<<endl;
					msg="No user exist";
				}
			}

			else 
			{
				cout<<"Group doesn't exist"<<endl;
				msg="Group doesn't exist";
			}
		return msg;
		}


		string create_grp(string group_val)
		{

			cout<<"INSIDE CREATE GROUP"<<endl;
			if(grouping.count(group_val)==false)
			{
			if(session_val==name[user_id] && session_val!="")
			{
				struct group *num = (struct group *)malloc(sizeof(struct group));
				num->group_id=group_val;
				num->groupowner=user_id;
			  	cout<<"Yes it was me"<<endl;
			  	grouping.insert(make_pair(group_val,num));
			  	cout<<"Value of Group"<<grouping[group_val]->groupowner<<endl;
				group_verification=grouping[group_val]->group_id;
			 msg="Created group";
			 join_group(group_val);
			}

			else
			{
				cout<<"No yet created group"<<endl;
				msg="Wasn't able to create group";
			}
		}

		else
		{
			msg="Group Already Exist";
			
		}
		return msg;
		}

		
		string leave_group(string group_val)
		{
			vector<string>::iterator it; 
			if(grouping.count(group_val)==true)
			{
				if(session_val==name[user_id])
				{
				cout<<"yes it exist"<<endl;
				
				 it=find(grouping[group_val]->pending_client.begin(), grouping[group_val]->pending_client.end(), user_id); 
			    if (it!=grouping[group_val]->pending_client.end()) 
			    { 
			    	cout<<"Removed"<<endl;
			        grouping[group_val]->pending_client.erase(it); 
			       msg="User Removed Successfully";
			    }

			    else
			    {
			    	cout<<"Note a part of group";
			    msg="User is not part of group";	
			    }

				}

				else
				{
					cout<<"No such user exist"<<endl;
				msg="No User Exist";
				}
			
			}

			else 
			{
				cout<<"Group doesn't exist"<<endl;
				msg="No Such Group Exist";
			}	
			return msg;
		}


		string list_request(string group_val)
		{
			if(session_val==name[user_id] && session_val!="")
			{
				if(grouping.count(group_val)==true)
				{
				for(vector<string>::iterator p=grouping[group_val]->pending_client.begin();p!=grouping[group_val]->pending_client.end();p++)
				{
					cout<<*p<<" ";
					listreq+=(*p);
					listreq+=" ";
				}
				msg=listreq;
				}
				else
				{
					cout<<"Not a valid group id";
				msg="Not a vaild group";
				}
			}

			else
			{
				cout<<"Invalid user trying to access the request";
				msg="Invalid User";
			}
		return msg;
		}

		string accept_request(string group_val,string user_re)
		{

			if(grouping[group_val]->groupowner==user_id)
			{
			if(session_val==name[user_id] && session_val!="")
			{
				if(grouping.count(group_val)==true)
				{
			auto it=find(grouping[group_val]->pending_client.begin(), grouping[group_val]->pending_client.end(), user_re); 
			    if (it!=grouping[group_val]->pending_client.end()) 
			    { 
			    	cout<<"Removed"<<endl;  
			      	grouping[group_val]->pending_client.erase(it);
			      	grouping[group_val]->client_num.push_back(user_re);
			      	msg="Accepted Request";
			    }
			    else
				{
					cout<<"User doesn't belong to pending part";
					msg="User doesn't belong to the group";
				}
				}
				else
				{
					cout<<"No such group_id exist";
					msg="No such group exist";
				}	
		}

		else
		{
			cout<<"Invalid user trying to access the request list";
			msg="Invalid User exist here";
		}
		}

		else
		{
			cout<<"Not a authenticate user make such query";
			msg="Not a Authenticated User";			
		}
		return msg;
		}

		string list_group()
		{

		if(session_val==name[user_id] && session_val!="")
		{
		for(auto it=name.cbegin();it!=name.cend();++it)
		{
			cout<<(*it).first<<" ";
			listgrp+=(*it).first;
			listgrp+=" ";
		}
		msg=listgrp;
		}

		else 
		{
			cout<<"Invalid user"<<endl;
			msg="Invalid User";
		}
		return msg;
		}

		/*void list_files(string group_val)
		{


		}*/

		string upload_file(string file_path,string group_val)
		{
			bool flag_pres=false;
			//unordered_map<string,vector<pair<string,struct file_info *>>> file_det;
			if(session_val==name[user_id] && session_val!="")
			{

				int network_socket=socket(AF_INET,SOCK_STREAM,0);

				struct sockaddr_in server_address;

				server_address.sin_family=AF_INET;

				server_address.sin_port=htons(user_config[user_id].second);

				//cout << thread_sock->val1;

				server_address.sin_addr.s_addr=INADDR_ANY;

				int connection_status=connect(network_socket,(struct sockaddr *) &server_address,sizeof(server_address));

				if(connection_status==-1)
				{
				cout<<"Note able to connect";
				exit(1);
				}
				char arr[256]="Upload";
				strcat(arr," ");
				strcat(arr,file_path.c_str());	
				send(network_socket,(char *)arr,strlen(arr),0);
				char server_response[256];
				memset(server_response,0,sizeof(server_response));
				recv(network_socket,server_response,sizeof(server_response),0);
				cout<<"Recived Upload Server"<<server_response<<endl;
				struct file_info *res = (struct file_info *)malloc(sizeof(struct file_info));	
				//file_det[file_path].first="vikram";
				string str_temp=filepathextract(file_path);
				std::vector<pair<string,struct file_info *>> vec(file_det[group_val].begin(),file_det[group_val].begin());
				int j=0;
				vector<pair<string,struct file_info *>>::iterator i;
				vector<pair<string,struct file_info *>>::iterator temp;
				for(i=file_det[group_val].begin();i!=file_det[group_val].end();i++)
				{
						if(str_temp==(*i).first)
						{
							temp=i;
							//(*i).second->u_id.push_back(user_id);
							flag_pres=true;
							break;
						}
					j++;

				}

				if(flag_pres==true)
				{
					cout<<"USER_ID"<<user_id<<endl;

					(*temp).second->u_id.push_back(user_id);
					//res->u_id.push_back(user_id);
					cout<<"Condition 1"<<endl;
				}

				else if(flag_pres==false)
				{
					cout<<"USER ID"<<user_id<<endl;
					res->group_id=group_val;
				res->groupowner=grouping[group_val]->groupowner;
				res->hash=0;
				res->file_size=atoi(server_response);
				//res->file_name=filepathextract(file_path);
				
				res->file_path=file_path;
				res->u_id.push_back(user_id);
				vec.push_back({str_temp,res});
				cout<<"BUNCH OF DATA TO INSERT"<<endl;

				//file_det[group_val].second=res;
					//file_det[group_val].push_back({str_temp,res});
					//file_det[group_val]=file_det[group_val].push_back({str_temp,res});			
				file_det[group_val]=vec;
					cout<<"Condition 2"<<endl;

				}

					cout<<"finalans"<<file_det[group_val][0].second->u_id[0]<<endl;
				//intializor();
				msg="Uploaded the file";
			//file_det.insert((make_pair(group_val,vec)));
			}

		/*for(auto j=file_det[group_val].begin();j!=file_det[group_val].end();j++)
		{
			for(int i=0;i<file_det[group_val][j].second->uid.size();i++)
		{
			cout<<file_det[group_val][j]->second->uid[i]<<endl;	
		}
		}*/

		return msg;
	}


	string download_file(string group_val,string file_name,string destn)
	{
		cout<<"download_file function is being called"<<endl;
		bool flag_file=false;
		int req_index=0;
		string found_filename="";
		if(grouping.count(group_val)==true)
		{
			cout<<"YES I AM PART OF GROUP"<<endl;
			auto is=find(grouping[group_val]->client_num.begin(), grouping[group_val]->client_num.end(), user_id);
				if(is!=grouping[group_val]->client_num.end())
				{
					if(session_val==name[user_id] && session_val!="")
					{
						cout<<"YAHA TAK PAHUCH GAYA"<<endl;
						for(auto v1 = file_det[group_val].begin();v1!=file_det[group_val].end();v1++)
						{
							cout<<"me inside for loop"<<v1->first<<endl;
							if(v1->first==file_name)
							{
								cout<<"me inside main condn check"<<endl;
							 	req_index=v1-file_det[group_val].begin();
								found_filename=v1->first;
								flag_file=true;
								break;
							}

						}

						if(flag_file==true)
						{
						int network_socket=socket(AF_INET,SOCK_STREAM,0);

						struct sockaddr_in server_address;

						server_address.sin_family=AF_INET;

						server_address.sin_port=htons(user_config[user_id].second);

						server_address.sin_addr.s_addr=INADDR_ANY;

						int connection_status=connect(network_socket,(struct sockaddr *) &server_address,sizeof(server_address));

						if(connection_status==-1)
						{
							cout<<"Note able to connect";
							exit(1);
						}
						char arr[256]="Download";
						strcat(arr," ");
						for(vector<string>::iterator k=file_det[group_val][req_index].second->u_id.begin();k!=file_det[group_val][req_index].second->u_id.end();k++)
						{
						strcat(arr,(to_string(user_config[*k].second)).c_str());
						strcat(arr," ");
						//char abc[100]="";
						string temp=""; 
						temp+=file_det[group_val][req_index].second->file_path;
						//strcat(abc,temp.c);
						strcat(arr,temp.c_str());
						strcat(arr," ");
						strcat(arr,destn.c_str());
						strcat(arr," ");
						temp="";
						temp+=to_string(file_det[group_val][req_index].second->file_size);
						strcat(arr,temp.c_str());
						}
						cout<<"ARR"<<arr<<endl;
						send(network_socket,(char *)arr,strlen(arr),0);
						char server_response[256];
						memset(server_response,0,sizeof(server_response));
						recv(network_socket,server_response,sizeof(server_response),0);
						cout<<"Recived Upload Server"<<server_response<<endl;
						msg="File exist So Move Further Bro";
					//file_det.insert((make_pair(group_val,vec)));
					}
				else
				{
					cout<<"No Such file exist in the group";
				}
			}
			else
			{
				cout<<"User is not logged in";
				msg="User not exist";
			}
		}
			else
			{
				msg="The user is not part of the group";
			}
	}

	else
	{
		msg="No Such Group Exist";
	}
return msg;
}
};

char *mystrtok(char str[],char delim)
{
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
	output[i]='\0';
	input=NULL;
	return output;
}

void *thread_handler(void *dummyptr)
{
	trackerrecoder tr;
	struct data *res = (struct data *)dummyptr;
	//char *res=(char *)dummyptr;
	while(true)
	{
	cout<<res->val<<endl;
	char *store[7]={};
	string vamp[7]={};
	string msg="";
	char *ptr;
	ptr = mystrtok(res->str2,' ');
	store[0]=ptr;
	int i=1;
	while(ptr!=NULL)
	{
		//cout<<"ptr"<<ptr;
		ptr=mystrtok(NULL,' ');
		store[i]=ptr;
	i++;
	}

	int q=0;
	while(q<(i-1))
	{
		int size=strlen(store[q]);
		vamp[q]=convertToString(store[q],size);
		cout<<"tokenise data:"<<vamp[q]<<endl;
		q++;
	}

	if(vamp[0]=="create_user")
	{
	 msg=tr.add_user(vamp[1],vamp[2]);
	}

	else if(vamp[0]=="login_user")
	{
	msg=tr.login_me(vamp[1],vamp[2],vamp[3],vamp[4]);
	
	}

	else if(vamp[0]=="create_group")
	{
		//cout<<"inside"<<endl;
		msg=tr.create_grp(vamp[1]);
		//cout<<"outside"<<endl;
	}

	else if(vamp[0]=="join_group")
	{
		msg=tr.join_group(vamp[1]);
	}

	else if(vamp[0]=="leave_group")
	{
		msg=tr.leave_group(vamp[1]);
	}

	else if(vamp[0]=="list_request")
	{
		msg=tr.list_request(vamp[1]);
	}

	else if(vamp[0]=="accept_request")
	{
		msg=tr.accept_request(vamp[1],vamp[2]);
	}

	else if(vamp[0]=="list_groups")
	{
		msg=tr.list_group();
	}
	/*else if(vamp[0]=="list_files")
	{
		msg=tr.list_files(vamp[1]);
	}*/
	else if(vamp[0]=="upload_file")
	{
		msg=tr.upload_file(vamp[1],vamp[2]);
	}

	else if(vamp[0]=="download_file")
	{
		msg=tr.download_file(vamp[1],vamp[2],vamp[3]);
	}

	char inp[60]="";
	int l=0;
	while(l<msg.length())
	{
		inp[l]=msg[l];
		l++;
	}
	inp[l]='\0';
	send(res->val,(char *)inp,strlen(inp),0);
	memset(res->str2,0,sizeof(res->str2));
	int x=recv(res->val,(char *)res->str2,sizeof(res->str2),0);
	}
}



void *server_func(void *dummyptr)
{
int server_socket=socket(AF_INET,SOCK_STREAM,0);
pthread_t thread_id[5];
struct sockaddr_in server_address;
server_address 	.sin_family=AF_INET;
server_address.sin_port=htons(8083);
server_address.sin_addr.s_addr=INADDR_ANY;

bind(server_socket,(struct sockaddr *) &server_address,sizeof(server_address));

listen(server_socket,10);

int val;
int client_socket;

int i=0;
while(true)
{
cout<<"value of i:"<<i<<endl;

client_socket=accept(server_socket,NULL,NULL);

char buff[60]="";
int bo=recv(client_socket,(char *)buff,sizeof(buff),0);
struct data *args1 =(struct data *)malloc(sizeof(struct data));
args1->val=client_socket;
strcpy(args1->str2,buff);
pthread_create(&thread_id[i],NULL,thread_handler,args1);
i++;
pthread_join(thread_id[i],NULL);
//i++;
//pthread_detach(thread_id);
//close(client_socket);
}
close(client_socket);
}

int main()
{

pthread_t thread_server;
pthread_t thread_client;
pthread_create(&thread_server,NULL,server_func,NULL);
//pthread_create(&thread_client,NULL,client_func,NULL);
//pthread_detach(thread_server);
pthread_join(thread_server,NULL);
return 0;
} 