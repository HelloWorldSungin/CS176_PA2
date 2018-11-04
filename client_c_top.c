/*
client_c_tcp.c
Sung In Kim
CS176A
Developed based on the source code provide by http://www.linuxhowtos.org/data/6/client.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
  // source code start
  // =================================================================
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    /*
    char *h_name
    This is the “official” name of the host.

    char **h_aliases
    These are alternative names for the host, represented as a null-terminated vector of strings.

    int h_addrtype
    This is the host address type;

    int h_length
    This is the length, in bytes, of each address.

    char **h_addr_list
    This is the vector of addresses for the host. (Recall that the host might be connected to multiple networks and have different addresses on each one.) The vector is terminated by a null pointer.

    char *h_addr
    This is a synonym for h_addr_list[0]; in other words, it is the first host address.
    */
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);                         // Convert string argument argv[2] (port number) to an integer type int
    sockfd = socket(AF_INET, SOCK_STREAM, 0);       // creates an unbounded socket in the communications domain.
                                                    // returns a file descriptor that can be used in later function calls that operate on sockets
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);                // set *h_name in the hostent struct of server
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));  // place sizeof(serv_addr) number of 0 to the address of serv_addr
    serv_addr.sin_family = AF_INET;                 // assigning the domain type for the socket
    bcopy((char *)server->h_addr,                   // copy server->h_length bytes from the area pointed to by h_addr to area pointed to by s_addr
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);             // converts the unsigned short int hostshort from host byte order to network byte order
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) // attempt to make a connection on a socket, similar with bind function
        error("ERROR connecting");


    printf("Enter string: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);                        // Read char from stdin and stores them as a C string into buffer until 127-1 characters have been
                                                    // read or either a newline or the end-of-file is reached
    // write to enter string
    n = write(sockfd,buffer,strlen(buffer));        // attempt to write strlen(buffer) bytes from the buffer pointed to by buffer to the file associated with
                                                     // the open file descriptor(sockfd)
    if (n < 0) error("ERROR writing to socket");
  // =================================================================
  // source code end
// keep listening to the server while socket connection is not closed
    while (!(n<=0)) {
        bzero(buffer,256);                              // clear the buffer with 0
        // read from the Server
        n = read(sockfd,buffer,255);                    // read from a file. attempt to read 127 bytes from the file associated with the open file
                                                    // descriptor(sockfd), into the buffer pointed to by buffer. The behavior of multiple concurrent reads on
                                                    // the same pipe, FIFO, or terminal device is unspecified
        if (n < 0) error("ERROR reading from socket");

        if (strlen(buffer) != 0)
        printf("From server: %s\n",buffer);

        // write to the Server
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
    }

    return 0;
}
