#include "sock.h"

sock::sock(int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening sock");
        exit(1);
    }
    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = port;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("error binding");
        exit(1);
    }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
        (sockaddr*)&cli_addr,
        &clilen);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        exit(1);
    }
}

sock::~sock(void)
{
    close(newsockfd);
    close(sockfd);
}

void sock::send(void* buf, int len)
{
    n = write(newsockfd, buf, len);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}
void sock::recv(void* buf, int len)
{
    n = read(newsockfd, buf, len);
    if (n < 0)
    {
        perror("ERROR reading from sockett");
        exit(1);
    }
}