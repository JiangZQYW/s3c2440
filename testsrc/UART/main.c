#include "s3c2410.h"
#include "serial.h"
#include "led_on.h"

 
static unsigned long m_RandSeed;

/* ������� */
inline unsigned long  Rand()  
{
    return (m_RandSeed=1664525L*m_RandSeed+1013904223L)>>5;
}

void wait(unsigned long dly)
{
	for(; dly > 0; dly--);
}

int main()
{
	unsigned long i = 5, cnt = 0;
	unsigned char c;
	
	init_uart( );	//������11520��8N1(8������λ����У��λ��1��ֹͣλ)
        led_on(6);

        putcc('h');
        putcc('e');
        putcc('l');
        putcc('l');
        putcc('\0');


	while(1){//������Ӵ��ڽ������ݺ��ж����Ƿ����ֻ���ĸ���������1�����
		led_on( i++ );
                if (i == 10)
                    i=5;
		c = getc( ) + 1;
		if ( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ){
			putcc( c );
			cnt++;
		}
		if( cnt == 40){
			cnt = 0;
			putcc(0x0d);	//�س�
			putcc(0x0a);	//����
		}
	}

	return 0;
}
