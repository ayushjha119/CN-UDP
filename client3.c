     
    /*client3*/
     
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
    #define n 256
    
     
    struct sockaddr_in serv_addr;
     
    int skfd, r, w;
     
    unsigned short serv_port = 25021; /*port number used by the server*/
    char serv_ip[] = "127.0.0.1";	  /*server's IP-address*/
     
    //char rbuff[128];				  /*buffer for receiving messages*/
    //char sbuff[128] = "Hello Ayush"; /*buffer for sending messages*/
    int length = sizeof(struct sockaddr_in);
    char buffer[n];
     
    int main()
    {
     
    	/*initializing server socket address structure with zero values*/
    	bzero(&serv_addr, sizeof(serv_addr));
     
    	/*filling up the server socket address structure with appropriate values*/
    	serv_addr.sin_family = AF_INET;			   /*address family*/
    	serv_addr.sin_port = htons(serv_port);	   /*port number*/
    	inet_aton(serv_ip, (&serv_addr.sin_addr)); /*IP-address*/
     
    	printf("\nUDP LIVE chat.\n");
     
    	/*creating socket*/
    	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    	{
    		printf("\nCLIENT ERROR: Cannot create socket.\n");
    		exit(1);
    	}
    	printf("CLIENT: connected to the server\n");
     
    	/*send a message to the echo server*/
    	//int length = sizeof(struct sockaddr_in);
    	
    	while(1){
    		
    		printf("CLIENT Turn: \n");
    		bzero(buffer,n);
    		fgets(buffer,n,stdin);
    		if (sendto(skfd, buffer, n, 0, (struct sockaddr *)&serv_addr, length) < 0)
    		{
    		printf("\nCLIENT ERROR: Cannot send message to the echo server.\n");
    		close(skfd);
    		exit(1);
    		}
    		if(strncmp("Bye",buffer,3)==0){
    		break;
    		}
    		
    		printf("\nwaiting for server...\n");
    		bzero(buffer,n);
    		if (recvfrom(skfd, buffer, n, 0, (struct sockaddr *)&serv_addr, &length) < 0){
    		printf("\nSERVER ERROR: Cannot receive mesage from server.\n");
    		close(skfd);
    		exit(1);
    		
    		}
    		printf("Client: %s \n",buffer);
    		if(strncmp("Bye",buffer,3)==0){
    		break;
    		}
    		
    	}
    	
    	
    	close(skfd);
    	return 0;
    } /*main ends*/