#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;
int main(int argc,char **argv)
{
    SYSTEMTIME time_s,time_e;//s�ǿ�ʼ��e�ǽ�������ʱ���ð�����������������ʱ��
	int hou,min,sec,msec;//������ʱ�������
	STARTUPINFO str;//�ṹ��
	memset(&str,0,sizeof(STARTUPINFO));//��ʼ
	str.cb=sizeof(STARTUPINFO);
	str.dwFlags=STARTF_USESHOWWINDOW;
	str.wShowWindow=SW_SHOW;
	PROCESS_INFORMATION end;

	
	if(!CreateProcess(argv[1],argv[2],NULL,NULL,FALSE,0,NULL,NULL,&str,&end))
	{
		cout<<"����һ������ɽ�������˲ر���!"<<endl;
		exit(1);
	}
	else
	{
		
		GetSystemTime(&time_s);
		cout<<"����һ������ɽ�������˲ر���!"<<endl;
		cout<<"����һ����׼����ʼ�Ա��������������ʼʱ��: "<<time_s.wHour+8<<"ʱ"<<time_s.wMinute<<"��"<<time_s.wSecond<<"��"<<time_s.wMilliseconds<<"����"<<endl;
		cout<<"���߰ѱ����ÿ��ˣ��ر�������ɶ��û��......"<<endl;//ʱ��ʱ��Ӱ˸�Сʱ�����й�ʱ�� 
	 } 
	 	WaitForSingleObject(end.hProcess, INFINITE);
	
	GetSystemTime(&time_e);
	cout<<"����һ���˻���������ˣ�����ʱ��: "<<time_e.wHour+8<<"ʱ"<<time_e.wMinute<<"��"<<time_e.wSecond<<"��"<<time_e.wMilliseconds<<"����"<<endl;
	
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
	cout<<"����������ʱ��: "<<endl;
	if(hou>0)
	{
		cout<<hou+8<<"ʱ";
	}
	if(min>0)
	{
		cout<<min<<"��";
	}
	if(sec>0)
	{
		cout<<sec<<"��";
	}
	if(msec>0)
	{
		cout<<msec<<"����";
	}
	cout<<endl;
	return 0;
}

