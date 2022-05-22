     
    /*client2*/
     
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <time.h>
     
    struct sockaddr_in serv_addr;
     
    int skfd, r, w;
     
    unsigned short serv_port = 25021; /*port number used by the server*/
    char serv_ip[] = "127.0.0.1";	  /*server's IP-address*/
     
    char rbuff[128];				  /*buffer for receiving messages*/
    char sbuff[128] = "Hello Ayush"; /*buffer for sending messages*/
    //int length = sizeof(struct sockaddr_in);
    int n =128;
     
    int main()
    {
     
    	/*initializing server socket address structure with zero values*/
    	bzero(&serv_addr, sizeof(serv_addr));
     
    	/*filling up the server socket address structure with appropriate values*/
    	serv_addr.sin_family = AF_INET;			   /*address family*/
    	serv_addr.sin_port = htons(serv_port);	   /*port number*/
    	inet_aton(serv_ip, (&serv_addr.sin_addr)); /*IP-address*/
     
    	printf("\nUDP ECHO CLIENT.\n");
     
    	/*creating socket*/
    	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    	{
    		printf("\nCLIENT ERROR: Cannot create socket.\n");
    		exit(1);
    	}
    	strcpy(sbuff,"OKK\n");
     
    	/*send a message to the echo server*/
    	int length = sizeof(struct sockaddr_in);
    	if (sendto(skfd, sbuff, 128, 0, (struct sockaddr *)&serv_addr, length) < 0)
    	{
    		printf("\nCLIENT ERROR: Cannot send message to the echo server.\n");
    		close(skfd);
    		exit(1);
    	}
    	bzero(rbuff,n);
    	printf("\nCLIENT: Message sent to echo server.\n");
     
    	/*read back the echoed message from server*/
    	if (recvfrom(skfd, rbuff, 128, 0, (struct sockaddr *)&serv_addr, &length) < 0)
    		printf("\nCLIENT ERROR: Cannot receive mesage from server.\n");
    	else
    	{
    		/*print the received message on console*/
    		printf("\nCLIENT: time from echo server: %s\n", rbuff);
    	}
    	close(skfd);
    	exit(1);
    } /*main ends*/