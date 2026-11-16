#define _POSIX_C_SOURCE 200809L
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_PORT 5005
#define BACKLOG 5
#define BUF_SIZE 1024

static volatile sig_atomic_t running = 1;
static void stop_now(int sig) { (void)sig; running = 0; }

static void usage(const char *p) {
    fprintf(stderr, "Usage: %s --protocol tcp|udp [--bind IPv4] [--port N]\n", p);
}

static void display_message(const char *msg, const char *peer) {
    int seq=-1, speed=-1, rpm=-1, steering=-999;
    char status[32]="UNKNOWN";
    long long ts=0;
    sscanf(msg, "seq=%d,speed_kmph=%d,engine_rpm=%d,steering_deg=%d,status=%31[^,],timestamp_ms=%lld",
           &seq, &speed, &rpm, &steering, status, &ts);
    if (seq < 0 || speed < 0 || rpm < 0 || steering == -999) {
        printf("[DASHBOARD] malformed telemetry from %s: %s\n", peer, msg);
        return;
    }
    printf("\n+--------------------------------------------------+\n");
    printf("|             VEHICLE MONITORING SYSTEM            |\n");
    printf("+--------------------------------------------------+\n");
    printf("| Vehicle ECU       : %-28s |\n", peer);
    printf("| Sequence          : %-28d |\n", seq);
    printf("| Vehicle speed     : %-24d km/h |\n", speed);
    printf("| Engine RPM        : %-28d |\n", rpm);
    printf("| Steering angle    : %-24d deg |\n", steering);
    printf("| Vehicle status    : %-28s |\n", status);
    printf("| ECU timestamp     : %-28lld |\n", ts);
    printf("+--------------------------------------------------+\n");
}

static void handle_stream(int fd, const char *peer) {
    char buf[BUF_SIZE], line[BUF_SIZE];
    size_t used = 0;
    for (;;) {
        ssize_t n = recv(fd, buf, sizeof(buf), 0);
        if (n == 0) { printf("[DASHBOARD] TCP peer %s closed connection\n", peer); break; }
        if (n < 0) {
            if (errno == EINTR && running) continue;
            if (errno == EINTR && !running) break;
            perror("recv");
            break;
        }
        for (ssize_t i=0; i<n; i++) {
            if (buf[i] == '\n') {
                line[used] = '\0';
                display_message(line, peer);
                used = 0;
            } else if (used < sizeof(line)-1) {
                line[used++] = buf[i];
            } else {
                fprintf(stderr,"[DASHBOARD] oversized record discarded\n");
                used=0;
            }
        }
    }
}

int main(int argc, char **argv) {
    const char *protocol="udp", *bind_ip="0.0.0.0";
    int port=DEFAULT_PORT;
    for (int i=1; i<argc; i++) {
        if (!strcmp(argv[i],"--protocol") && i+1<argc) protocol=argv[++i];
        else if (!strcmp(argv[i],"--bind") && i+1<argc) bind_ip=argv[++i];
        else if (!strcmp(argv[i],"--port") && i+1<argc) port=atoi(argv[++i]);
        else { usage(argv[0]); return 2; }
    }
    if ((strcmp(protocol,"tcp") && strcmp(protocol,"udp")) || port<1 || port>65535) {
        usage(argv[0]); return 2;
    }
    signal(SIGINT,stop_now);
    signal(SIGTERM,stop_now);

    int type = !strcmp(protocol,"tcp") ? SOCK_STREAM : SOCK_DGRAM;
    int server = socket(AF_INET,type,0);
    if (server<0) { perror("socket"); return 1; }
    int yes=1;
    setsockopt(server,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons((unsigned short)port);
    if (inet_pton(AF_INET,bind_ip,&addr.sin_addr)!=1) {
        fprintf(stderr,"Invalid bind IPv4 address\n");
        close(server);
        return 2;
    }
    if (bind(server,(struct sockaddr *)&addr,sizeof(addr))<0) {
        perror("bind");
        close(server);
        return 1;
    }

    printf("[DASHBOARD ECU] %s receiver listening on %s:%d\n",protocol,bind_ip,port);
    if (!strcmp(protocol,"udp")) {
        while (running) {
            char msg[BUF_SIZE];
            struct sockaddr_in client;
            socklen_t len=sizeof(client);
            ssize_t n=recvfrom(server,msg,sizeof(msg)-1,0,(struct sockaddr *)&client,&len);
            if (n<0) {
                if(errno==EINTR && running) continue;
                if(errno==EINTR && !running) break;
                perror("recvfrom");
                break;
            }
            msg[n]='\0';
            char peer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET,&client.sin_addr,peer,sizeof(peer));
            display_message(msg,peer);
        }
    } else {
        if (listen(server,BACKLOG)<0) {
            perror("listen");
            close(server);
            return 1;
        }
        while (running) {
            struct sockaddr_in client;
            socklen_t len=sizeof(client);
            int fd=accept(server,(struct sockaddr *)&client,&len);
            if (fd<0) {
                if(errno==EINTR) { if(running) continue; else break; }
                perror("accept");
                break;
            }
            char peer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET,&client.sin_addr,peer,sizeof(peer));
            printf("[DASHBOARD ECU] accepted TCP connection from %s\n",peer);
            handle_stream(fd,peer);
            close(fd);
        }
    }
    close(server);
    printf("[DASHBOARD ECU] stopped\n");
    return 0;
}
