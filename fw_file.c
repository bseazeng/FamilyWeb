/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : fw_file.c
  版 本 号   : 初稿
  作    者   : bseazeng
  生成日期   : 2017年12月25日 星期一
  最近修改   :
  功能描述   : to provide base function for using
  函数列表   :
  修改历史   :
  1.日    期   : 2017年12月25日 星期一
    作    者   : bseazeng
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include<stdio.h>
#include "file.h"

/*****************************************************************************
 函 数 名  : readFile(char *filename, file_list head)
 功能描述  : 从磁盘文件读取道内存中
 输入参数  : 无
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年12月28日 星期四
    作    者   : bseazeng
    修改内容   : 新生成函数

*****************************************************************************/
int readFile(char *filename, file_list file_head)
{	
	struct list_head *tmp=NULL;
	file_list *filelist_tmp = NULL;
	value_list *valuelist_tmp = NULL;
	int filename_len = strlen(filename);
	int file_name_len_tmp = 0;
	int file_has_read_flag = 0;

	FILE *fp = NULL;
	
	list_for_each(tmp,file_head.next_file)
	{
		filelist_tmp =list_entry(tmp,file_list,next_file);
		if(NULL != filelist_tmp)
		{
		    file_name_len_tmp = strlen(filelist_tmp->filename);
			if ( filename_len != file_name_len_tmp )
			{
			    continue;
			}

			if ( 0 != strncmp(filename,filelist_tmp->filename,filename_len) )
			{
			    continue;
			}
			
			file_has_read_flag = 1;//文件已读取到内存中
			break;
		}
		
	}

	if ( 1 == file_has_read_flag )
	{
		printf("[%s][%d]:file has read.\n",__FILE__,__LINE__);
	    return RETURN_OK;
	}

	filelist_tmp = (file_list*)malloc(sizeof(file_list));
	CHECK_PTR_RETURN_INT(filelist_tmp);
	filelist_tmp->filename = (char*)malloc(filename_len+1);
	if ( NULL == filelist_tmp->filename)
	{
	    free(filelist_tmp);
		printf("[%s][%d]:null pointer.\n",__FILE__,__LINE);
		return RETURN_NULL;
	}
	memcpy(filelist_tmp->filename,filename,filename_len);
	filelist_tmp->filename[filename_len] = '\0';

	INIT_LIST_HEAD(&(filelist_tmp->content));
	INIT_LIST_HEAD(&(filelist_tmp->next_file));	
	list_add(&(filelist_tmp->next_file),&(file_head.next_file));

	valuelist_tmp = (value_list*)malloc(sizeof(value_list));
	CHECK_PTR_RETURN_INT(valuelist_tmp);
	readValueList();
	
}
/*****************************************************************************
 函 数 名  : getStrings
 功能描述  : 从file中获取key的取值
 输入参数  : char *key
 输出参数  : 无
 返 回 值  : char *
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年12月27日 星期三
    作    者   : bseazeng
    修改内容   : 新生成函数

*****************************************************************************/
char * getStrings( char *key ,char *filename)
{
    FILE *fp = NULL;
	char line_buff[FILE_LINE_BUF_SIZE]={0};
	char line_key[FILE_LINE_BUF_SIZE]={0};
	char *line_value = NULL;
	int key_len = 0; //key长度
	int key_line_len = 0; //一行内容中获取的key的长度

	CHECK_PTR_RETURN_NULL(key);
	key_len = strlen(key);
	
	CHECK_PTR_RETURN_NULL(filename);
	fp	=	fopen(filename,"r");
	CHECK_PTR_RETURN_NULL(fp);
	while( !feof(fp) ) 
	{  
        if(	NULL != fgets( line_buff,FILE_LINE_BUF_SIZE-1,fp ) ) 
    	{
    	    printf("%s",line_buff);

			//判读注释行
			if (isComment(line_buff))
				continue;
			//获取该行的key
			getLineKey(line_buff,&line_key);
			//判读该行key是否与入参key相同,先判读长度
			key_line_len=strlen(line_key);
			if( key_len != key_line_len )
				continue;
			if( 0 != strncmp(key,line_key,key_len) )
				continue;
			
			line_value = (char*)malloc(FILE_LINE_BUF_SIZE-key_line_len);
			CHECK_PTR_RETURN_NULL(line_value);
			memset(line_value,0,FILE_LINE_BUF_SIZE-key_line_len);
			getLineValue(line_buff,&line_value);
			return line_value;
    	}
    	 
    }  
	
}
