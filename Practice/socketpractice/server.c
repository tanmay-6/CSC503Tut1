#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 6500
#define BACKLOG 5
#define BUFSZ 512

static void reap(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0) {}
}

int main(void) {
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    char buf[BUFSZ];

    signal(SIGCHLD, reap);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) { perror("socket"); exit(1); }

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind"); exit(1);
    }

    if (listen(listenfd, BACKLOG) < 0) {
        perror("listen"); exit(1);
    }

    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0) {
            if (errno == EINTR) continue;
            perror("accept"); continue;
        }

        pid_t pid = fork();
        if (pid < 0) { perror("fork"); close(connfd); continue; }

        if (pid == 0) {
            // Child: handle one client
            close(listenfd);
            ssize_t n = read(connfd, buf, sizeof(buf) - 1);
            if (n > 0) {
                buf[n] = '\0';
                // Echo back
                write(connfd, buf, (size_t)n);
            }
            close(connfd);
            _exit(0);
        } else {
            // Parent
            close(connfd);
        }
    }
    return 0;
}
