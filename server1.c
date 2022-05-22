     
    /*server1*/
     
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
     
    struct sockaddr_in serv_addr, cli_addr;
    int listenfd, connfd, r, w, val, cli_addr_len;
     
    unsigned short serv_port = 25021; /*port number to be used by the server*/
    char serv_ip[] = "127.0.0.1";	  /*server's IP-address*/
     
    char buff[128]; /*buffer for sending and receiving messages*/
     
    int main()
    {
    	/*initializing server socket address structure with zero values*/
    	bzero(&serv_addr, sizeof(serv_addr));
     
    	/*filling up the server socket address structure with appropriate values*/
    	serv_addr.sin_family = AF_INET;			   /*address family*/
    	serv_addr.sin_port = htons(serv_port);	   /*port number*/
    	inet_aton(serv_ip, (&serv_addr.sin_addr)); /*IP-address*/
     
    	printf("\nTCP ECHO SERVER.\n");
     
    	/*creating socket*/
    	if ((listenfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    	{
    		printf("\nSERVER ERROR: Cannot create socket.\n");
    		exit(1);
    	}
     
    	/*binding server socket address structure*/
    	if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	{
    		printf("\nSERVER ERROR: Cannot bind.\n");
    		close(listenfd);
    		exit(1);
    	}
     
    	/*listen to client connection requests*/
     
    	cli_addr_len = sizeof(cli_addr);
    	int length = sizeof(struct sockaddr_in);
    	for (;;)
    	{
    		printf("\nSERVER: Listenning for clients...Press Cntrl + c or ctrl + z to stop echo server:\n");
     
    		/*waiting for messages from client*/
    		if (recvfrom(listenfd, buff, 128, 0, (struct sockaddr *)&serv_addr, &length) < 0)
    			printf("\nSERVER ERROR: Cannot receive message from client.\n");
    		else
    		{
    			/*echo back the message received from client*/
    			if (sendto(listenfd, buff, 128, 0, (struct sockaddr *)&serv_addr, length) < 0)
     
    				printf("\nSERVER ERROR: Cannot send message to the client.\n");
    			else
    				printf("\nSERVER: Echoed back %s .\n", buff);
    		}
    	} /*for ends*/
    	return 0;
    } /*main ends*/