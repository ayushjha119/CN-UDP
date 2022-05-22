/* server 5*/
 
 
 
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define n 256
 
struct sockaddr_in serv_addr, cli_addr;
int socketfd, connfd;
socklen_t cli_addr_len;
unsigned short serv_port = 27050;
char serv_ip[] = "127.0.0.1";
// char serv_ip[] = "192.168.6.203";
char buffer[n];
 
 
int isOpr(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/') return 1;
    return 0;
}
 
// calc() fun takes two arg, compute first and put response to second
void calc(char* str, char* ans) {
    int opFound = 0, n1 = 0, n2 = 0, isError = 0;
    char num1[20], num2[20], op;
 
    for (int i = 0; i < strlen(str); i++) {
        if (isOpr(str[i])) {
            op = str[i];
            opFound = 1;
        } else if (((int)str[i] >= 48 && (int)str[i] <= 57) || str[i] == '.') {
            if (opFound == 0) {
                num1[n1++] = str[i];
            } else {
                num2[n2++] = str[i];
            }
        } else if (str[i] != ' ') {
            // printf("Error occured\n");
            isError = 1;
            break;
        }
    }
    num1[n1] = '\0', num2[n2] = '\0';
 
    // printf("%s %d \n", num1, atoi(num1));
    // printf("%s %d \n", num2, atoi(num2));
    // printf("%c \n", op);
 
    if (isError) {
        strcpy(ans, "Invalid expression");
    } else {
        // int x = atoi(num1), y = atoi(num2);
        float x = atof(num1), y = atof(num2);
        float tmp;
        switch (op) {
        case '+':
            tmp = x + y;
            break;
        case '-':
            tmp = x - y;
            break;
        case '*':
            tmp = x * y;
            break;
        case '/':
            if (y == 0) {
                sprintf(ans, "%s", "Divide by 0.");
                return;
            }
            tmp = x / y;
            break;
        }
        sprintf(ans, "%0.2f", tmp);
    }
    // printf("%s\n", ans);
}
 
 
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
 
    printf("\nLive Command Server.\n");
 
    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { // CHANGE
        printf("\nSERVER: Eror: can't create socket");
        exit(1);
    }
 
    if (bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nSERVER ERROR: can't bind.\n");
        close(socketfd);
        exit(1);
    }
 
    cli_addr_len = sizeof(cli_addr);
    printf("______________________________________________________________________________\n");
 
 
    for (; ; ) {
        printf("______________________________________________________________________________\n");
        printf("\nSERVER: Listenting for client ... .\n");
 
        while (1) {
            printf("\nWaiting for Client ...  \n");
            bzero(buffer, n);
            if (recvfrom(socketfd, buffer, n, 0, (struct sockaddr*)&cli_addr, &cli_addr_len) < 0) { // CHANGE
                printf("Server error : Can't read\n");
                break;
            }
 
            printf("Server : Expr sent by Client : %s", buffer);
 
            if (strncmp(buffer, "exit", 4) == 0) {
                printf("EXIT");
                break;
            }
            buffer[strlen(buffer) - 1] = ' ';
 
            char ans[n];
            calc(buffer, ans);
 
            bzero(buffer, n);
            strcpy(buffer, ans);
            if (sendto(socketfd, buffer, n, 0, (struct sockaddr*)&cli_addr, cli_addr_len) < 0) { // CHANGE
                printf("\nSERVER : Cannot send message to command client.\n");
                exit(1);
            }
            printf("Server : Response sent to server.\n");
        }
        printf("\n______________________________________________________________________________\n");
    }
    close(socketfd);
 
    return 0;
}
 
 
// // //  // // OUTOUT :-
 
 
// Live Command Server.
// ______________________________________________________________________________
// ______________________________________________________________________________
//
// SERVER : Listenting for client ... .
//
// Waiting for Client ...
// Server : Expr sent by Client : 5 * 5
// Server : Response sent to server.
//
// Waiting for Client ...
// Server : Expr sent by Client : 5 / 0
// Server : Response sent to server.
//
// Waiting for Client ...
// Server : Expr sent by Client : 5 + asdf6
// Server : Response sent to server.
//
// Waiting for Client ...
// Server : Expr sent by Client : exit
// EXIT
// ______________________________________________________________________________
// ______________________________________________________________________________
//
// SERVER : Listenting for client ... .
//
// Waiting for Client ...
// ^Z
 