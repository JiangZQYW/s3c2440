
#define	GPBCON		(*(volatile unsigned long *)0x56000010)
#define	GPBDAT		(*(volatile unsigned long *)0x56000014)

#define	GPGCON		(*(volatile unsigned long *)0x56000060)
#define	GPGDAT		(*(volatile unsigned long *)0x56000064)

/*
LED1-4��ӦGPB7-10
*/

#define	GPB7_out	(1<<(7*2))
#define	GPB8_out	(1<<(8*2))
#define	GPB9_out	(1<<(9*2))
#define	GPB10_out	(1<<(10*2))

/*
#K1-K3��ӦGPF1-3
#K4��ӦGPF7
k3-k5��ӦGPG5-7
K2 ��Ӧ GPG3
*/
#define	GPG3_in		~(3<<(3*2))
#define	GPG5_in		~(3<<(5*2))
#define	GPG6_in		~(3<<(6*2))
#define	GPG7_in		~(3<<(7*2))


int main()
{

	GPBCON	 =GPB7_out | GPB8_out | GPB9_out | GPB10_out ;	//LED1-LED4��Ӧ��4��������Ϊ���
	GPGCON &= GPG3_in & GPG5_in & GPG6_in & GPG7_in ;		//K1-K4��Ӧ��4��������Ϊ����

	while(1){
		GPBDAT	= ((GPGDAT & 0xe0)<<3) | ((GPGDAT & 0x08)<<4);	//��KnΪ0(��ʾ����)������LEDnΪ0(��ʾ����)
	}

	return 0;
}
