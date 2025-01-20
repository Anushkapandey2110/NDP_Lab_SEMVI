//needs changes
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<dirent.h>
#include<string.h>
#define MAXSIZE 90

main()
{
int sockfd,newsockfd,retval;
socklen_t actuallen;
int recedbytes,sentbytes;
struct sockaddr_in serveraddr,clientaddr;

int choice,count=0;
char search_string[MAXSIZE];
int i=0;int j=0;
int a=0;
sockfd=socket(AF_INET,SOCK_STREAM,0);

if(sockfd==-1)
{
printf("\nSocket creation error");
}

serveraddr.sin_family=AF_INET;
serveraddr.sin_port=htons(3388);
serveraddr.sin_addr.s_addr=htons(INADDR_ANY);
retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
if(retval==-1)
{
printf("Binding error");
close(sockfd);
}

retval=listen(sockfd,1);
if(retval==-1)
{
close(sockfd);
}

actuallen=sizeof(clientaddr);
newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);


if(newsockfd==-1)
{
close(sockfd);
}
 while (1) {
        
        char file_name[MAXSIZE];
       // Receive the file name from the client
    recedbytes = recv(newsockfd, file_name, sizeof(file_name), 0);
    if (recedbytes <= 0) {
        printf("Recv Error: File name\n");
        return -1;
    }
    
    printf("Received file name: %s\n", file_name);

    // Search for the file in the current directory
    const char *dir_path = ".";
    DIR *dir;
    struct dirent *entry;
    int file_found = 0;

    // Open the directory
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return -1; // Error opening directory
    }

    // Iterate over all the files and directories in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Compare the file names
        if (strcmp(entry->d_name, file_name) == 0) {
            printf("File found: %s/%s\n", dir_path, entry->d_name);
            file_found = 1;
            break;  // File found, no need to search further
        }
    }
    
    closedir(dir);

    if (!file_found) {
        printf("File not found\n");
        return 0;  // Exit if the file is not found
    }

    // Receive the user's choice
    recedbytes = recv(newsockfd, &choice, sizeof(choice), 0);
    if (recedbytes <= 0) {
        printf("Recv Error: Choice\n");
        return -1;
    }

    // If the user chose to search for a string in the file
    if (choice == 1) {
        // Receive the string to search for
        recedbytes = recv(newsockfd, search_string, sizeof(search_string), 0);
        if (recedbytes <= 0) {
            printf("Recv Error: Search string\n");
            return -1;
        }

        printf("Searching for string: %s\n", search_string);

        // Open the file and search for the string
        FILE *fptr = fopen(file_name, "r");
        if (fptr == NULL) {
            perror("ERROR OPENING FILE");
            return -1;
        }

        char line[1024];
        count = 0;
        
        // Read the file line by line
        while (fgets(line, sizeof(line), fptr) != NULL) {
            if (strstr(line, search_string) != NULL) {
                count++;  // Increment count if the string is found
            }
        }

        fclose(fptr);

        // Send result back to the client
        if (count > 0) {
            sentbytes = send(newsockfd, &count, sizeof(count), 0);
            if (sentbytes == -1) {
                printf("Send error\n");
                return -1;
            }
            printf("String found %d times.\n", count);
        } else {
            sentbytes = send(newsockfd, &count, sizeof(count), 0);
            if (sentbytes == -1) {
                printf("Send error\n");
                return -1;
            }
            
        }
        else if (choice == 2) {
        // Receive the string to be replaced and the string with which it will be replaced
        char str1[MAXSIZE];
        char str2[MAXSIZE];
        char msg[MAXSIZE];
        recedbytes = recv(sockfd, str1, strlen(str1) +1, 0);
        if (recedbytes == -1) {
            perror("Recv error");
            close(sockfd);
            exit(1);
        }
         recedbytes = recv(sockfd, str2, strlen(str2) +1, 0);
        if (recedbytes == -1) {
            perror("Recv error");
            close(sockfd);
            exit(1);
        }
        
        
    // Open the file for reading and writing
    FILE *file = fopen(file_name, "r+");
    if (file == NULL) {
        perror("Unable to open file");
        return 1;
    }

    // Read the entire content of the file into a buffer
    char content[MAXSIZE];
    size_t content_size = fread(content, sizeof(char), MAXSIZE, file);
    content[content_size] = '\0';  // Null-terminate the string

    // Flag to check if we made any replacements
    int replacement_made = 0;

    // Start replacing the occurrences of old_substr with new_substr in the content
    char *pos;
    char temp[MAXSIZE];
    int index = 0;
    int old_len = strlen(str1);
    int new_len = strlen(str2);

    while ((pos = strstr(content, str1)) != NULL) {
        // Copy the part before the old substring
        int len = pos - content;
        strncpy(temp + index, content, len);
        index += len;

        // Copy the new substring
        strncpy(temp + index, new_substr, new_len);
        index += new_len;

        // Move the pointer past the old substring
        content = pos + old_len;

        // Set the replacement flag
        replacement_made = 1;
    }

    // Copy the remaining part of the string
    strcpy(temp + index, content);

    // If no replacement was made, notify the user
    if (!replacement_made) {
       sentbytes = send(newsockfd, "string replaced",15, 0);
            if (sentbytes == -1) {
                printf("Send error\n");
                return -1;
            }
    } else {
        // Go back to the beginning of the file to overwrite the content
        fseek(file, 0, SEEK_SET);

        // Write the modified content back to the file
        fwrite(temp, sizeof(char), strlen(temp), file);

        // Truncate the file to the new size (in case the new content is smaller)
        ftruncate(fileno(file), strlen(temp));
	sentbytes = send(newsockfd, "string not found",16, 0);
            if (sentbytes == -1) {
                printf("Send error\n");
                return -1;
            }
       
    }

    fclose(file);

        
        }
            else if (sort == 9) {
                // Sorting in descending order
                printf("Sorting in descending order\n");
                for (i = 0; i < n - 1; i++) {
                    for (j = 0; j < n - 1 - i; j++) {
                        if (arr[j] < arr[j + 1]) {
                            int temp = arr[j];
                            arr[j] = arr[j + 1];
                            arr[j + 1] = temp;
                        }
                    }
                }
                sentbytes = send(newsockfd, arr, n * sizeof(int), 0);
                if (sentbytes == -1) {
                    printf("Send error\n");
                    break;
                }
            }
            else {
                printf("Invalid sort choice\n");
            }
        }
        else if (choice == 3) {
            // Exit condition
            break;
        }
        else {
            printf("Invalid choice\n");
            break;
        }
    }

    // Close the sockets outside the loop to allow proper connection handling
    close(newsockfd);
    close(sockfd);

    return 0;
}
