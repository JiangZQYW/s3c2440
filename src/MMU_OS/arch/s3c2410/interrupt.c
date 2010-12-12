#include "s3c2410.h"
#include "interrupt.h"
#include "serial.h"
#include "sched.h"

void IRQ_Handle()
{
	unsigned long oft = INTOFFSET;
	switch( oft )
	{
		case EINT1_OFT:	printk("EINT1, K1 pressed!\n\r");	break;
		case EINT2_OFT:	printk("EINT2, K2 pressed!\n\r");	break;
		case EINT3_OFT:	printk("EINT3, K3 pressed!\n\r");	break;
		case EINT4_7_OFT:	printk("EINT7, K4 pressed!\n\r");	break;
		case INT_TIMER0_OFT:	do_timer();			break;
		default:		printk("Interrupt unknown!\n\r");	break;
			
	}

	//���ж�
	if( oft == 4 ) EINTPEND = 1<<7;		//EINT4-7����IRQ4��ע��EINTPEND[3:0]����δ�ã�����Щλд��1���ܵ���δ֪���
	SRCPND = 1<<oft;	
	INTPND	= INTPND;	 
}

void OS_ENTER_CRITICAL()
{
__asm__(
	"mrs	r0,	cpsr\n"	/*��cpsr*/
	"orr	r0, r0, 	#0x80\n"	/*I-bit=1*/
	"msr	cpsr_c,	r0\n"	/*����I-bit=1����ֹIRQ�ж�*/
	);
	
}

void OS_EXIT_CRITICAL()
{
__asm__(
	"mrs	r0,	cpsr\n"	/*��cpsr*/
	"bic	r0, r0, 	#0x80\n"	/*I-bit=0*/
	"msr	cpsr_c,	r0\n"	/*����I-bit=0����IRQ�ж�*/
	);
 
}

