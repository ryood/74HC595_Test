/*
 * _74HC595_SPI_Test.c
 *
 * Created: 2015/08/22 18:40:42
 *  Author: gizmo
 */ 

#define F_CPU	1000000UL

#include <avr/io.h>
#include <util/delay.h>

#define SHIFT_PORT PORTB
#define SHIFT_RCK PB1

void delay_ms(uint16_t ms){
	while(ms--){
		_delay_ms(1);
	}
}

void _shift_rck(){  //���b�`�N���b�N������M
	SHIFT_PORT &= ~(1<<SHIFT_RCK);
	SHIFT_PORT |= (1<<SHIFT_RCK);
}

void shift_out(uint8_t data){
	//int8_t i;
	SPDR = data;     //�f�[�^�Z�b�g�ő��M�J�n
	while(!(SPSR & (1<<SPIF)));    //�]�������܂őҋ@
	_shift_rck();   //���b�`���X�V
}

void init_spi(){
	//SPI�N���b�N�͍ő���fOSC/2
	SPCR = 0b01010000;
	SPSR = 0b00000001;
}

int main(){
	uint16_t i;
	uint8_t t;
	//DDRB = 0b00101111;  //���o�͐ݒ�(SS��SPI�̓��쎞�͏o�͂ɂ��Ă���)
	DDRB = 0b00101110;  //���o�͐ݒ�(SS��SPI�̓��쎞�͏o�͂ɂ��Ă���)
	init_spi();     //SPI�����ݒ�
	while(1){
		//0����255�܂ł��J�E���g�A�b�v���ďo��
		for(i=0;i<=0xff;i++){
			shift_out(i);
			delay_ms(30);
		}
		
		//�o�[�����΂�����k�߂���
		for(t=0;t<5;t++){
			for(i=0;i<9;i++){
				shift_out((0xff>>(8-(i%9))));
				delay_ms(70);
			}
			for(i=0;i<9;i++){
				shift_out((0xff>>(i%9)));
				delay_ms(70);
			}
		}
		
		//���E�ɃX�N���[��
		for(t=0;t<5;t++){
			for(i=0;i<8;i++){
				shift_out((1<<(i%8)));
				delay_ms(70);
			}
			for(i=0;i<8;i++){
				shift_out((0x80>>(i%8)));
				delay_ms(70);
			}
		}
		
	}
	return 0;
}