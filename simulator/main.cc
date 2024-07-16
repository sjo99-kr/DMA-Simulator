#include "dma_system.cc"
#include <stdlib.h>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>




int main(int argc, char* argv[]){
	std::cout<< "start " << std::endl;
	if(argc < 3){
		std::cerr << "Usage : " << argv[0] << " <dma_descript> <descriptors>" << std::endl;
		return 1;
	}
	
	///////////////////////// Memory setting ////////////////////////////////
	uint8_t RAM[1000];
	uint8_t peripheral[200];
	for(uint32_t i =0; i<1000; i++){
		RAM[i] = i /4;
	}
	for(uint32_t i = 0; i<200; i++){
		peripheral[i] = i;	
	}
	/////////////////////////////////////////////////////////////////////////
	
	std::string dma_descript = argv[1];
	std::string descriptors = argv[2];
	
	std::cout << "FILE SETTING ON" <<std::endl;
	
	// DMA System setting
	DMA_SYSTEM dma_system(dma_descript, descriptors, RAM, peripheral);
	
	return 0;

}
