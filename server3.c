#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define SERV_PORT 8080
#define MAXLINE 512

// SIGCHLD handler to reap terminated child processes
void sigchld_handler(int signo) {
    // Reap all terminated children without blocking
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}

int main() {
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;
    char filename[MAXLINE];
    ssize_t n;

    // Install SIGCHLD handler before forking children
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction error");
        exit(1);
    }

    // Create listening socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket error");
        exit(1);
    }

    // Initialize server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    // Bind socket
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    // Listen on the socket
    if (listen(listenfd, 5) < 0) {
        perror("listen error");
        exit(1);
    }

    printf("Server listening on port %d\n", SERV_PORT);

    // Accept incoming connections in a loop
    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        if (connfd < 0) {
            perror("accept error");
            continue;
        }

        if ((childpid = fork()) == 0) {
            // Child process
            close(listenfd);  // Close listening socket in child

            // Read filename sent by client
            n = read(connfd, filename, sizeof(filename) - 1);
            if (n <= 0) {
                // Error or connection closed by client
                close(connfd);
                exit(1);
            }
            filename[n] = '\0';  // Null-terminate

            printf("Client requested file: %s\n", filename);

            FILE *fp = fopen(filename, "rb");
            if (!fp) {
                perror("fopen");
                close(connfd);
                exit(1);
            }

            // Send file content to client
            char buffer[MAXLINE];
            size_t read_bytes;
            while ((read_bytes = fread(buffer, 1, MAXLINE, fp)) > 0) {
                if (write(connfd, buffer, read_bytes) != (ssize_t)read_bytes) {
                    perror("write error");
                    break;
                }
            }

            fclose(fp);
            close(connfd);
            exit(0);
        }
        // Parent closes connected socket and continues accepting
        close(connfd);
    }

    // Not reached
    return 0;
}
