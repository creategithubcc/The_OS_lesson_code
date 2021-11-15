#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

struct huanchongqu//这块定义一个缓冲区 
{
	int array[3];///
	int head;//头部 
	int tail;//尾部 
	int empty;//空栈 
};

struct gonxiangneirong//这块定义一个共享的内存 
{
	struct huanchongqu data;//缓冲区里面的数据 
	HANDLE full;//满的 
	HANDLE empty;//空的 
	HANDLE mutex; //互斥的
};

void xianshishuju(struct gonxiangneirong* a)//这一块显示缓冲区的数据并输出 
{
	cout<<"缓冲区数据当前是: ";
	int i;
	for(i=a->data.head; ; )
	{
		cout<<a->data.array[i]<<" ";
		i+=1;
		i%=3;//因为是一个大小为3的缓冲区 
		if(i==a->data.tail)
		{
			cout<<endl;
			return;
		}
	}
}

PROCESS_INFORMATION jincheng(int b)//这部分再创建一个进程 
{
	char fname[MAX_PATH],cmdl[MAX_PATH];//这一部分创建进程 
	GetModuleFileName(NULL,fname,MAX_PATH);
	PROCESS_INFORMATION c;
	STARTUPINFO d;  
	sprintf(cmdl,"\"%s\" %d",fname,b);//用sprintf把数据写入字符串中 
	ZeroMemory(reinterpret_cast<void*>(&d),sizeof(d));
	d.cb=sizeof(d);
	BOOL childprocess=CreateProcess(fname,cmdl,NULL,NULL,FALSE,0,NULL,NULL,&d,&c);///然后创建子进程 
	return c; 
}
int suijishu(int n)//这个n是一个随机数字 
{
	int ran=rand();
	int ans=ran%100;//我把它们控制在100以内 
	return ans+n;//防止零的出现 
}
int main(int argc,char* argv[])
{
	HANDLE map;
	int e=0;//生产者或者消费者的编号 
	int ans,i;
	if(argc>1)
	{
		sscanf(argv[1],"%d",&e);//如果有参数就作为另一个子进程
	 } 
	if(e==0)//这是主进程，也就是零号 
	{
		map=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(struct gonxiangneirong),"huanchongqu"); ;
		HANDLE hmap=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"huanchongqu");
		LPVOID pf=MapViewOfFile(hmap,FILE_MAP_ALL_ACCESS,0,0,0);
		if(hmap==NULL) //最开始没有写这一块代码的时候也能安全运行并且没有bug 
		{
			cout<<"发生了错误！！"<<endl;//但是后来发现好多人都觉得需要写于是又重新添加 
        	exit(0);                    //了这一段代码
    	}
		if(pf==NULL)//不写这块代码也能安全运行，但以防万一还是写一下 
		{
			cout<<"发生了错误！！"<<endl;
			exit(0);
    	}
		struct gonxiangneirong *adrs=(struct gonxiangneirong*)(pf);//地址 
    	
    	adrs->full=CreateSemaphore(NULL,0,3,"FULL");//创建或打开信号量对象
    	adrs->empty=CreateSemaphore(NULL,3,3,"EMPTY");
    	adrs->mutex=CreateSemaphore(NULL,1,1,"MUTEX");
    	adrs->data.head=0;
    	adrs->data.tail=0;
    	adrs->data.empty=1;
    	UnmapViewOfFile(pf);//停止程序的内存映射 
    	pf=NULL;
    	CloseHandle(hmap); //关闭同步 
    	PROCESS_INFORMATION array2[5];//创建一个进程数组 
    	for(i=0;i<=4;i++)
    	{
    		array2[i]=jincheng(++e);
		}
		for(i=0;i<=4;i++)
		{
			WaitForSingleObject(array2[i].hProcess,INFINITE);//等待进程结束
		}
		for(i=0;i<=4;i++)
		{
			CloseHandle(array2[i].hProcess);//关闭进程句柄
		}
	}
	else if(e==1||e==2)//生产者
	{
		HANDLE hMap=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"huanchongqu"); //打开文件映射对象的函数 
		LPVOID pf=MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,0);//文件映射对象映射到当前应用程序的地址空间 
		if(pf==NULL)//不写这块代码也能安全运行，但以防万一还是写一下 
		{
			cout<<"发生了错误！！"<<endl;
			exit(0);
    	}
    	if(hMap==NULL)//不写这块代码也能安全运行，但以防万一还是写一下 
		{
			cout<<"发生了错误！！"<<endl;
			exit(0);
    	}
		struct gonxiangneirong *adrs=(struct gonxiangneirong*)(pf);
    	adrs->full=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"FULL");
    	adrs->empty=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"EMPTY");
    	adrs->mutex=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"MUTEX");	
    	for(i=0;i<=5;i++)
    	{
    		Sleep(suijishu(3));
    		WaitForSingleObject(adrs->empty,INFINITE);//挂起等待 
    		WaitForSingleObject(adrs->mutex,INFINITE);//往缓冲区添加数据
    		ans=suijishu(3);
			adrs->data.array[adrs->data.tail]=ans;
			adrs->data.tail=(adrs->data.tail+1)%3;
			adrs->data.empty=0;
			SYSTEMTIME time;
			GetLocalTime(&time);//获取这个电脑的时间 
			cout<<"现在的时间是："<<time.wYear<<"年"<<time.wMonth<<"月"<<time.wDay<<"日"<<time.wHour<<"时"<<time.wMinute<<"分"<<time.wSecond<<"秒"<<endl; 
			cout<<e-1<<"号生产者偷偷地把"<<ans<<"放入缓冲区内"<<endl;
			xianshishuju(adrs);
			ReleaseSemaphore(adrs->full,1,NULL);//信号量增加1的值
			ReleaseSemaphore(adrs->mutex,1,NULL);
		}
		UnmapViewOfFile(pf);//停止当前程序pf的一个内存映射 
		pf=NULL;
		CloseHandle(hMap);//关闭同步内核对象 
	}
	else if(e==3||e==4||e==5)//消费者
	{
		HANDLE hMap=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"huanchongqu"); 
    	LPVOID pf=MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,0);
		if(pf==NULL)//不写这块代码也能安全运行，但以防万一还是写一下 
		{
			cout<<"发生了错误！！"<<endl;
			exit(0);
    	}
    	if(hMap==NULL)//不写这块代码也能安全运行，但以防万一还是写一下 
		{
			cout<<"发生了错误！！"<<endl;
			exit(0);
    	}
		struct gonxiangneirong *adrs=(struct gonxiangneirong*)(pf);
    	adrs->full=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"FULL");
    	adrs->empty=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"EMPTY");
    	adrs->mutex=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"MUTEX");	
    	for(i=0;i<=3;i++)
    	{
    		Sleep(suijishu(3));
    		WaitForSingleObject(adrs->full,INFINITE); 
    		WaitForSingleObject(adrs->mutex,INFINITE);
			ans=adrs->data.array[adrs->data.head];
			adrs->data.head=(adrs->data.head+1)%3;
			if(adrs->data.head==adrs->data.tail)
			{
				adrs->data.empty=1;	
			} 
			else
			{
				 adrs->data.empty=0;
			} 
			SYSTEMTIME time;
			GetLocalTime(&time);
			cout<<"现在的时间是："<<time.wYear<<"年"<<time.wMonth<<"月"<<time.wDay<<"日"<<time.wHour<<"时"<<time.wMinute<<"分"<<time.wSecond<<"秒"<<endl; 
			cout<<e-3<<"号消费者光明正大地把"<<ans<<"从缓冲区拿走"<<endl;
			if(adrs->data.empty==0)
			{
				xianshishuju(adrs);
			} 
			else
			{
				cout<<"缓冲区内啥都没有了"<<endl;
			} 
			ReleaseSemaphore(adrs->empty,1,NULL);
			ReleaseSemaphore(adrs->mutex,1,NULL);
		}
		UnmapViewOfFile(pf);
		pf=NULL;
		CloseHandle(hMap);
	}
	CloseHandle(map);
	map=INVALID_HANDLE_VALUE;
	return 0;
}
