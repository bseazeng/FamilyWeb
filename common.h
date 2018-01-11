/******************************************************************************

                  ��Ȩ���� (C), 2016-2200, ����bsea

 ******************************************************************************
  �� �� ��   : common.h
  �� �� ��   : ����
  ��    ��   : bseazeng
  ��������   : Saturday, July 23, 2016
  ����޸�   :
  ��������   : common.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : Saturday, July 23, 2016
    ��    ��   : bseazeng
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define RETURN_TYPE                 (0)
#define RETURN_ERR                  (RETURN_TYPE - 1)
#define RETURN_OK                   (RETURN_TYPE + 0)
#define RETURN_NULL                 (RETURN_TYPE + 1) //ָ���
#define RETURN_BUFFER_NOT_ENOUGH    (RETURN_TYPE + 2) //BUFFER���Ȳ���
#define RETURN_ERROR_BUFFER         (RETURN_TYPE + 3) //���յ���Ϣ����
#define RETURN_FILLED               (RETURN_TYPE + 4) //�Ѿ����

#define CHECK_PTR_RETURN_NULL(p)	\
	(\
		do\
		{\
			if (0 == p)\
			{\	
				printf("[%s][%d]:null pointer.\n",__FILE__,__LINE__);\
				return NULL;\
			}\				
		}while(0);\
	)

#define CHECK_PTR_RETURN_INT(p)	\
	(\
		do\
		{\
			if (0 == p)\
			{\	
				printf("[%s][%d]:null pointer.\n",__FILE__,__LINE__);\
				return RETURN_NULL;\
			}\				
		}while(0);\
	)

#define CHECK_PTR_RETURN(p)	\
	(\
		do\
		{\
			if (0 == p)\
			{\	
				printf("[%s][%d]:null pointer.\n",__FILE__,__LINE__);\
				return;\
			}\				
		}while(0);\
	)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __COMMON_H__ */
