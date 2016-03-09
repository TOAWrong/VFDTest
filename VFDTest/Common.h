#ifndef _COMMON_H_
#define _COMMON_H_

#define  WM_THREAD_STOP 1500
#define	 WM_THREAD_MSG	1501
#define  WM_THREAD_SHUT 1502

#define	 LED_BLUE	1	// 工作状态指示灯，该指示灯与工位共同启停
#define  LED_GREEN	3	// 合格指示灯
#define  LED_RED	2	// 不合格指示灯

#define	 CTRL_ORDER_OPEN 255	// 继电器闭合指令，即打开对应指示灯
#define  CTRL_ORDER_CLOSE 0		// 继电器断开指令，即关闭对应指示灯



#endif