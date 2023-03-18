#include<stdio.h>
#include<WinSock2.h>
#include<conio.h>
#include"login.h"
#pragma comment(lib, "ws2_32.lib")
int merror(int redata, int error, char* showinfo)
{
	if (redata == error)
	{
		perror(showinfo);
		getchar();
		return -1;
	}
	return 0;
}
void sendfile(SOCKET s, char* filename);
int main()
{
	printf("welcome\n");
	WSADATA wsdata;
	int isok = WSAStartup(MAKEWORD(2, 2), &wsdata);//确定socket版本信息
	merror(isok, WSAEINVAL, "申请socket失败");

	SOCKET sever = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	merror(sever, INVALID_SOCKET, "创建socket失败");

	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;//和创建时一样
	seraddr.sin_port = htons(25565);
	seraddr.sin_addr.s_addr = INADDR_ANY;
	isok = bind(sever, &seraddr, sizeof(seraddr));
	merror(isok, SOCKET_ERROR, "绑定失败\n");

	isok = listen(sever, 5);
	merror(isok, SOCKET_ERROR, "监听失败\n");//监听

	struct sockaddr_in claddr;
	int cllen = sizeof(claddr);
	while (1)
	{
		SOCKET client = accept(sever, &claddr, &cllen);
		merror(client, INVALID_SOCKET, "连接失败\n");

		char revdata[1024] = "";
		recv(client, revdata, 1024, 0);
		printf("%s 共接收到%zd字节数据\n", revdata, strlen(revdata));
		
		//发送用户信息以供客户端登录
		char sendata[1024] = "";
		FILE* fp = fopen("userdata.txt", "r");
		if (!fp)
		{
			printf("failed when opening the file\n");
			return -1;
		}
		fread(sendata, sizeof(char), 1024, fp);
			send(client, sendata, 1024, 0);
		if (fclose(fp))
		{
			printf("Error in closing file\n");
			exit(-1);
		}

		//接收用户端新注册的用户名及密码
		char buf[1024];
		fp = fopen("userdata.txt", "w");
		if (!fp)
		{
			printf("failed when opening the file\n");
			return -1;
		}
			recv(client, buf, 1024, 0);
			fputs(buf, fp);
			fprintf(fp, "\n");
		if (fclose(fp))
			{
				printf("Error in closing file\n");
				exit(-1);
			}
		if (_kbhit() != 0)
		{
			char ch;
			ch = _getch();
			if (ch == 'b')
				break;
		}//是否关闭服务器，在某次连接完成后按下b关闭
	}
	closesocket(sever);
	WSACleanup();//释放接口

	getchar();
	return 0;
}
