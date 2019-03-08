/* A simple server in the internet domain using TCP
   The port number is passed as an argument
   This version runs forever, forking off a separate
   process for each connection
*/
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/lexical_cast.hpp>
/*
string create_html_output_for_binary(const string &full_path)
{
    std::vector<char> buffer;

    //... other code here
    const char* file_name = full_path.c_str();
    FILE* file_stream = fopen(file_name, "rb");
    string file_str;
    size_t file_size;
    if(file_stream != nullptr)
    {
        fseek(file_stream, 0, SEEK_END);
        long file_length = ftell(file_stream);
        rewind(file_stream);

        buffer.resize(file_length);

        file_size = fread(&buffer[0], 1, file_length, file_stream);
    }

    // .... other code here
}*/
using namespace std;
using boost::lexical_cast;

string output(const string full_path,long *file_length)
{
	*file_length=0;
	ifstream input(full_path);
	if(input.is_open()){
	    string str;
	    while(!input.eof()){
          string data;
          getline(input,data);
          str = lexical_cast<string>(str)+lexical_cast<string>("\n")+lexical_cast<string>(data);
    }
	*file_length=str.size();
	input.close();
  return str;}
	string str="<html>\n<body>\njames_bond\n</body>\n</html>";
	*file_length=str.size();
return str;
}
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
void send_data (int sock,string str)
{	 int n;
	 n = write(sock,str.c_str(),str.size());
   if (n < 0) error("ERROR writing to socket");
}
string read_data (int sock)
{
   int n;
   char buffer[30000];
   bzero(buffer,30000);
   n = read(sock,buffer,29999);
	 string request;
	 request=buffer;
	 if (n < 0) error("ERROR reading from socket");
	 cout<<request;
	 bzero(buffer,30000);
	 string req_type=request.substr(0,3);
	 //cout<<req_type<<"*****************\n";
	 if(req_type=="GET")
	 {
		  int i;
		  for(i=4;i<request.size();i++)
		  if(request[i]==' ')
		  break;
			string file=request.substr(4,i-4);
			if(file=="/")
			{file="index.html";
			return file;}
			file=request.substr(5,i-5);
			//cout<<file<<"**************\n";
			return file;
	 }
	 return 0;
}
void dostuff (int sock)
{	int start;
	string file=read_data(sock);
	if(file[0]=='0')
	error("Wrong_request");
	long file_length=0;
	for(start=0;start<file.size();start++)
	if(file[start]=='.')
	break;
	string extension=file.substr(start+1,file.size()-start-1);
	//cout<<extension<<"************\n";
	string str3,str2=output(file,&file_length),content_type="text/html";
	//cout<<str2<<endl;
	string end_line="\n\n";
	if(extension=="jpg")
	{content_type="image/jpg";
   end_line="\n";}
	else if(extension=="png")
	{content_type="image/png";
	 end_line="\n";}
	else if(extension=="jpeg")
	{content_type="image/jpeg";
	 end_line="\n";}
	 else if(extension=="css")
	 {content_type="stylesheet";}
	 else if(extension=="js")
	 {content_type="script";}
	str3 =lexical_cast<string>("HTTP/1.1 200 OK\nserver: linuxbox\nContent-Type: ")+lexical_cast<string>(content_type);
	str3=lexical_cast<string>(str3)+lexical_cast<string>("\nContent-Length: ")+lexical_cast<string>(file_length)+lexical_cast<string>(end_line)+lexical_cast<string>(str2);
	send_data(sock,str3);
}
int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0)
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
         dostuff(newsockfd);
		     exit(0);
         }
         else close(newsockfd);
     }
     close(sockfd);
     return 0;
}
