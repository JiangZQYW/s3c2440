@******************************************************************************
@ File��head.s
@ ���ܣ�����SDRAM���������Ƶ�SDRAM��Ȼ������SDRAM����ִ��
@******************************************************************************       
 
.extern		main
.text
.global _start
_start:
					@����disable_watch_dog, memsetup, init_nand, nand_read_ll��init.c�ж���
        ldr     sp, =0x34000000
	bl	disable_watch_dog		@��WATCH DOG
	ldr	lr,	=halt_loop	@���÷��ص�ַ
	ldr	pc,	=main		@bָ���blָ��ֻ��ǰ����ת32M�ķ�Χ����������ʹ����pc��ֵ�ķ���������ת
halt_loop:
	b	halt_loop
