/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : fw_main.c
  版 本 号   : 初稿
  作    者   : bseazeng
  生成日期   : 2017年12月25日 星期一
  最近修改   :
  功能描述   : entry of this webserver
  函数列表   :
  修改历史   :
  1.日    期   : 2017年12月25日 星期一
    作    者   : bseazeng
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
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
	int			connfd; //客户端连接文件描述符
	struct		sockaddr_in client;//客户端信息
	int			server_port; //服务器端口
	int			level;//日志级别
	int			sk_buffer_size;//套接字缓存长度
	int			request_buffer_size;//客户端请求命令长度
};

typedef struct ARG_  ARG;


