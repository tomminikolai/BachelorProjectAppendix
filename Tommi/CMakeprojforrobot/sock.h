#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


/// <summary>
/// Socket server for communication
/// </summary>
class sock
{
private:
    int sockfd, newsockfd, portno, n;
    socklen_t clilen;
    sockaddr_in serv_addr, cli_addr;
public:
    /// <summary>
    /// Constructor always on devices IP adress
    /// </summary>
    /// <param name="port">The port you want to open the server on</param>
    sock(int port);
    ~sock(void);
    /// <summary>
    /// sends len bytes from location of buf
    /// </summary>
    /// <param name="buf">pointer to data that should be sent</param>
    /// <param name="len">amount of bytes to send</param>
    void send(void* buf, int len);
    /// <summary>
    /// recieves len bytes and puts them at the location of buf
    /// </summary>
    /// <param name="buf">pointer to receive buffer</param>
    /// <param name="len">bytes to recieve</param>
    void recv(void* buf, int len);
};