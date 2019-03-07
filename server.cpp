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
//ifstream input1(full_path); //put your program together with this file in the same folder.
*file_length=0;
	ifstream input(full_path);	
	if(input.is_open()){
	    string str;
	    while(!input.eof()){
          string number;
          getline(input,number); //read number
           //cout<<number<<endl; //print it out
          str = lexical_cast<string>(str)+lexical_cast<string>("\n")+lexical_cast<string>(number);
    }
	//cout<<str;
	*file_length=str.size();
	input1.close();
	//cout<<str.size()<<"########";
   return str;}	
    
return "<html><body>james_bond</body></html>";
}
//void dostuff(int,string*); /* function prototype */

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
void dostuff (int sock,string str)
{
   int n;
   char buffer[30000];
   bzero(buffer,30000);
   n = read(sock,buffer,29999);
   if (n < 0) error("ERROR reading from socket");
   printf("Here is the message: %s\n",buffer);
   //const char *buffer=str.c_str();
   n = write(sock,str.c_str(),str.size());
   if (n < 0) error("ERROR writing to socket");
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
            long file_length=0;
		string str3,str2=output("index.html",&file_length);
		str3 =lexical_cast<string>("HTTP/1.1 200 OK\nDate: Thu, 7 Mar 2019 12:00:04 GMT\nserver: linuxbox\nContent-Type: text/html;charset=UTF-8\nContent-Length: "+lexical_cast<string>(file_length)+lexical_cast<string>("\n\n")+lexical_cast<string>(str2);
		cout<<str3;            
            dostuff(newsockfd,str3);
		
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}
