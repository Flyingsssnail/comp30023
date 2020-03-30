/* A modified client program for assignment1

   To compile: gcc client.c -o client

   To run: start the server, then the client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define REQUEST_SIZE 4096
#define DATA_SIZE 100000
#define DEBUG
#define TEST_SERVER_PORT 80

int main(int argc, char ** argv)
{
    int sockfd, ret;
    struct sockaddr_in serv_addr;
    struct hostent * server;

    char req_data[REQUEST_SIZE];
    /* No server response will be longer than 100,000 bytes. */
    char res_data[DATA_SIZE];

    /* file pointer to hold reference to the file */
    FILE *fptr;

    if (argc != 2)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }


    /* Translate host name into peer's IP address ;
     * This is name translation service by the operating system
     */
    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    /* Building data structures for socket */

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(TEST_SERVER_PORT);

    /* Create TCP socket -- active open
    * Preliminary steps: Setup: creation of active open socket
    */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("ERROR Create Socket Failed!");
        exit(0);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(0);
    }

    #ifdef DEBUG
    fprintf(stdout, "Socket connection established\n");
    #endif


    /* construct a query */
    // while(1) {

    /* initilize Request
    */
    bzero(req_data, REQUEST_SIZE);
    strcat(req_data, "GET / HTTP/1.1\r\n");
    strcat(req_data, "User-agent: leic5\r\n");
    strcat(req_data, "Host: ");
    strcat(req_data, argv[1]);
    strcat(req_data, "\r\n");
    strcat(req_data, "Connection: keep-alive\r\n");
    strcat(req_data, "\r\n");

    #ifdef DEBUG
    fprintf(stdout, "initialization of request socket completed\n");
    #endif
    /* Send Request
    */
    ret = write(sockfd, req_data, strlen(req_data));

    if (ret < 0)
    {
        perror("ERROR writing to socket");
        exit(0);
    }

    bzero(res_data, DATA_SIZE);
        /* Receive Response
        */
        #ifdef DEBUG
        fprintf(stdout, "Beginning to receive response\n");
        #endif
        ret = read(sockfd, res_str, DATA_SIZE);

        if (ret < 0)
        {
            perror("ERROR reading from socket");
            close(sockfd);
            exit(0);
        }

        #ifdef DEBUG
        fprintf(stdout, "writing into file\n");
        #endif

        /* Parse res_data to file */
        fptr = fopen("data/response.txt", "w");

        if (fptr == NULL) {
            perror("ERROR creating a file");
            close(sockfd);
            exit(0);
        }

        fputs(res_data, fptr);
        fclose(fptr);

        #ifdef DEBUG
        fprintf(stdout, "Output file created adn saved successfully\n");
        #endif

        /* read URLs from res_data */

        /* identify duplicate URLs */

        /* finished */

        close(sockfd);
// }

    return 0;
}