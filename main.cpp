#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include "../spi/spi.h"
#include "../rotary_encoder/rotary_encoder.h"

int digital_analog_converter::LDAC = 6; //pi pin used for LDAC control (set negative to turn off LDAC)

digital_potentiometer pot1(
		.5 * 1000 * 1000,		//clock speed
		8,				//bits
		0,				//min_resistance (pot output at transmitting 0}
		100 * 1000,			//max_resistance (pot output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		0				//Chip select
		);

digital_potentiometer pot2(
		.5 * 1000 * 1000,		//clock speed
		8,				//bits
		0,				//min_resistance (pot output at transmitting 0}
		100 * 1000,			//max_resistance (pot output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		1				//Chip select
		);

rotary_encoder r1(22,21);
rotary_encoder r2(3,2);
void t1_function(){
	r1.update();
}
void t2_function(){
	r2.update();
}

void check_pots();

int main(){
	std::thread t1 (t1_function);
	std::thread t2 (t2_function);

	std::cout << "starting..." << std::endl;
	
	r1.set_position(pot1.resistance_to_code(10 * 1000));
	r2.set_position(pot2.resistance_to_code(10 * 1000));
	int prev_position[] = {0,0};
	for(;;){
		check_pots();
		if(prev_position[0] != r1.get_position() || prev_position[1] != r2.get_position()){
			pot1.transmit(r1.get_position());
			pot2.transmit(r2.get_position());
			std::cout << pot1.code_to_resistance(r1.get_position()) / 1000.0 << "k,\t";
			std::cout << pot2.code_to_resistance(r2.get_position()) / 1000.0 << "k" << std::endl;
			prev_position[0] = r1.get_position();
			prev_position[1] = r2.get_position();
      		}
	}

	return 0;
}

void check_pots(){
		if(r1.get_position() < 0 ){
			r1.set_position(0);
			std::cerr << "Pot R1 is too low" << std::endl;
		}
		if(r1.get_position() > 0xFF - 1 ){
			r1.set_position(0xFF - 1);
			std::cerr << "Pot R1 is too high" << std::endl;
		}
		if(r2.get_position() < 0 ){
			r2.set_position(0);
			std::cerr << "Pot R2 is too low" << std::endl;
		}
		if(r2.get_position() > 0xFF - 1 ){
			r2.set_position(0xFF - 1);
			std::cerr << "Pot R1 is too high" << std::endl;
		}
	}
	
