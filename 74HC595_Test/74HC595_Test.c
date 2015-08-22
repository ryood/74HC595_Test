/*
 * _74HC595_Test.c
 *
 * Created: 2015/08/22 18:05:09
 *  Author: gizmo
 */ 

#define F_CPU	1000000UL

#include <avr/io.h>
#include <util/delay.h>

#define SHIFT_PORT PORTB
#define SHIFT_DATA PB1
#define SHIFT_SCK PB2
#define SHIFT_RCK PB3

void delay_ms(uint16_t ms){
	while(ms--){
		_delay_ms(1);
	}
}

void _shift_sck(){  //�V�t�g���W�X�^�N���b�N������M
	SHIFT_PORT |= (1<<SHIFT_SCK);
	SHIFT_PORT &= ~(1<<SHIFT_SCK);
}
void _shift_rck(){  //���b�`�N���b�N������M
	SHIFT_PORT &= ~(1<<SHIFT_RCK);
	SHIFT_PORT |= (1<<SHIFT_RCK);
}
void _shift_data(uint8_t bit){  //�V���A���f�[�^��SER�ɏo��
	if(bit){
		SHIFT_PORT |= (1<<SHIFT_DATA);
		}else{
		SHIFT_PORT &= ~(1<<SHIFT_DATA);
	}
}

void shift_out(uint8_t data){
	int8_t i;
	for(i=7;i>=0;i--){   //��ʃr�b�g����W���M
		_shift_data((data>>i)&1);
		_shift_sck();
	}
	_shift_rck();   //���b�`���X�V
}

int main(){
	uint16_t i;
	uint8_t t;
	//DDRB = 0b00000111;  //���o�͐ݒ�
	DDRB = 0b00001110;
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