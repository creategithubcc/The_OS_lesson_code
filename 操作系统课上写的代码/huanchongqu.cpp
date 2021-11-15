#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

struct huanchongqu//��鶨��һ�������� 
{
	int array[3];///
	int head;//ͷ�� 
	int tail;//β�� 
	int empty;//��ջ 
};

struct gonxiangneirong//��鶨��һ��������ڴ� 
{
	struct huanchongqu data;//��������������� 
	HANDLE full;//���� 
	HANDLE empty;//�յ� 
	HANDLE mutex; //�����
};

void xianshishuju(struct gonxiangneirong* a)//��һ����ʾ�����������ݲ���� 
{
	cout<<"���������ݵ�ǰ��: ";
	int i;
	for(i=a->data.head; ; )
	{
		cout<<a->data.array[i]<<" ";
		i+=1;
		i%=3;//��Ϊ��һ����СΪ3�Ļ����� 
		if(i==a->data.tail)
		{
			cout<<endl;
			return;
		}
	}
}

PROCESS_INFORMATION jincheng(int b)//�ⲿ���ٴ���һ������ 
{
	char fname[MAX_PATH],cmdl[MAX_PATH];//��һ���ִ������� 
	GetModuleFileName(NULL,fname,MAX_PATH);
	PROCESS_INFORMATION c;
	STARTUPINFO d;  
	sprintf(cmdl,"\"%s\" %d",fname,b);//��sprintf������д���ַ����� 
	ZeroMemory(reinterpret_cast<void*>(&d),sizeof(d));
	d.cb=sizeof(d);
	BOOL childprocess=CreateProcess(fname,cmdl,NULL,NULL,FALSE,0,NULL,NULL,&d,&c);///Ȼ�󴴽��ӽ��� 
	return c; 
}
int suijishu(int n)//���n��һ��������� 
{
	int ran=rand();
	int ans=ran%100;//�Ұ����ǿ�����100���� 
	return ans+n;//��ֹ��ĳ��� 
}
int main(int argc,char* argv[])
{
	HANDLE map;
	int e=0;//�����߻��������ߵı�� 
	int ans,i;
	if(argc>1)
	{
		sscanf(argv[1],"%d",&e);//����в�������Ϊ��һ���ӽ���
	 } 
	if(e==0)//���������̣�Ҳ������� 
	{
		map=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(struct gonxiangneirong),"huanchongqu"); ;
		HANDLE hmap=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"huanchongqu");
		LPVOID pf=MapViewOfFile(hmap,FILE_MAP_ALL_ACCESS,0,0,0);
		if(hmap==NULL) //�ʼû��д��һ������ʱ��Ҳ�ܰ�ȫ���в���û��bug 
		{
			cout<<"�����˴��󣡣�"<<endl;//���Ǻ������ֺö��˶�������Ҫд������������� 
        	exit(0);                    //����һ�δ���
    	}
		if(pf==NULL)//��д������Ҳ�ܰ�ȫ���У����Է���һ����дһ�� 
		{
			cout<<"�����˴��󣡣�"<<endl;
			exit(0);
    	}
		struct gonxiangneirong *adrs=(struct gonxiangneirong*)(pf);//��ַ 
    	
    	adrs->full=CreateSemaphore(NULL,0,3,"FULL");//��������ź�������
    	adrs->empty=CreateSemaphore(NULL,3,3,"EMPTY");
    	adrs->mutex=CreateSemaphore(NULL,1,1,"MUTEX");
    	adrs->data.head=0;
    	adrs->data.tail=0;
    	adrs->data.empty=1;
    	UnmapViewOfFile(pf);//ֹͣ������ڴ�ӳ�� 
    	pf=NULL;
    	CloseHandle(hmap); //�ر�ͬ�� 
    	PROCESS_INFORMATION array2[5];//����һ���������� 
    	for(i=0;i<=4;i++)
    	{
    		array2[i]=jincheng(++e);
		}
		for(i=0;i<=4;i++)
		{
			WaitForSingleObject(array2[i].hProcess,INFINITE);//�ȴ����̽���
		}
		for(i=0;i<=4;i++)
		{
			CloseHandle(array2[i].hProcess);//�رս��̾��
		}
	}
	else if(e==1||e==2)//������
	{
		HANDLE hMap=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"huanchongqu"); //���ļ�ӳ�����ĺ��� 
		LPVOID pf=MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,0);//�ļ�ӳ�����ӳ�䵽��ǰӦ�ó���ĵ�ַ�ռ� 
		if(pf==NULL)//��д������Ҳ�ܰ�ȫ���У����Է���һ����дһ�� 
		{
			cout<<"�����˴��󣡣�"<<endl;
			exit(0);
    	}
    	if(hMap==NULL)//��д������Ҳ�ܰ�ȫ���У����Է���һ����дһ�� 
		{
			cout<<"�����˴��󣡣�"<<endl;
			exit(0);
    	}
		struct gonxiangneirong *adrs=(struct gonxiangneirong*)(pf);
    	adrs->full=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"FULL");
    	adrs->empty=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"EMPTY");
    	adrs->mutex=OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,"MUTEX");	
    	for(i=0;i<=5;i++)
    	{
    		Sleep(suijishu(3));
    		WaitForSingleObject(adrs->empty,INFINITE);//����ȴ� 
    		WaitForSingleObject(adrs->mutex,INFINITE);//���������������
    		ans=suijishu(3);
			adrs->data.array[adrs->data.tail]=ans;
			adrs->data.tail=(adrs->data.tail+1)%3;
			adrs->data.empty=0;
			SYSTEMTIME time;
			GetLocalTime(&time);//��ȡ������Ե�ʱ�� 
			cout<<"���ڵ�ʱ���ǣ�"<<time.wYear<<"��"<<time.wMonth<<"��"<<time.wDay<<"��"<<time.wHour<<"ʱ"<<time.wMinute<<"��"<<time.wSecond<<"��"<<endl; 
			cout<<e-1<<"��������͵͵�ذ�"<<ans<<"���뻺������"<<endl;
			xianshishuju(adrs);
			ReleaseSemaphore(adrs->full,1,NULL);//�ź�������1��ֵ
			ReleaseSemaphore(adrs->mutex,1,NULL);
		}
		UnmapViewOfFile(pf);//ֹͣ��ǰ����pf��һ���ڴ�ӳ�� 
		pf=NULL;
		CloseHandle(hMap);//�ر�ͬ���ں˶��� 
	}
	else if(e==3||e==4||e==5)//������
	{
		HANDLE hMap=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,"huanchongqu"); 
    	LPVOID pf=MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0,0,0);
		if(pf==NULL)//��д������Ҳ�ܰ�ȫ���У����Է���һ����дһ�� 
		{
			cout<<"�����˴��󣡣�"<<endl;
			exit(0);
    	}
    	if(hMap==NULL)//��д������Ҳ�ܰ�ȫ���У����Է���һ����дһ�� 
		{
			cout<<"�����˴��󣡣�"<<endl;
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
			cout<<"���ڵ�ʱ���ǣ�"<<time.wYear<<"��"<<time.wMonth<<"��"<<time.wDay<<"��"<<time.wHour<<"ʱ"<<time.wMinute<<"��"<<time.wSecond<<"��"<<endl; 
			cout<<e-3<<"�������߹�������ذ�"<<ans<<"�ӻ���������"<<endl;
			if(adrs->data.empty==0)
			{
				xianshishuju(adrs);
			} 
			else
			{
				cout<<"��������ɶ��û����"<<endl;
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
