     
    /*server3*/
     
    #include <fcntl.h>
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #define n 256
     
    struct sockaddr_in serv_addr, cli_addr;
    int listenfd, connfd, r, w, val;
    socklen_t cli_addr_len;
     
    unsigned short serv_port = 25021; /*port number to be used by the server*/
    char serv_ip[] = "127.0.0.1";	  /*server's IP-address*/
     
    char buffer[n]; /*buffer for sending and receiving messages*/

  
    int length = sizeof(struct sockaddr_in); 
    int main()
    {
    	/*initializing server socket address structure with zero values*/
    	bzero(&serv_addr, sizeof(serv_addr));
     
    	/*filling up the server socket address structure with appropriate values*/
    	serv_addr.sin_family = AF_INET;			   /*address family*/
    	serv_addr.sin_port = htons(serv_port);	   /*port number*/
    	inet_aton(serv_ip, (&serv_addr.sin_addr)); /*IP-address*/
     
    	printf("\nUDP LIVE SERVER.\n");
     
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
    	int maxcli=5;
     
    	cli_addr_len = sizeof(cli_addr);
    	//int length = sizeof(struct sockaddr_in);
    	for(int client=1;client<=maxcli;client++){
    	
    		printf("\nSERVER: Listenning for clients...Press Cntrl + c or ctrl + z to stop echo server:\n");
    		printf("\nSERVER: connenction from client accepted\n");
    		
    		
    		while(1){
    		printf("\nWaiting for client...\n");
    		bzero(buffer,n);
    		if(recvfrom(listenfd,buffer,n,0,(struct sockaddr *)&serv_addr,&length)<0){
    			printf("\nSERVER ERROR: CAn't Read\n");
    			close(listenfd);
    			close(connfd);
    			exit(1);
    		}
    		printf("client: %s \n",buffer);
    		if(strncmp("Bye",buffer,3)==0){
    		break;
    		}
    		printf("\nSERVER TURN: \n");
    		bzero(buffer,n);
    		fgets(buffer,n,stdin);
    		if (sendto(listenfd, buffer, n, 0, (struct sockaddr *)&serv_addr, length) < 0){
    				printf("\nSERVER ERROR: Cannot send message to the client.\n");
    				close(listenfd);
    				close(connfd);
    				exit(1);
    			}
    			
    			
    			if(strncmp("Bye",buffer,3)==0){
    			break;
    			}
    			
    		
    		}
    		
    		close(connfd);
    		
     
    		}
    	close(listenfd);
    	return 0;
    } /*main ends*/