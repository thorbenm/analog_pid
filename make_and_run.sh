#/bin/bash
echo "making..."
g++ -std=c++14 -lwiringPi -pthread *.cpp ../spi/spi.cpp ../rotary_encoder/rotary_encoder.cpp && ./a.out

