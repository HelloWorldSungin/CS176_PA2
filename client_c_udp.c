/*
server_c_udp.c
Sung In Kim
CS176A
Developed based on the source code provide by http://www.linuxhowtos.org/data/6/client_udp.c
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void error(const char *);

// function return true : if the buffer has more than one digit
// function return false : if the buffer only has one digit or if it is empty
bool is_one_digit(char *buffer) {
    // check if the next char is empty 
    if (((buffer[1]) == '\0') && (buffer[0] != '\0')) {
        return true;
    }

    return false;
}

int main(int argc, char *argv[])
{
  // source code start
  // =================================================================
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256];
   
   if (argc != 3) { printf("Usage: server port\n");
                    exit(1);
   }
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr, 
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   printf("Enter string: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);
    // SENDING THE ENTER STRING
    n=sendto(sock,buffer,
             strlen(buffer),0,(const struct sockaddr *)&server,length);
    if (n < 0) error("Sendto");     
  // =================================================================
  // source code end
    while(1) {
    bzero(buffer, 256);
    // Revive from the Server
    n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from, &length);

    write(1, "From server: ", 13);
    write(1, buffer, n);  
    printf("\n");
    // check if the message is the "Sorry, cannot compute!"
    if (isalpha(buffer[0])) {
      break;
    }          

    // check if the buffer has only one digit left, if so clock the socket
    if (is_one_digit(buffer)) {
      break;
    }

    //Send the buffer back to the server
    n = sendto(sock,buffer,
             strlen(buffer),0,(const struct sockaddr *)&server,length);
    if (n < 0) error("Sendto");
  }
   close(sock);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}