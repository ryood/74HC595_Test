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

void _shift_rck(){  //ラッチクロックを一つ送信
	SHIFT_PORT &= ~(1<<SHIFT_RCK);
	SHIFT_PORT |= (1<<SHIFT_RCK);
}

void shift_out(uint8_t data){
	//int8_t i;
	SPDR = data;     //データセットで送信開始
	while(!(SPSR & (1<<SPIF)));    //転送完了まで待機
	_shift_rck();   //ラッチを更新
}

void init_spi(){
	//SPIクロックは最速のfOSC/2
	SPCR = 0b01010000;
	SPSR = 0b00000001;
}

int main(){
	uint16_t i;
	uint8_t t;
	//DDRB = 0b00101111;  //入出力設定(SSはSPIの動作時は出力にしておく)
	DDRB = 0b00101110;  //入出力設定(SSはSPIの動作時は出力にしておく)
	init_spi();     //SPI初期設定
	while(1){
		//0から255までをカウントアップして出力
		for(i=0;i<=0xff;i++){
			shift_out(i);
			delay_ms(30);
		}
		
		//バーを延ばしたり縮めたり
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
		
		//左右にスクロール
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