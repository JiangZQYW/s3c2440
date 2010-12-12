@******************************************************************************
@ File��head.s
@ ���ܣ�����SDRAM���������Ƶ�SDRAM��Ȼ������SDRAM����ִ��
@******************************************************************************       
   
.extern		main
.text 
.global _start 
_start:
@******************************************************************************       
@�ж�����,��Reset��HandleIRQ�⣬�����쳣��û��ʹ��(������ҷ����ˣ�����������)
@******************************************************************************       
	b	Reset

@ 0x04: Undefined instruction exception
HandleUndef:
	b	HandleUndef 
 
@ 0x08: Software interrupt exception  
HandleSWI:
	b	HandleSWI

@ 0x0c: Prefetch Abort (Instruction Fetch Memory Abort)
HandlePrefetchAbort:
	b	HandlePrefetchAbort

@ 0x10: Data Access Memory Abort
HandleDataAbort:
	b	HandleDataAbort

@ 0x14: Not used
HandleNotUsed:
	b	HandleNotUsed

@ 0x18: IRQ(Interrupt Request) exception
	b	HandleIRQ

@ 0x1c: FIQ(Fast Interrupt Request) exception
HandleFIQ:
	b	HandleFIQ

Reset:					@����disable_watch_dog, memsetup, init_nand, nand_read_ll��init.c�ж���
	ldr	sp,	=4096		@���ö�ջ 
	bl	disable_watch_dog		@��WATCH DOG
	bl	memsetup		@��ʼ��SDRAM
	bl	init_nand		@��ʼ��NAND Flash
					@��NAND Flash�е�ַ4096��ʼ��1024�ֽڴ���(main.c����õ�)���Ƶ�SDRAM��

					@nand_read_ll������Ҫ3��������
	ldr	r0, 	=0x30000000	@1. Ŀ���ַ=0x30000000������SDRAM����ʼ��ַ
	mov    	r1, 	#4096		@2. Դ��ַ   = 4096�����ӵ�ʱ��main.c�еĴ��붼����NAND Flash��ַ4096��ʼ��
	mov	r2, 	#1024		@3. ���Ƴ���= 1024(bytes)�����ڱ�ʵ���main.c�������㹻��
	bl	nand_read_ll		@����C����nand_read_ll

	
	msr	cpsr_c,	#0xd2		@�����ж�ģʽ
	ldr	sp,	=0x33000000	@�����ж�ģʽ��ջ

	msr	cpsr_c,	#0xdf		@����ϵͳģʽ
	ldr	sp,	=0x34000000	@����ϵͳģʽ��ջ

	bl	init_irq			@�����жϳ�ʼ����������init.c��
	msr	cpsr_c,	#0x5f		@����I-bit=0����IRQ�ж�
	
	ldr	lr,	=halt_loop	@���÷��ص�ַ
	ldr	pc,	=main		@bָ���blָ��ֻ��ǰ����ת32M�ķ�Χ����������ʹ����pc��ֵ�ķ���������ת
halt_loop:
	b	halt_loop

HandleIRQ:
	sub	lr, lr, #4			@���㷵�ص�ַ
	stmdb	sp!, 	{ r0-r12,lr }	@����ʹ�õ��ļĴ���
	
	ldr	lr,	=int_return	@���÷��ص�ַ	
	ldr	pc, 	=EINT_Handle	@�����жϴ���������interrupt.c��
int_return:
	ldmia	sp!, 	{ r0-r12,pc }^	@�жϷ���, ^��ʾ��spsr��ֵ���Ƶ�cpsr
	




