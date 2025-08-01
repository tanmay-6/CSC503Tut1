#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERV_PORT 9877
#define MAXLINE 512

int main(int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char recvline[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    servaddr.sin_port = htons(SERV_PORT);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    // Send filename to server
    write(sockfd, argv[1], strlen(argv[1]));

    // Open file for writing received data
    char new_filename[512];
    snprintf(new_filename, sizeof(new_filename), "received_%s", argv[1]);
    FILE *fp = fopen(new_filename, "wb");
    if (!fp) {
        perror("fopen");
        close(sockfd);
        exit(1);
    }

    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        fwrite(recvline, 1, n, fp);
    }
    if (n < 0) {
        perror("read error");
    }

    printf("File received and stored as %s\n", new_filename);
    fclose(fp);
    close(sockfd);
    return 0;
}
