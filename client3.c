#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define server_port 8080
#define MAXLINE 512

int main(int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char recvline[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    const char *recv_dir = "./client_received_files";
    struct stat st = {0};
    if (stat(recv_dir, &st) == -1) {
        if (mkdir(recv_dir, 0700) < 0) {
            perror("mkdir failed");
            exit(1);
        }
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(server_port);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    if (write(sockfd, argv[1], strlen(argv[1])) != (ssize_t)strlen(argv[1])) {
        perror("write error");
        close(sockfd);
        exit(1);
    }

    char new_filename[1024];
    snprintf(new_filename, sizeof(new_filename), "%s/received_%s", recv_dir, argv[1]);

    FILE *fp = fopen(new_filename, "wb");
    if (!fp) {
        perror("fopen failed");
        close(sockfd);
        exit(1);
    }

    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        if (fwrite(recvline, 1, n, fp) != (size_t)n) {
            perror("fwrite error");
            fclose(fp);
            close(sockfd);
            exit(1);
        }
    }
    if (n < 0) {
        perror("read error");
    }

    fclose(fp);
    close(sockfd);

    printf("File received and stored as %s\n", new_filename);

    char diff_command[2048];
    snprintf(diff_command, sizeof(diff_command), "diff \"%s\" \"%s\"", argv[1], new_filename);

    printf("Running diff to compare files...\n");
    int diff_res = system(diff_command);

    if (diff_res == 0) {
        printf("Files are identical.\n");
    } else if (diff_res == 1) {
        printf("Files differ.\n");
    } else {
        printf("diff command failed or returned error code %d\n", diff_res);
    }

    return 0;
}
