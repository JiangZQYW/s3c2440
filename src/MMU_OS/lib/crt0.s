@******************************************************************************
@ File��crt0.s
@ ���ܣ�ͨ����ת��C����
@******************************************************************************       

.text
.global _start
_start:				@ int main(int argc, char **argv)
	ldr	r0, [sp]		@ ����argc
	add	r1, sp, #4		@ ����argv
@	mov	r2, r0, lsl #2
@	add	r2, r2, r1		
@	add	r2, r2, #4		@ ����envp = &argv + (argc + 1) * 4,argv��envp����֮����һ��NULL
	bl	main		@ ����main����
halt_loop:
	b	halt_loop