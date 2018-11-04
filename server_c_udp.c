/*
server_c_udp.c
Sung In Kim
CS176A
Developed based on the source code provide by http://www.linuxhowtos.org/data/6/server_udp.c
*/
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// function return true : if the buffer only contained digits
// function return false : if the buffer contain another char other than digits
bool is_all_digit(char *buffer) {
    int buffer_length = strlen(buffer);
    // Check if the the char is digit only
    for (int i = 0; i < buffer_length; i++) {
       if (isalpha(buffer[i]) != 0) {
        return false;
       } 
    }
    return true;
}

// input buffer has to be all digits!
void addition_calculation(char *buffer) {
    int int_sum = 0;
    int int_buffer;

    // add all the sum in the digits 
    for (int i = 0; i < strlen(buffer); i++) {
        if (isdigit(buffer[i])) {
            int_buffer = buffer[i] - '0';
            int_sum = int_buffer + int_sum;
        }
    }     

    bzero(buffer,1024);              // zeroing out the buffer
    sprintf(buffer, "%d", int_sum); // convert int to char and store it in the buffer
}


int main(int argc, char *argv[])
{
  // source code start
  // =================================================================
   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];
   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
  // =================================================================
  // source code end
   while (1) {

       bzero(buf, 1024);  // clearing the buffer 
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");

        // check if the input string from the client is all number
        if (!is_all_digit(buf)) {
          n = sendto(sock,"Sorry, cannot compute!",22,
                  0,(struct sockaddr *)&from,fromlen);
          if (n  < 0) error("sendto");
          break;
        } 

        // additions calculation
        addition_calculation(buf);
        // send it back to the client
       n = sendto(sock,buf,17,
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");
   }
   return 0;
 }
