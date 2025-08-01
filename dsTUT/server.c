#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_PORT 9877      // must be same as client port
#define MAXLINE 512

int main() {
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;
    char filename[MAXLINE];
    ssize_t n;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    if (listen(listenfd, 5) < 0) {
        perror("listen error");
        exit(1);
    }

    printf("Server listening on port %d\n", SERV_PORT);

    while (1) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if (connfd < 0) {
            perror("accept error");
            continue;
        }

        // Read filename from client
        n = read(connfd, filename, sizeof(filename)-1);
        if (n <= 0) {
            close(connfd);
            continue;
        }
        filename[n] = '\0';  // Null-terminate filename

        FILE *fp = fopen(filename, "rb");
        if (!fp) {
            perror("fopen");
            // Send an error message or empty file indication if you want
            close(connfd);
            continue;
        }

        // Read file and send to client
        char buffer[MAXLINE];
        while ((n = fread(buffer, 1, MAXLINE, fp)) > 0) {
            if (write(connfd, buffer, n) != n) {
                perror("write error");
                break;
            }
        }
        fclose(fp);
        close(connfd);
    }
    return 0;
}
