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
#define BUF_SIZE 256
static volatile sig_atomic_t running=1;
static void stop_now(int sig){(void)sig;running=0;}
static void usage(const char*p){fprintf(stderr,"Usage: %s --protocol tcp|udp --host IPv4 [--port N] [--count N] [--interval-ms N]\n",p);}
static long long now_ms(void){struct timespec ts;clock_gettime(CLOCK_REALTIME,&ts);return (long long)ts.tv_sec*1000LL+ts.tv_nsec/1000000LL;}
int main(int argc,char**argv){
 const char*protocol="udp",*host="127.0.0.1";int port=DEFAULT_PORT,count=0,interval_ms=1000;
 for(int i=1;i<argc;i++){
  if(!strcmp(argv[i],"--protocol")&&i+1<argc)protocol=argv[++i];
  else if(!strcmp(argv[i],"--host")&&i+1<argc)host=argv[++i];
  else if(!strcmp(argv[i],"--port")&&i+1<argc)port=atoi(argv[++i]);
  else if(!strcmp(argv[i],"--count")&&i+1<argc)count=atoi(argv[++i]);
  else if(!strcmp(argv[i],"--interval-ms")&&i+1<argc)interval_ms=atoi(argv[++i]);
  else{usage(argv[0]);return 2;}
 }
 if((strcmp(protocol,"tcp")&&strcmp(protocol,"udp"))||port<1||port>65535||interval_ms<1||count<0){usage(argv[0]);return 2;}
 signal(SIGINT,stop_now);signal(SIGTERM,stop_now);
 int sock=socket(AF_INET,!strcmp(protocol,"tcp")?SOCK_STREAM:SOCK_DGRAM,0);
 if(sock<0){perror("socket");return 1;}
 struct sockaddr_in server;memset(&server,0,sizeof(server));server.sin_family=AF_INET;server.sin_port=htons((unsigned short)port);
 if(inet_pton(AF_INET,host,&server.sin_addr)!=1){fprintf(stderr,"Invalid IPv4 address: %s\n",host);close(sock);return 2;}
 if(!strcmp(protocol,"tcp")){if(connect(sock,(struct sockaddr*)&server,sizeof(server))<0){perror("TCP connect");close(sock);return 1;}printf("[VEHICLE ECU] TCP connected to %s:%d\n",host,port);}
 else printf("[VEHICLE ECU] UDP sending to %s:%d (connectionless)\n",host,port);
 int seq=0;
 while(running&&(count==0||seq<count)){
  seq++;int speed=28+(seq*13)%105,rpm=850+(seq*317)%4200,steering=((seq*17)%121)-60;
  const char*status=(speed>110||rpm>4400)?"WARNING":"NORMAL";char payload[BUF_SIZE];
  int n=snprintf(payload,sizeof(payload),"seq=%d,speed_kmph=%d,engine_rpm=%d,steering_deg=%d,status=%s,timestamp_ms=%lld\n",seq,speed,rpm,steering,status,now_ms());
  if(n<0||n>=(int)sizeof(payload)){fprintf(stderr,"Payload formatting error\n");break;}
  ssize_t sent=!strcmp(protocol,"tcp")?send(sock,payload,(size_t)n,0):sendto(sock,payload,(size_t)n,0,(struct sockaddr*)&server,sizeof(server));
  if(sent<0){perror("send");if(errno==EPIPE||errno==ECONNRESET)break;}else printf("[VEHICLE ECU] %s | %s",protocol,payload);
  struct timespec delay={interval_ms/1000,(interval_ms%1000)*1000000L};while(running&&nanosleep(&delay,&delay)<0&&errno==EINTR){}
 }
 if(!strcmp(protocol,"tcp"))shutdown(sock,SHUT_WR);close(sock);printf("[VEHICLE ECU] stopped after %d message(s)\n",seq);return 0;
}