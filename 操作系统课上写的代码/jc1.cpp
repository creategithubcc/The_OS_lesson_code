#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;
int main(int argc,char **argv)
{
    SYSTEMTIME time_s,time_e;//s是开始，e是结束，到时候用把这两个相减，求具体时间
	int hou,min,sec,msec;//年月日时分秒毫秒
	STARTUPINFO str;//结构体
	memset(&str,0,sizeof(STARTUPINFO));//初始
	str.cb=sizeof(STARTUPINFO);
	str.dwFlags=STARTF_USESHOWWINDOW;
	str.wShowWindow=SW_SHOW;
	PROCESS_INFORMATION end;

	
	if(!CreateProcess(argv[1],argv[2],NULL,NULL,FALSE,0,NULL,NULL,&str,&end))
	{
		cout<<"勇者一行人在山洞发现了藏宝箱!"<<endl;
		exit(1);
	}
	else
	{
		
		GetSystemTime(&time_s);
		cout<<"勇者一行人在山洞发现了藏宝箱!"<<endl;
		cout<<"勇者一行人准备开始对宝箱进行撬锁，开始时间: "<<time_s.wHour+8<<"时"<<time_s.wMinute<<"分"<<time_s.wSecond<<"秒"<<time_s.wMilliseconds<<"毫秒"<<endl;
		cout<<"勇者把宝箱敲开了！藏宝箱里面啥都没有......"<<endl;//时序时间加八个小时就是中国时间 
	 } 
	 	WaitForSingleObject(end.hProcess, INFINITE);
	
	GetSystemTime(&time_e);
	cout<<"勇者一行人灰溜溜地走了，结束时间: "<<time_e.wHour+8<<"时"<<time_e.wMinute<<"分"<<time_e.wSecond<<"秒"<<time_e.wMilliseconds<<"毫秒"<<endl;
	
	msec=time_e.wMilliseconds-time_s.wMilliseconds;
	sec=time_e.wSecond-time_s.wSecond;
	min=time_e.wMinute-time_s.wMinute;
	hou=time_e.wHour-time_s.wHour;

    if(msec<0)
	{
		sec--;
		msec+=1000;
	}
	if(sec<0)
	{
		min--;
		sec+=60;
	}
	if(min<0)
	{
		hou--;
		min+=60;
	}
	cout<<endl;
	cout<<"这个程序结束时间: "<<endl;
	if(hou>0)
	{
		cout<<hou+8<<"时";
	}
	if(min>0)
	{
		cout<<min<<"分";
	}
	if(sec>0)
	{
		cout<<sec<<"秒";
	}
	if(msec>0)
	{
		cout<<msec<<"毫秒";
	}
	cout<<endl;
	return 0;
}

