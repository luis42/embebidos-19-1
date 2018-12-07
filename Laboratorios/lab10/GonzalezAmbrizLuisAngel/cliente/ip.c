#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <pthread.h>
#include <semaphore.h> 
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

char *ip_local();
int main(int argc, char const *argv[])
{
   char *ip = ip_local();
   return 0;
}
char *ip_local(){
   int fd;
   char *ip; 
   struct ifreq ifr;
   fd = socket(AF_INET, SOCK_DGRAM, 0);
   ifr.ifr_addr.sa_family = AF_INET;
   strncpy(ifr.ifr_name, "wlp3s0", IFNAMSIZ-1);
   ioctl(fd, SIOCGIFADDR, &ifr);
   close(fd);
   ip = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
   printf("IP del servidor:\t %s \n", ip);
   return ip;
}