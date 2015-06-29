#ifndef _DEFINE_H
#define _DEFINE_H

/* 
 sizeof(): ��������(��������, ����, ����, �ṹ���)��ռ�ڴ��ֽ���(���ֽ�Ϊ��λ)
 strlen(): �����ַ�������ַ�����(���ֽ�Ϊ��λ), �� "\0" Ϊ�����ж�, ������Ϊ '\0' ������Ԫ��
*/

/* ��������Ԫ�ظ��� array[] */
#define ARR_SIZE(array)		(sizeof(array) /sizeof(*(array)))

/* ����ṹ��type��ĳ����Աmember�Ĵ�С(���ֽ�Ϊ��λ) */
#define struct_member_size(type, member)	(sizeof(((type *)0)->member))

/* ����ṹ��type��Աmember����ڽṹ��type�׵�ַ��ƫ��ֵ(���ֽ�Ϊ��λ) */
#define struct_member_offset(type, member)	((size_t)&(((type *)0)->member))
#define offsetof(type, member)		((size_t)&(((type *)0)->member))

/* ͨ���ṹ��type������ĳ����Աmember���׵�ַptr������øýṹ��������׵�ַ */
#define container_of(ptr, type, member)		\
	({\
		const typeof((((type *)0)->member)) *__mptr = (ptr);\
		(type *)((char *)__mptr - offsetof(type, member));\
	})

/* �õ�ָ����ַ�ϵ�һ���ֽڻ��� */
#define MEM_B(x)		(*((char *)(x)))
#define MEM_W(x)		(*((int *)(x)))

/* �����ֵ����Сֵ */
#define MAX(x, y)		(((x) > (y))?(x):(y))
#define MIN(x, y)		(((x) < (y))?(x):(y))

/* �õ�һ�������ĵ�ַ*/
#define B_PTR(var)		((char *)(void *)&(var))
#define W_PTR(var)		((int *)(void *)&(var))

/* ��һ����ĸת��Ϊ��д*/
#define UPCASE(c)	(((c)>='a' && (c) <= 'z') ? ((c) -0x20) : (c))
#define DWCASE(c)	(((c)>='A' && (c) <= 'Z') ? ((c) + 0x20) : (c))

/*  �ж��ַ��ǲ���10��ֵ������*/
#define  DECCHK(c)	((c)>='0' && (c)<='9') 

/* �ж��ַ��ǲ���16��ֵ������*/
#define HEXCHK(c)	(((c) >= '0' && (c)<='9') || ((c)>='A' && (c)<= 'F') || ((c)>='a' && (c)<='f'))

/* ����LSB��ʽ�������ֽ�ת��Ϊһ��short */
#define FLIPW(ray)	((((short)(ray)[0]) * 256) + (ray)[1]))

/* ����LSB��ʽ��һ��shortת��Ϊ�����ֽ�*/
#define FLOPW(ray, val)	(ray)[0] = ((val)/256); (ray)[1] = ((val) & 0xFF)


/* �õ�һ��short �ĸ�λ�͵�λ�ֽ�*/
#define SHORT_LO(val)	((char)((short)(val) & 255))
#define SHORT_HI(val)		((char)((short)(val) >> 8))

/* ����IO�ռ�ӳ���ڴ洢�ռ�Ľṹ, �����������*/
#define inp(port)			(*((volatile char *)(port)))
#define inpw(port)			(*((volatile short *)(port)))
#define inpdw(port)		(*((volatile long *)(port)))
#define outp(port,val)		(*((volatile char *)(port))=((char)(val)))
#define outpw(port, val)	(*((volatile short*)(port))=((short)(val)))
#define outpdw(port, val)	(*((volatile long *)(port))=((long)(val)))

/* ��¼�ļ��� */
#define _GET_FILE_NAME(f)	#f 
#define GET_FILE_NAME(f)	_GET_FILE_NAME(f) 
/* static char FILE_NAME[] = GET_FILE_NAME(__FILE__); */


/* �õ�һ����ֵ��������Ӧ���ַ��������С  */
#define _TYPE_BUF_SIZE(type)	sizeof #type
#define   TYPE_BUF_SIZE(type)	_TYPE_BUF_SIZE(type)
/* char buf[TYPE_BUF_SIZE(abcd)]; -> char buf[5] */

/* 
 * ���Ժ�:
 * __LINE__ , __FILE__ , __DATE__ , __TIME__ , __FUNC__ , __FUNCTION__ , __STDC__
 */

#endif /* _DEFINE_H */
