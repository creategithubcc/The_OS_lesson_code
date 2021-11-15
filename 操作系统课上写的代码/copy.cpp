#include <stdio.h>
#include<iostream>
#include <string.h>
#include <windows.h>
#include <stdlib.h>

using namespace std;
void fuzhi_wenjian(char *yuanwenjian,char *mubiaowenjian) //复制文件，我要实现的功能是遍历文件，获取更改文件属性（时间，读写），文件大小等 
{
	HANDLE yuan=CreateFile(yuanwenjian,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	//这一块是创建文件对象名，访问，共享，安全，怎么创建，属性，句柄
	HANDLE mubiao=CreateFile(mubiaowenjian,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	WIN32_FIND_DATA wenjian_liebiao;//通过这个文件数据获取文件的信息 
	LONG daxiao=wenjian_liebiao.nFileSizeLow-wenjian_liebiao.nFileSizeHigh; //计算文件的大小 
	int *huanchongqu=new int[daxiao];//计算出大小后就创建一个大小相符的缓冲区 
	DWORD a;//定义一个临时的变量 
	ReadFile(yuan,huanchongqu,daxiao,&a,NULL);//开始读取文件 
	WriteFile(mubiao,huanchongqu,daxiao,&a,NULL);//创建文件 ，这一块就是复制文件，把te的文件复制到te2文件夹 
	HANDLE faxian_wenjian=FindFirstFile(yuanwenjian,&wenjian_liebiao);//找到第一个源文件 
	SetFileTime(mubiao,&wenjian_liebiao.ftCreationTime,&wenjian_liebiao.ftLastAccessTime,&wenjian_liebiao.ftLastWriteTime);//这一块实现时间属性保持一致
	CloseHandle(yuan);	// 关掉文件对象yuan 
	CloseHandle(mubiao);
	CloseHandle(faxian_wenjian); //关闭句柄
}

void fuzhi_mulu(char *yuanwenjian,char *mubiaowenjian) //复制目录，也就是子文件夹 
{
	WIN32_FIND_DATA wenjian_liebiao;
	char source[10086],object[10086];//创建两个数组分别代表源文件与目标文件的数组 
	strcpy(object,mubiaowenjian);// 
	strcpy(source,yuanwenjian);//把源文件里的数据复制到source里 
	strcat(object,"\\");//表示该目录 ，表示拼接到object里 
	strcat(source,"\\*.*");//表示该目录所有文件，*是通配符，通配符就是模糊搜索用到的字符 ，在这里的意思就是xxx.xx文件 
	HANDLE faxian_wenjian=FindFirstFile(source,&wenjian_liebiao);
	while(FindNextFile(faxian_wenjian,&wenjian_liebiao)!=0) //开始遍历所有文件，一直到没有文件的时候结束 
	{
		if(wenjian_liebiao.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)//如果是目录文件夹的话 
		{ 
			if(strcmp(wenjian_liebiao.cFileName,"..")!=0&&strcmp(wenjian_liebiao.cFileName,".")!=0)//如果不是父目录也不是当前目录的话
			{ 
				memset(source,0,sizeof(source));//先把source里的数据清空 
				strcpy(source,yuanwenjian);//拷贝 
				strcat(source,"\\");//拼接到当前文件夹 
				strcat(source,wenjian_liebiao.cFileName);
				strcat(object,wenjian_liebiao.cFileName);
				CreateDirectory(object,NULL);//然后创建目录 
				fuzhi_mulu(source,object); //递归,进入子目录继续拷贝，注意文件的属性时间要一样 ，下面的代码执行操作和上面一样，因为进到了子目录里所以都要重新创建 
				HANDLE yuan_wenjian=CreateFile(source,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
				HANDLE mubiao_wenjian=CreateFile(object,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
				FILETIME chuangjian_shijian,fangwen_shijian,xiugai_shijian;//该文件的创建时间，访问时间和修改时间 
				GetFileTime(yuan_wenjian,&chuangjian_shijian,&fangwen_shijian,&xiugai_shijian);//获取这些时间信息 
				SetFileTime(mubiao_wenjian,&chuangjian_shijian,&fangwen_shijian,&xiugai_shijian); //修改文件时间信息 
				strcpy(object,mubiaowenjian);//把目标文件拷贝过去 
				strcat(object,"\\");//拼接object 
			}
		}
		else //如果是文件的话
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
	CloseHandle(faxian_wenjian);//关闭句柄 
}

int main(int argc,char *argv[])
{
	WIN32_FIND_DATA wenjian_liebiao;
	if(FindFirstFile(argv[1],&wenjian_liebiao)==INVALID_HANDLE_VALUE)//查找文件，主要还是看有没有该文件夹 argv1是输入要复制文件的文件夹 
	{
		cout<<"查找文件发生错误！"<<endl;
		exit(0);
	}
	if(FindFirstFile(argv[2],&wenjian_liebiao)==INVALID_HANDLE_VALUE)// argv2是输入被复制文件的文件夹 
	{
		CreateDirectory(argv[2],NULL); //创建文件夹文件
		cout<<"文件夹里文件创建成功！"<<endl;
	}
	fuzhi_mulu(argv[1],argv[2]);
	//设置目录时间属性一致
	HANDLE yuan_wenjian=CreateFile(argv[1],GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	HANDLE mubiao_wenjian=CreateFile(argv[2],GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	FILETIME chuangjian_shijian,fangwen_shijian,xiugai_shijian;
	GetFileTime(yuan_wenjian,&chuangjian_shijian,&fangwen_shijian,&xiugai_shijian);
	SetFileTime(mubiao_wenjian,&chuangjian_shijian,&fangwen_shijian,&xiugai_shijian);
	cout<<"成功复制！"<<endl;
	return 0;
}
