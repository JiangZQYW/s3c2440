#include "sched.h"
#include "s3c2410.h"
#include "interrupt.h"
#include "init.h"
#include "mmu.h"
#include "string.h"
#include "serial.h"

struct task_struct task[TASK_NR];
struct task_struct*  current = NULL;
extern void __switch_to(struct task_struct *pcur,struct task_struct *pnext);


/*****************************************************************************
* ��ʼ��task����
* ��ʼ������0,��task[0]
*****************************************************************************/
void sched_init(void)
{
	struct task_struct *p = &task[0];
	int i;

	DPRINTK(KERNEL_DEBUG,"\n\rkernel:sched_init,create task 0\n\r");
	
	for(i = 0; i < TASK_NR; i++,p++){
		p->pid = -1;
		p->state = TASK_UNALLOCATE;
		p->count = 0;
		p->priority = 0;
	}
	p = &task[0];
	p->pid = 0;
	p->state = TASK_RUNNING;
	p->count = 5;
	p->priority = 5;	
	current = &task[0];
}

/*****************************************************************************
* �л���task�����д�������״̬�ġ�countֵ��������
* ������������countֵ��Ϊ0,�����¼���count = count / 2 + priority
* ����0�����뾺��
*****************************************************************************/
void schedule()
{
	long max = 0;
	long i = 0, j = 0, next = 0, no_running_tsk = 0;
	struct task_struct * ptmp_tsk = NULL;

	DPRINTK(KERNEL_DEBUG,"kernel:schedule\n\r");	

	while(1){
		for(i = 0; i < TASK_NR; i++){
			if((task[i].state == TASK_RUNNING)&&(max < task[i].count)){
				max = task[i].count;
				next = i;
			}
		}

		if(max)break;
//		if(no_running_tsk)break;
//		no_running_tsk = 1;

		for(i = 0; i < TASK_NR; i++){
			task[i].count = (task[i].count >> 1) + task[i].priority;
		}
	}
	if(current == &task[next])
		return;
	if(current->pid < 0 || task[next].pid < 0)
		return;
	ptmp_tsk = current;
	current = &task[next];
	DPRINTK(KERNEL_DEBUG,"kernel:__switch_to\n\r");		
	__switch_to(ptmp_tsk,&task[next]);
}



/*****************************************************************************
* ��ǰ����count��1
* �����ǰ���̲����ڿ�����״̬����count=0�����Ƚ���
*****************************************************************************/
void do_timer()
{
	DPRINTK(KERNEL_DEBUG,"kernel:do_timer\n\r");		
	if(!current)return;
	if(current->count){
		current->count--;
	}
	if((current->state != TASK_RUNNING)|| !current->count){
		schedule();
	}
}


int OSCreateProcess(unsigned long nand_start_addr,unsigned long len,char** argv, char** envp,long priority)
{
	long i = 1,j,argc,envc;
	unsigned long *p_VA;
	char *pDes;
	
	DPRINTK(KERNEL_DEBUG,"kernel:OSCreateProcess\n\r");
	
	for(; i < TASK_NR; i++){
		if(task[i].state == TASK_UNALLOCATE && VALIDE_TASK_INDEX(i)){

			OS_ENTER_CRITICAL();

			/*��NAND Flash�и��ƴ��뵽���̺�i��Ӧ�������ַ*/
			/*�����ַSDRAM_RAW_RW_VA_BASE + i*0x100000ָ��PID=i�Ľ��̵�1M�ڴ棬���Ҵ������ַ>32M��������PID�޹�*/
			DPRINTK(KERNEL_DEBUG,"kernel:OSCreateProcess::nand_read_ll\n\r");			
			nand_read_ll((unsigned char*)(SDRAM_RAW_RW_VA_BASE + i*0x100000),nand_start_addr,len);

			/****************************************************************************************
			* �����û�����int main(int argc, char **argc)�Ĳ���(envpδʵ��)��
			* ����i��1M��ַ�����1k�ռ����ڴ�Ų���(��ַ��������Ϊ)��
			* argc,argv[0],argv[1],...,argv[argc-1],NULL,"argv[0]��Ӧ��string","argv[1]��Ӧ��string",...
			*****************************************************************************************/
			DPRINTK(KERNEL_DEBUG,"kernel:OSCreateProcess::set the new process's parameters\n\r");			
			p_VA = (unsigned long *)(SDRAM_RAW_RW_VA_BASE + i*0x100000 - 1024);
			j = 0;
			if(argv)
				while(argv[j++]);		/*ͳ��argv*/
			*p_VA = argc  = j;		/*argc*/
			p_VA++;			/*ָ��argc[0]*/
			pDes = (char *)(p_VA + argc + 1);	/*pDesָ��"...,argc[argc-1],NULL"���棬�������argv[...]��Ӧ���ַ���*/
			for(j = 0; j < argc; j++){
				*p_VA = (unsigned long)pDes;
				p_VA++;
				pDes = (char *)(((unsigned long)pDes + strcpy(pDes,argv[j]) + 3)&(~0x03));/*pDes 4�ֽڶ���*/
			}
			
			/************************************************************************************
			* ���ò�����spsr,sp,pc 
			* �ο�sched.S�б�����������ĵ�˳��Ϊ:
			* sp(�û�/ϵͳģʽ�µ�sp),spsr,r0-r12,lr
			* ����sp����task[i].content[0]�У�spsr����task[i].contetn[1]�У�lr����task[i].content[15]��
			**************************************************************************************/
			DPRINTK(KERNEL_DEBUG,"kernel:OSCreateProcess::set the new process's content\n\r");
			task[i].pid = i;
			task[i].state = TASK_RUNNING;
			task[i].count = 15;
			task[i].priority = priority;
			task[i].content[0] = 0x100000 - 1024; /*�û�/ϵͳģʽsp*/
			task[i].content[1] = 0x5f;   	/*spsr*/
			task[i].content[15] = 0;	       	/*lr, ���̴�0��ַ��ʼִ��*/
			
			OS_EXIT_CRITICAL();
			return ;
		}
	}
	
}

