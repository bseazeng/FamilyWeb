/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : fw_main.c
  �� �� ��   : ����
  ��    ��   : bseazeng
  ��������   : 2017��12��25�� ����һ
  ����޸�   :
  ��������   : entry of this webserver
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��12��25�� ����һ
    ��    ��   : bseazeng
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "fw_file.h"

struct ARG_
{
	int			connfd; //�ͻ��������ļ�������
	struct		sockaddr_in client;//�ͻ�����Ϣ
	int			server_port; //�������˿�
	int			level;//��־����
	int			sk_buffer_size;//�׽��ֻ��泤��
	int			request_buffer_size;//�ͻ������������
};

typedef struct ARG_  ARG;


