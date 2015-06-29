#ifndef _DEFINE_H
#define _DEFINE_H

/* 
 sizeof(): 计算数据(包括数组, 变量, 类型, 结构体等)所占内存字节数(以字节为单位)
 strlen(): 计算字符数组的字符个数(以字节为单位), 以 "\0" 为结束判断, 不计算为 '\0' 的数组元素
*/

/* 计算数组元素个数 array[] */
#define ARR_SIZE(array)		(sizeof(array) /sizeof(*(array)))

/* 计算结构体type中某个成员member的大小(以字节为单位) */
#define struct_member_size(type, member)	(sizeof(((type *)0)->member))

/* 计算结构体type成员member相对于结构体type首地址的偏移值(以字节为单位) */
#define struct_member_offset(type, member)	((size_t)&(((type *)0)->member))
#define offsetof(type, member)		((size_t)&(((type *)0)->member))

/* 通过结构体type变量中某个成员member的首地址ptr进而获得该结构体变量的首地址 */
#define container_of(ptr, type, member)		\
	({\
		const typeof((((type *)0)->member)) *__mptr = (ptr);\
		(type *)((char *)__mptr - offsetof(type, member));\
	})

/* 得到指定地址上的一个字节或字 */
#define MEM_B(x)		(*((char *)(x)))
#define MEM_W(x)		(*((int *)(x)))

/* 求最大值和最小值 */
#define MAX(x, y)		(((x) > (y))?(x):(y))
#define MIN(x, y)		(((x) < (y))?(x):(y))

/* 得到一个变量的地址*/
#define B_PTR(var)		((char *)(void *)&(var))
#define W_PTR(var)		((int *)(void *)&(var))

/* 将一个字母转换为大写*/
#define UPCASE(c)	(((c)>='a' && (c) <= 'z') ? ((c) -0x20) : (c))
#define DWCASE(c)	(((c)>='A' && (c) <= 'Z') ? ((c) + 0x20) : (c))

/*  判断字符是不是10进值的数字*/
#define  DECCHK(c)	((c)>='0' && (c)<='9') 

/* 判断字符是不是16进值的数字*/
#define HEXCHK(c)	(((c) >= '0' && (c)<='9') || ((c)>='A' && (c)<= 'F') || ((c)>='a' && (c)<='f'))

/* 按照LSB格式把两个字节转化为一个short */
#define FLIPW(ray)	((((short)(ray)[0]) * 256) + (ray)[1]))

/* 按照LSB格式把一个short转化为两个字节*/
#define FLOPW(ray, val)	(ray)[0] = ((val)/256); (ray)[1] = ((val) & 0xFF)


/* 得到一个short 的高位和低位字节*/
#define SHORT_LO(val)	((char)((short)(val) & 255))
#define SHORT_HI(val)		((char)((short)(val) >> 8))

/* 对于IO空间映射在存储空间的结构, 输入输出处理*/
#define inp(port)			(*((volatile char *)(port)))
#define inpw(port)			(*((volatile short *)(port)))
#define inpdw(port)		(*((volatile long *)(port)))
#define outp(port,val)		(*((volatile char *)(port))=((char)(val)))
#define outpw(port, val)	(*((volatile short*)(port))=((short)(val)))
#define outpdw(port, val)	(*((volatile long *)(port))=((long)(val)))

/* 记录文件名 */
#define _GET_FILE_NAME(f)	#f 
#define GET_FILE_NAME(f)	_GET_FILE_NAME(f) 
/* static char FILE_NAME[] = GET_FILE_NAME(__FILE__); */


/* 得到一个数值类型所对应的字符串缓冲大小  */
#define _TYPE_BUF_SIZE(type)	sizeof #type
#define   TYPE_BUF_SIZE(type)	_TYPE_BUF_SIZE(type)
/* char buf[TYPE_BUF_SIZE(abcd)]; -> char buf[5] */

/* 
 * 调试宏:
 * __LINE__ , __FILE__ , __DATE__ , __TIME__ , __FUNC__ , __FUNCTION__ , __STDC__
 */

#endif /* _DEFINE_H */
