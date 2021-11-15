#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/ipc.h> 
#include<sys/shm.h>
#include<sys/sem.h>
struct huanchongqu//这块定义一个缓冲区 
{
	int array[3];///
	int head;//头部 
	int tail;//尾部 
	int empty;//空栈 
};
void xianshishuju(struct huanchongqu* a)//这一块显示缓冲区的数据并输出 
{
	printf("缓冲区数据当前是: ");
	int i;
	for(i=a->head; ; )
	{
		printf("%d ",a->array[i]);
		i+=1;
		i%=3;//因为是一个大小为3的缓冲区 
		if(i==a->tail)
		{ 
			printf("\n");
			return;
		}
	}
}
void P(int xinhao,int ans)//这里进行p操作 
{
	struct sembuf p;
	p.sem_flg=0;//创建访问标志
	p.sem_op=-1;//创建操作值
	p.sem_num=ans;//创建索引
	semop(xinhao,&p,1);
}
void V(int xinhao,int ans)//这里进行v操作 
{
	struct sembuf v;
	v.sem_flg=0;//创建访问标志
	v.sem_op=1;//创建操作值
	v.sem_num=ans;//创建索引
	semop(xinhao,&v,1);
}
int suijishu(int n)//这个n是一个随机数字 
{
	int ran=rand();
	int ans=ran%100;//我把它们控制在100以内 
	return ans+n;//防止零的出现 
}
int main(int argc,char *argv[])
{
	int e=0;
	int i,j,ans;
	int xinhao=semget(0,3,IPC_CREAT|0666);//////////创建信号量
	if(xinhao<0)
	{
		
		printf("发生1了错误bug!\n");
		exit(0);
	}
	semctl(xinhao,1,SETVAL,0);//full这里最开始给信号量初值为0
	semctl(xinhao,0,SETVAL,3);//empty这里最开始给信号量初值为3
	semctl(xinhao,2,SETVAL,1);//mutex这里最开始给信号量初值为1

	int share_m=shmget(0,sizeof(struct huanchongqu),IPC_CREAT|0666);////这里申请一个共享内存区
	if(share_m<0)
	{
	
		printf("发生2了错误bug!\n");
		exit(0);
	}
	struct huanchongqu* adrs=shmat(share_m,0,0);//这里将共享附加到申请通信的进程空间
	if(adrs==(void *)-1)
	{
		printf("发生3了错误bug!\n");
		exit(0);
	}
	adrs->head=0;
	adrs->tail=0;
	adrs->empty=1;
	for(i=0;i<=1;i++)//fok准备生产者
	{
		pid_t fok=fork();//进程号整形类型
		if(fok<0)
		{
			printf("发生4了错误bug!\n");
			exit(0);
		}
		if(fok==0)//开始创建生产者
		{
			adrs=shmat(share_m,0,0);
			if(adrs==(void *)-1)
			{
				printf("发生5了错误bug!\n");
				exit(0);
			}	
			for(j=0;j<=5;j++)
			{			
				sleep(suijishu(2));//等待随机数的时间 
				P(xinhao,0);//申请empty
				P(xinhao,2);//申请mutex
				e=suijishu(5);//将生产的随机数赋值给array 
				adrs->array[adrs->tail]=e;
				adrs->tail=(adrs->tail+1)%3;//控制在3以内 
				adrs->empty=0;
				time_t s;
				time(&s);
				printf("现在的时间为：%s",ctime(&s));		
				printf("%d号生产者偷偷地把%d放入缓冲区内\n",i,e);
				xianshishuju(adrs);
				V(xinhao,1);//释放full
				V(xinhao,2);//释放mutex
			}
			shmdt(adrs);//这一块是把共享段和子进程这两个解除连接
			exit(0);
		}
	}
	for(i=0;i<=2;i++)//fok准备消费者 
	{
		pid_t fok=fork();
		if(fok<0)
		{
			printf("发生6了错误bug!\n");
			exit(0);
		}
		if(fok==0)//开始创建消费者
		{
			adrs=shmat(share_m,0,0);
			if(adrs==(void *)-1)
			{
				printf("发生了7错误bug!\n");
				exit(0);
			}	
			for(j=0;j<=3;j++)
			{			
				sleep(suijishu(2));//等待随机数的时间 
				P(xinhao,1);//申请full
				P(xinhao,2);//申请mutex
				e=adrs->array[adrs->head];
				adrs->head=(adrs->head+1)%3;
				if(adrs->head==adrs->tail)//头尾相等的话，这个时候缓冲为空
				{
					adrs->empty=1;
				}
				else
				{
					adrs->empty=0;
				}
				time_t s;
				time(&s);
				printf("现在的时间为：%s",ctime(&s));		
				printf("%d号消费者光明正大地将%d从缓冲区内取出\n",i,e);
				if(adrs->empty==0)
				{
					xianshishuju(adrs);	
				}
				else
				{
					printf("缓冲区当前什么都没有啦！\n");
				}
				V(xinhao,0);//释放empty
				V(xinhao,2);//释放mutex
			}
			shmdt(adrs);//这一块是把共享段和子进程这两个解除连接
			exit(0);
		}
	}	
	while(wait(0)!=-1);
	shmdt(adrs);
	shmctl(share_m,IPC_RMID,0);//删除共享段
	semctl(xinhao,IPC_RMID,0);//删除信号量
	return 0;
}
