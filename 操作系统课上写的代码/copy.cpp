#include <stdio.h>
#include<iostream>
#include <string.h>
#include <windows.h>
#include <stdlib.h>

using namespace std;
void fuzhi_wenjian(char *yuanwenjian,char *mubiaowenjian) //�����ļ�����Ҫʵ�ֵĹ����Ǳ����ļ�����ȡ�����ļ����ԣ�ʱ�䣬��д�����ļ���С�� 
{
	HANDLE yuan=CreateFile(yuanwenjian,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	//��һ���Ǵ����ļ������������ʣ�������ȫ����ô���������ԣ����
	HANDLE mubiao=CreateFile(mubiaowenjian,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	WIN32_FIND_DATA wenjian_liebiao;//ͨ������ļ����ݻ�ȡ�ļ�����Ϣ 
	LONG daxiao=wenjian_liebiao.nFileSizeLow-wenjian_liebiao.nFileSizeHigh; //�����ļ��Ĵ�С 
	int *huanchongqu=new int[daxiao];//�������С��ʹ���һ����С����Ļ����� 
	DWORD a;//����һ����ʱ�ı��� 
	ReadFile(yuan,huanchongqu,daxiao,&a,NULL);//��ʼ��ȡ�ļ� 
	WriteFile(mubiao,huanchongqu,daxiao,&a,NULL);//�����ļ� ����һ����Ǹ����ļ�����te���ļ����Ƶ�te2�ļ��� 
	HANDLE faxian_wenjian=FindFirstFile(yuanwenjian,&wenjian_liebiao);//�ҵ���һ��Դ�ļ� 
	SetFileTime(mubiao,&wenjian_liebiao.ftCreationTime,&wenjian_liebiao.ftLastAccessTime,&wenjian_liebiao.ftLastWriteTime);//��һ��ʵ��ʱ�����Ա���һ��
	CloseHandle(yuan);	// �ص��ļ�����yuan 
	CloseHandle(mubiao);
	CloseHandle(faxian_wenjian); //�رվ��
}

void fuzhi_mulu(char *yuanwenjian,char *mubiaowenjian) //����Ŀ¼��Ҳ�������ļ��� 
{
	WIN32_FIND_DATA wenjian_liebiao;
	char source[10086],object[10086];//������������ֱ����Դ�ļ���Ŀ���ļ������� 
	strcpy(object,mubiaowenjian);// 
	strcpy(source,yuanwenjian);//��Դ�ļ�������ݸ��Ƶ�source�� 
	strcat(object,"\\");//��ʾ��Ŀ¼ ����ʾƴ�ӵ�object�� 
	strcat(source,"\\*.*");//��ʾ��Ŀ¼�����ļ���*��ͨ�����ͨ�������ģ�������õ����ַ� �����������˼����xxx.xx�ļ� 
	HANDLE faxian_wenjian=FindFirstFile(source,&wenjian_liebiao);
	while(FindNextFile(faxian_wenjian,&wenjian_liebiao)!=0) //��ʼ���������ļ���һֱ��û���ļ���ʱ����� 
	{
		if(wenjian_liebiao.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)//�����Ŀ¼�ļ��еĻ� 
		{ 
			if(strcmp(wenjian_liebiao.cFileName,"..")!=0&&strcmp(wenjian_liebiao.cFileName,".")!=0)//������Ǹ�Ŀ¼Ҳ���ǵ�ǰĿ¼�Ļ�
			{ 
				memset(source,0,sizeof(source));//�Ȱ�source���������� 
				strcpy(source,yuanwenjian);//���� 
				strcat(source,"\\");//ƴ�ӵ���ǰ�ļ��� 
				strcat(source,wenjian_liebiao.cFileName);
				strcat(object,wenjian_liebiao.cFileName);
				CreateDirectory(object,NULL);//Ȼ�󴴽�Ŀ¼ 
				fuzhi_mulu(source,object); //�ݹ�,������Ŀ¼����������ע���ļ�������ʱ��Ҫһ�� ������Ĵ���ִ�в���������һ������Ϊ��������Ŀ¼�����Զ�Ҫ���´��� 
				HANDLE yuan_wenjian=CreateFile(source,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
				HANDLE mubiao_wenjian=CreateFile(object,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
				FILETIME chuangjian_shijian,fangwen_shijian,xiugai_shijian;//���ļ��Ĵ���ʱ�䣬����ʱ����޸�ʱ�� 
				GetFileTime(yuan_wenjian,&chuangjian_shijian,&fangwen_shijian,&xiugai_shijian);//��ȡ��Щʱ����Ϣ 
				SetFileTime(mubiao_wenjian,&chuangjian_shijian,&fangwen_shijian,&xiugai_shijian); //�޸��ļ�ʱ����Ϣ 
				strcpy(object,mubiaowenjian);//��Ŀ���ļ�������ȥ 
				strcat(object,"\\");//ƴ��object 
			}
		}
		else //������ļ��Ļ�
		{
			memset(source,0,sizeof(source));
			strcpy(source,yuanwenjian);
			strcat(source,"\\");
			strcat(source,wenjian_liebiao.cFileName);
			strcat(object,wenjian_liebiao.cFileName);
			fuzhi_wenjian(source,object);
			strcpy(object,mubiaowenjian);
			strcat(object,"\\");
		}
	}
	CloseHandle(faxian_wenjian);//�رվ�� 
}

int main(int argc,char *argv[])
{
	WIN32_FIND_DATA wenjian_liebiao;
	if(FindFirstFile(argv[1],&wenjian_liebiao)==INVALID_HANDLE_VALUE)//�����ļ�����Ҫ���ǿ���û�и��ļ��� argv1������Ҫ�����ļ����ļ��� 
	{
		cout<<"�����ļ���������"<<endl;
		exit(0);
	}
	if(FindFirstFile(argv[2],&wenjian_liebiao)==INVALID_HANDLE_VALUE)// argv2�����뱻�����ļ����ļ��� 
	{
		CreateDirectory(argv[2],NULL); //�����ļ����ļ�
		cout<<"�ļ������ļ������ɹ���"<<endl;
	}
	fuzhi_mulu(argv[1],argv[2]);
	//����Ŀ¼ʱ������һ��
	HANDLE yuan_wenjian=CreateFile(argv[1],GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	HANDLE mubiao_wenjian=CreateFile(argv[2],GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	FILETIME chuangjian_shijian,fangwen_shijian,xiugai_shijian;
	GetFileTime(yuan_wenjian,&chuangjian_shijian,&fangwen_shijian,&xiugai_shijian);
	SetFileTime(mubiao_wenjian,&chuangjian_shijian,&fangwen_shijian,&xiugai_shijian);
	cout<<"�ɹ����ƣ�"<<endl;
	return 0;
}
