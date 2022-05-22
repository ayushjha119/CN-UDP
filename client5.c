/* client 5*/
 
 
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<time.h>
#define n 256
 
struct sockaddr_in serv_addr;
int skfd;
unsigned short serv_port = 27050;
char serv_ip[] = "127.0.0.1";
// char serv_ip[] = "192.168.6.205";
char buffer[n];
 
int main(int argc, char* argv[]) {
 
    if (argc > 1) {
        serv_port = atoi(argv[1]);
        printf("SERVER port no.   : %d \n", serv_port);
    }
    if (argc > 2) {
        bzero(serv_ip, sizeof(serv_ip));
        strcpy(serv_ip, argv[2]);
        printf("SERVER ip address : %s \n", serv_ip);
    }
 
    bzero(&serv_addr, sizeof(serv_addr));
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    inet_aton(serv_ip, (&serv_addr.sin_addr));
 
    printf("\nLive Command Client.\n");
 
    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { // CHANGE
        printf("\nCLIENT ERROR: Cannot create socket.\n");
        exit(1);
    }
 
    printf("\nCLIENT: Connected to the server\n");
    printf("______________________________________________________________________________\n");
 
    while (1) {
        printf("______________________________________________________________________________\n");
        printf("CLIENT : Write a expr : ");
        bzero(buffer, n);
        fgets(buffer, n, stdin);
        if (sendto(skfd, buffer, n, 0, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)) < 0) {  // CHANGE
            printf("\nSERVER : Cannot send message to the command server");
            exit(1);
        }
        printf("Client : Expr sent to server \n");
 
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("EXIT\n");
            close(skfd);
            exit(1);
        }
 
        printf("Client : Server Response : ");
        int length = sizeof(struct sockaddr_in);
 
        bzero(buffer, n);
        if (recvfrom(skfd, buffer, n, 0, (struct sockaddr*)&serv_addr, &length) < 0) {      // CHANGE
            printf("Server error : Can't read from server \n");
            close(skfd);
            exit(1);
        }
        printf("%s", buffer);
 
        printf("\n______________________________________________________________________________\n");
    }
    close(skfd);
 
    return 0;
 
}
 
// // //  // // OUTOUT :-
 
 
// Live Command Client.
//
// CLIENT : Connected to the server
// ______________________________________________________________________________
// ______________________________________________________________________________
// CLIENT : Write a expr : 5 * 5
// Client : Expr sent to server
// Client : Server Response : 25.00
// ______________________________________________________________________________
// ______________________________________________________________________________
// CLIENT : Write a expr : 5 / 0
// Client : Expr sent to server
// Client : Server Response : Divide by 0.
// ______________________________________________________________________________
// ______________________________________________________________________________
// CLIENT : Write a expr : 5 + asdf6
// Client : Expr sent to server
// Client : Server Response : Invalid expression
// ______________________________________________________________________________
// ______________________________________________________________________________
// CLIENT : Write a expr : exit
// Client : Expr sent to server
// EXIT
 
 