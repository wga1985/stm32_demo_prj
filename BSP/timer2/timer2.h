#ifndef __TIMER2_H
#define __TIMER2_H

/*-------------------------------------------------------------------------------------------------------*/

typedef enum
{
	TimerEvId_ID0,
	TimerEvId_ID1,
	TimerEvId_ID2,
	TimerEvId_ID3,
	TimerEvId_ID4,
	TimerEvId_IDMAX
}TimerEvId;

/*-------------------------------------------------------------------------------------------------------*/

/* ���index��Ӧ��ʱ�䳬ʱ�����㲢����1��δ��ʱ����0 */
#define IS_TIMEOUT_NMS(index, nms)	((g_Tim2Array[(unsigned char)(index)] >= (nms))? ((g_Tim2Array[(unsigned char)(index)] = 0) == 0) : 0)

extern volatile unsigned int g_Tim2Array[(unsigned char)TimerEvId_IDMAX];

/*-------------------------------------------------------------------------------------------------------*/

typedef unsigned int TimerDelay;
typedef struct _tag_timer_arg
{
	TimerEvId eventId;
	TimerDelay timerdelay; //ms
	unsigned int timerTick; //ms
}TimerArg_t;

typedef void (*TimerCallbackFunc)(TimerArg_t timer); //����ص�����ָ��
typedef struct _tag_timer
{
	TimerArg_t timerarg;
	TimerCallbackFunc callbackfunc;
}Timer_t;

/*-------------------------------------------------------------------------------------------------------*/

int timer2_callback_register(Timer_t timer);
int timer2_callback_unregister(Timer_t timer);

void timer2_init(unsigned short psc, unsigned short arr);
void timer2_open(void);
void timer2_close(void);

#endif /* __TIMER2_H */

