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
int choice,num,res;
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
 int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter the numbers:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Send the size of the array and the array to the server
    sentbytes = send(sockfd, &n, sizeof(n), 0);
    if (sentbytes == -1) {
        perror("Send error");
        close(sockfd);
        exit(1);
    }

    sentbytes = send(sockfd, arr, n * sizeof(int), 0);
    if (sentbytes == -1) {
        perror("Send error");
        close(sockfd);
        exit(1);
    }

    // Now, prompt the user for a choice
    printf("Enter your choice:\n");
    printf("1. Search for a number\n");
    printf("2. Sort numbers\n");
    printf("3. Exit\n");
    scanf("%d", &choice);

    // Send choice to the server
    sentbytes = send(sockfd, &choice, sizeof(choice), 0);
    if (sentbytes == -1) {
        perror("Send error");
        close(sockfd);
        exit(1);
    }

    // Handle the search for a number if the choice is 1
    if (choice == 1) {
        printf("Enter the number to search for: ");
        scanf("%d", &num);

        // Send the number to search for
        sentbytes = send(sockfd, &num, sizeof(num), 0);
        if (sentbytes == -1) {
            perror("Send error");
            close(sockfd);
            exit(1);
        }

        // Receive search result (echoed number here)
        recedbytes = recv(sockfd, &res, sizeof(res), 0);
        if (recedbytes == -1) {
            perror("Recv error");
            close(sockfd);
            exit(1);
        }
	if(res !=-1)
        printf("Number found at index: %d\n", res);
        else
        printf("Number not found");
    } 
    // Handle the sorting if the choice is 2
    else if (choice == 2) {
        // Receive the sorted array from the server
        int sort;
        printf("Enter 8 for sorting in ascending order and 9 for sorting in descending order");
        scanf("%d",&sort);
        sentbytes = send(sockfd,&sort,sizeof(sort),0);
         if (sentbytes == -1) {
            perror("Send error");
            close(sockfd);
            exit(1);
        }
        recedbytes = recv(sockfd, arr, sizeof(arr), 0);
        if (recedbytes == -1) {
            perror("Recv error");
            close(sockfd);
            exit(1);
        }

        printf("Sorted array:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    } else {
        printf("Invalid choice\n");
    }
    if(choice == 3)
    {
    	printf("exiting");
    	close(sockfd);
    	exit(1);
    }
    }

    close(sockfd);
    return 0;
}
