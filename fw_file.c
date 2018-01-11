/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : fw_file.c
  �� �� ��   : ����
  ��    ��   : bseazeng
  ��������   : 2017��12��25�� ����һ
  ����޸�   :
  ��������   : to provide base function for using
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��12��25�� ����һ
    ��    ��   : bseazeng
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include<stdio.h>
#include "file.h"

/*****************************************************************************
 �� �� ��  : readFile(char *filename, file_list head)
 ��������  : �Ӵ����ļ���ȡ���ڴ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��12��28�� ������
    ��    ��   : bseazeng
    �޸�����   : �����ɺ���

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
			
			file_has_read_flag = 1;//�ļ��Ѷ�ȡ���ڴ���
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
 �� �� ��  : getStrings
 ��������  : ��file�л�ȡkey��ȡֵ
 �������  : char *key
 �������  : ��
 �� �� ֵ  : char *
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��12��27�� ������
    ��    ��   : bseazeng
    �޸�����   : �����ɺ���

*****************************************************************************/
char * getStrings( char *key ,char *filename)
{
    FILE *fp = NULL;
	char line_buff[FILE_LINE_BUF_SIZE]={0};
	char line_key[FILE_LINE_BUF_SIZE]={0};
	char *line_value = NULL;
	int key_len = 0; //key����
	int key_line_len = 0; //һ�������л�ȡ��key�ĳ���

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

			//�ж�ע����
			if (isComment(line_buff))
				continue;
			//��ȡ���е�key
			getLineKey(line_buff,&line_key);
			//�ж�����key�Ƿ������key��ͬ,���ж�����
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
