#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/time.h>  

int main(int argc,char **argv)
{
     
     int time=atoi(argv[2]);
     sleep(time);
     //usleep(atoi(argv[2]));
     printf("艳阳天那么风光好，红的花是绿的草！\n"); 
     return 0;
}
