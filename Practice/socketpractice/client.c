#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 6500
#define BUFSZ 512

int main(void) {
    int sockfd;
    struct sockaddr_in servaddr;
    char line[BUFSZ], recvline[BUFSZ];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) != 1) {
        perror("inet_pton"); exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect"); exit(1);
    }

    printf("Enter a line to send: ");
    if (!fgets(line, sizeof(line), stdin)) {
        fprintf(stderr, "no input\n"); close(sockfd); return 1;
    }

    size_t n = strlen(line);
    if (write(sockfd, line, n) != (ssize_t)n) {
        perror("write"); close(sockfd); return 1;
    }

    ssize_t r = read(sockfd, recvline, sizeof(recvline) - 1);
    if (r < 0) { perror("read"); close(sockfd); return 1; }
    recvline[r] = '\0';
    printf("Echo from server: %s", recvline);

    close(sockfd);
    return 0;
}
