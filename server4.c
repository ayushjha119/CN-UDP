     
    /*server4*/
     
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
    		
    			exit(1);
    		}
    		
    		printf("Server: cmd sent by client: %s\n",buffer);
    		if(strncmp(buffer,"exit",4)==0){
    		printf("EXIT\n");
    		break;
    		}
    		buffer[strlen(buffer)-1]=' ';
    		strcat(buffer, " > temp.txt");
    		int isDone = system(buffer);
    		
    		if(isDone==0){
    		FILE *fp=fopen("./temp.txt","r");
    		bzero(buffer,n);
    		if(fp==NULL){
    		printf("SErver : can't Read\n");
    		}else{
    		while(fgets(buffer,n,fp) !=NULL){
		if (sendto(listenfd, buffer, n, 0, (struct sockaddr *)&serv_addr, length) < 0){
    				printf("\nSERVER ERROR: Cannot send to cmd\n");
    				close(listenfd);
    				close(connfd);
    				exit(1);
    			}
    		}	
    		bzero(buffer,n);
    		strcpy(buffer,"DONE");
    		if (sendto(listenfd, buffer, n, 0, (struct sockaddr *)&serv_addr, length) < 0){
    				printf("\nSERVER ERROR: Cannot send to cmd\n");
    				close(listenfd);
    				close(connfd);
    				exit(1);
    			}
    			printf("Server Error : Response send to cli");
    			fclose(fp);
    		}
    		
    		}else{
    		printf("Sever error: Invalid cmd");
    		bzero(buffer,n);
    		strcpy(buffer,"Error: invalid cmd");
    		if (sendto(listenfd, buffer, n, 0, (struct sockaddr *)&serv_addr, length) < 0){
    				printf("\nSERVER ERROR: Cannot send to cmd\n");
    				close(listenfd);
    				close(connfd);
    				exit(1);
    			}
    			
    			
    		
    		}
    		system("rm temp.txt");
    		
    		}
    		
    		close(connfd);
    		
     
    		}
    	close(listenfd);
    	return 0;
    } /*main ends*/