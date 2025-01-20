#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#define MAXSIZE 50

main()
{
int sockfd,retval;
int recedbytes,sentbytes;
struct sockaddr_in serveraddr;
int choice,num,count;


sockfd=socket(AF_INET,SOCK_STREAM,0);
if(sockfd==-1)
{
printf("\nSocket Creation Error");

}
printf("%i",sockfd);
serveraddr.sin_family=AF_INET;
serveraddr.sin_port=htons(3388);
serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
if(retval==-1)
{
printf("Connection error");

}
while(1)
{
 char name[MAXSIZE];
        printf("Enter the name of the file: ");
        scanf("%s", name);

        // Send the name of the file to the server
        sentbytes = send(sockfd, name, strlen(name) + 1, 0);
        if (sentbytes == -1) {
            perror("Send error");
            close(sockfd);
            exit(1);
        }

        // Prompt for a choice
        printf("Enter your choice:\n");
        printf("1. Search\n");
        printf("2. Replace\n");
        printf("3. Exit\n");
        scanf("%d", &choice);

        // Send the choice to the server
        sentbytes = send(sockfd, &choice, sizeof(choice), 0);
        if (sentbytes == -1) {
            perror("Send error");
            close(sockfd);
            exit(1);
        }

        // If the choice is 1 (Search)
        if (choice == 1) {
            char search_string[MAXSIZE];
            printf("Enter the string you want to search for: ");
            scanf("%s", search_string);

            // Send the string to search for
            sentbytes = send(sockfd, search_string, strlen(search_string) + 1, 0);
            if (sentbytes == -1) {
                perror("Send error");
                close(sockfd);
                exit(1);
            }

            // Receive the result (number of occurrences or error message)
            recedbytes = recv(sockfd, &count, sizeof(count), 0);
            if (recedbytes == -1) {
                perror("Recv error");
                close(sockfd);
                exit(1);
            }

            // If result is -1, it means the string was not found
            if (count == 0) {
                printf("String not found\n");
            } else {
                printf("The string was found %d times in the file.\n", count);
            }
        }
    //
    else if (choice == 2) {
        // Send the string to be replaced and the string with which it will be replaced
        char str1[MAXSIZE];
        char str2[MAXSIZE];
        printf("Enter the string you want to replace");
        scanf("%s",&str1);
        sentbytes = send(sockfd,str1,strlen(str1)+1,0);
         if (sentbytes == -1) {
            perror("Send error");
            close(sockfd);
            exit(1);
        }
        printf("Enter the string you want to replace the string with");
        scanf("%s",&str2);
        sentbytes = send(sockfd,str2,strlen(str2)+1,0);
         if (sentbytes == -1) {
            perror("Send error");
            close(sockfd);
            exit(1);
        }
        char msg[MAXSIZE];
        recedbytes = recv(sockfd, msg, strlen(msg) +1, 0);
        if (recedbytes == -1) {
            perror("Recv error");
            close(sockfd);
            exit(1);
        }

        printf("%s",msg);
        }
    if(choice == 3)
    {
    	printf("exiting");
    	close(sockfd);
    	exit(1);
    }
    }
