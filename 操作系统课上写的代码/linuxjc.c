#include <math.h>  
#include <stdio.h>   
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/time.h>
int main(int argc,char **argv)  
{  
    struct timeval time_s;
	struct timeval time_e;  
    int hou,min,sec,msec,mic;
     
     
     if(fork()==0)  
    { 
        printf("生了个孩子\n");  
        execv(argv[1],argv);  
    } 
    else
    {  
        gettimeofday(&time_s,NULL);  
        wait(NULL);  
        gettimeofday( &time_e,NULL);
        hou=(time_e.tv_sec-time_s.tv_sec)/3600;
        min=((time_e.tv_sec-time_s.tv_sec)/60)%60;
        sec=(time_e.tv_sec-time_s.tv_sec)%60;
        msec=(time_e.tv_usec-time_s.tv_usec)/1000;
        mic=(time_e.tv_usec-time_s.tv_usec)%1000;

	if(hou<0)
        {
            hou+=24;
        }
	if(min<0)
        {
            hou--;
            min+=60;
        }
        if(sec<0)  
        {  
            min--;  
            sec+=60;  
        }
        if(msec<0)  
        {  
            sec--;  
            msec+=1000;  
        } 
        if(mic<0)  
        {  
            msec--;  
            mic+=1000;  
        }  
      
       ///////////////// 
        printf("%d时%d分%d秒%d毫秒%d微秒\n",hou,min,sec,msec,mic);  
    }  
     
    
    if (argc==1)  
    {  
        printf("错误!\n");  
        exit(0);  
    } 
    return 0;  
}
