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

rotary_encoder rot1(22,21);
rotary_encoder rot2(3,2);
void t1_function(){
	rot1.update();
}
void t2_function(){
	rot2.update();
}

void check_pots();

double ci = .01 * .001 * .001;
double cd = 0;

double r1 = 0;
double r2 = 0;
double kp = 0;
double ki = 0;

int main(){
	std::thread t1 (t1_function);
	std::thread t2 (t2_function);

	std::cout << "starting..." << std::endl;
	
	rot1.set_position(pot1.resistance_to_code(10 * 1000));
	rot2.set_position(pot2.resistance_to_code(10 * 1000));
	int prev_position[] = {0,0};
	for(;;){
		check_pots();
		if(prev_position[0] != rot1.get_position() || prev_position[1] != rot2.get_position()){
			pot1.transmit(rot1.get_position());
			pot2.transmit(rot2.get_position());

			r1 = pot1.code_to_resistance(rot1.get_position()); 
			r2 = pot2.code_to_resistance(rot2.get_position()); 

	//		std::cout << r1 / 1000.0 << "k,\t";
	//		std::cout << r1 / 1000.0 << "k" << std::endl;

			prev_position[0] = rot1.get_position();
			prev_position[1] = rot2.get_position();

			kp = r2 / r1 + cd / ci;
			ki = 1.0 / r1 / ci;					

			std::cout << "kp = " << kp << "\t";
			std::cout << "ki = " << ki << std::endl;
      		}
		
	}

	return 0;
}

void check_pots(){
		if(rot1.get_position() < 0 ){
			rot1.set_position(0);
			std::cerr << "Pot R1 is too low" << std::endl;
		}
		if(rot1.get_position() > 0xFF - 1 ){
			rot1.set_position(0xFF - 1);
			std::cerr << "Pot R1 is too high" << std::endl;
		}
		if(rot2.get_position() < 0 ){
			rot2.set_position(0);
			std::cerr << "Pot R2 is too low" << std::endl;
		}
		if(rot2.get_position() > 0xFF - 1 ){
			rot2.set_position(0xFF - 1);
			std::cerr << "Pot R1 is too high" << std::endl;
		}
	}
	
