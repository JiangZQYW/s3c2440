@******************************************************************************
@ File��crt0.s
@ ���ܣ�ͨ����ת��C����
@******************************************************************************       

.text
.global _start
_start:	
@	mov	sp, #4096		@ int main(int argc, char **argv)
	ldr	r0, [sp]		@ ����argc
	add	r1, sp, #4		@ ����argv
	bl	main		@ ����main����
halt_loop:
	b	halt_loop
