#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include "../spi/spi.h"
#include "../rotary_encoder/rotary_encoder.h"

int digital_analog_converter::LDAC = 6; //pi pin used for LDAC control (set negative to turn off LDAC)

digital_potentiometer digipot(
		.5 * 1000 * 1000,		//clock speed
		8,				//bits
		0,				//min_resistance (pot output at transmitting 0}
		10 * 1000,			//max_resistance (pot output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		0				//Chip select
		);

rotary_encoder r1(0,7);
void t1_function(){
	r1.update();
}

int main(){
	std::thread t1 (t1_function);

	int pause_length = .05 * 1000 * 1000;
	
	std::cout << "starting..." << std::endl;
	
	for(;;){
		digipot.transmit(r1.get_position());
		delay(100);
//		for(uint8_t j = 0x00; j < 0xFE; j++){
//			digipot.transmit(j);
//			usleep(pause_length);
//		}	
//		std::cout << "high!" << std::endl;
//		for(uint8_t j = 0xFF; j > 0x01; j--){
//			digipot.transmit(j);
//			usleep(pause_length);
//		}	
//		std::cout << "low!" << std::endl;
	}

	return 0;
}
