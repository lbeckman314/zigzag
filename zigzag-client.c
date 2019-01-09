#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/* waitpid() */
#include <sys/wait.h>
/* process id's */
#include <unistd.h>

#include "zigzag-client.h"


int main(int argc, char *argv[])
{
    int socketFD, portNumber, charsWritten, charsRead;
    struct sockaddr_in serverAddress;
    struct hostent* serverHostInfo;
    char bufferRecieve[256];
    char bufferSend[256];
    char nick[11];

    setbuf(stdout, NULL);


    if (argc < 3) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args
    fflush(stdout);

    // Set up the server address struct
    memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
    portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
    serverAddress.sin_family = AF_INET; // Create a network-capable socket
    serverAddress.sin_port = htons(portNumber); // Store the port number
    serverHostInfo = gethostbyname(argv[1]); // Convert the machine name into a special form of address
    if (serverHostInfo == NULL)
    {
        fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0);
    }
    memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

    // Set up the socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (socketFD < 0) error("CLIENT: ERROR opening socket");

    // Connect to server
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
        error("CLIENT: ERROR connecting");

    char handshake[256];
    memset(handshake, '\0', sizeof(handshake)); // Clear out the handshake array
    strcpy(handshake, "simple-handshake");

    // Send message to server
    charsWritten = send(socketFD, handshake, strlen(handshake), 0); // Write to the server
    if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
    if (charsWritten < strlen(handshake)) printf("CLIENT: WARNING: Not all data written to socket!\n");

    welcome();
    printf("Connected to server.\n");
    printf("Enter your nickname: ");
    fflush(stdout);

    memset(nick, '\0', sizeof(nick));
    fgets(nick, sizeof(nick) - 1, stdin);
    nick[strcspn(nick, "\n")] = '\0';

    printf("----------\n");
    fflush(stdout);

    // Send message to server
    charsWritten = send(socketFD, nick, strlen(nick), 0); // Write to the server
    if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
    if (charsWritten < strlen(nick)) printf("CLIENT: WARNING: Not all data written to socket!\n");


    /* fork child process */
    pid_t n1 = fork();
    switch (n1)
    {
        /* error in forking child process */
        case -1:
            perror("fork() error\n");
            exit(1);
            break;

        /* child process */
        case 0:
            // recieve message
            recieve(socketFD, bufferRecieve, nick);
            break;

        /* parent process */
        default:
        {
            int childstate;
            // send message
            while(1)
            {
                // Get input message from user
                printf("%s> ", nick);
                fflush(stdout);

                // Clear out the bufferSend array
                memset(bufferSend, '\0', sizeof(bufferSend));

                // Get input from the user, trunc to bufferSend - 1 chars, leaving \0
                fgets(bufferSend, sizeof(bufferSend) - 1, stdin);

                // Remove the trailing \n that fgets adds
                bufferSend[strcspn(bufferSend, "\n")] = '\0';

                if (strcmp(bufferSend, "\\quit") == 0)
                {
                    /* kill all ongoing child processes */
                    kill(n1, SIGTERM);

                    close(socketFD);

                    /* exit program */
                    exit(0);

                }

                sending(socketFD, bufferSend);
                waitpid(-1, &childstate, WNOHANG);

                if ( WIFEXITED(childstate) )
                {
                    int exit_status = WEXITSTATUS(childstate);
                    //printf("Exit status of the child was %d\n", exit_status);
                    fflush(stdout);
                    if (exit_status == 1)
                    {
                        return 0;
                    }
                }

            }

            break;

        }
    }




    close(socketFD); // Close the socket
    return 0;
}

int sending(int socketFD, char *bufferSend)
{
    // Send message to server
    int charsWritten = send(socketFD, bufferSend, strlen(bufferSend), 0); // Write to the server
    if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
    if (charsWritten < strlen(bufferSend)) printf("CLIENT: WARNING: Not all data written to socket!\n");
    fflush(stdout);

    return 1;
}

int recieve(int socketFD, char *bufferRecieve, char *nick)
{

    char message[256];

    // recieve message
    while(1)
    {

        memset(bufferRecieve, '\0', sizeof(bufferRecieve)); // Clear out the bufferRecieve again for reuse
        memset(message, '\0', sizeof(message)); // Clear out the bufferRecieve again for reuse

        while (strchr(bufferRecieve, '\n') == NULL) {
            int charsRead = recv(socketFD, bufferRecieve, sizeof(bufferRecieve) - 1, 0); // Read data from the socket, leaving \0 at end
            if (charsRead < 0) error("CLIENT: ERROR reading from socket");
            if (charsRead == 0)
            {
                printf("\n!! Server disconnected. Press enter to continue.");
                fflush(stdout);
                exit(1);
            }

            /*
            for (int i = 0; i <= strlen(bufferRecieve); i++)
            {
                printf("rec: %d\n", bufferRecieve[i]);
                fflush(stdout);
            }
            */

            // print('\rClient> ', end=' ')
            strcat(message, bufferRecieve);

        }

        char *newline = strchr(message, '\n');
        *newline = '\0';

        /*
        for (int i = 0; i < strlen(message); i++) {
            printf("::  %d\n", message[i]);
            fflush(stdout);
        }
        */

        //printf("\rClient1> ");
        fflush(stdout);
        //printf("\n");
        fflush(stdout);
        printf("\rServer> %s\n", message);
        fflush(stdout);
        printf("%s> ", nick);
        fflush(stdout);
        fflush(stdout);
    }

    return 1;
}



// Error function used for reporting issues
void error(const char *msg)
{
    perror(msg); exit(0);
}


char *welcome()
{
    char *line1 = " _____                        ";
    char *line2 = "|_  (_)__ _ _____ _ __ _      ";
    char *line3 =  " / /| / _` |_ / _` / _` |    ";
    char *line4 =  "/___|_\\__, /__\\__,_\\__, | ";
    char *line5 =  "      |___/        |___/     ";

    printf("%s\n", line1);
    printf("%s\n", line2);
    printf("%s\n", line3);
    printf("%s\n", line4);
    printf("%s\n\n", line5);
    fflush(stdout);
}
