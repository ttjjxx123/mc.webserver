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
	int isok = WSAStartup(MAKEWORD(2, 2), &wsdata);//ȷ��socket�汾��Ϣ
	merror(isok, WSAEINVAL, "����socketʧ��");

	SOCKET sever = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	merror(sever, INVALID_SOCKET, "����socketʧ��");

	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;//�ʹ���ʱһ��
	seraddr.sin_port = htons(25565);
	seraddr.sin_addr.s_addr = INADDR_ANY;
	isok = bind(sever, &seraddr, sizeof(seraddr));
	merror(isok, SOCKET_ERROR, "��ʧ��\n");

	isok = listen(sever, 5);
	merror(isok, SOCKET_ERROR, "����ʧ��\n");//����

	struct sockaddr_in claddr;
	int cllen = sizeof(claddr);
	while (1)
	{
		SOCKET client = accept(sever, &claddr, &cllen);
		merror(client, INVALID_SOCKET, "����ʧ��\n");

		char revdata[1024] = "";
		recv(client, revdata, 1024, 0);
		printf("%s �����յ�%zd�ֽ�����\n", revdata, strlen(revdata));
		
		//�����û���Ϣ�Թ��ͻ��˵�¼
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

		//�����û�����ע����û���������
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
		}//�Ƿ�رշ���������ĳ��������ɺ���b�ر�
	}
	closesocket(sever);
	WSACleanup();//�ͷŽӿ�

	getchar();
	return 0;
}
