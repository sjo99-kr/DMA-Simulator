#include "dma_system.cc"
#include <stdlib.h>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>




int main(int argc, char* argv[]){
	std::cout<< "start " << std::endl;
	if(argc < 7){
		std::cerr << "Usage : " << argv[0] << " <dma_descript> <descriptors> <mem0_trace> <mem1_trace> <meme2_trace> <mem3_trace>" << std::endl;
		return 1;
	}
	
	///////////////////////// Memory setting ////////////////////////////////
	std::string mem0_trace = argv[3];
	std::string mem1_trace = argv[4];
	std::string mem2_trace = argv[5];
	std::string mem3_trace = argv[6];
	/////////////////////////////////////////////////////////////////////////
	
	std::string dma_descript = argv[1];
	std::string descriptors = argv[2];
	
	std::cout << "FILE SETTING ON" <<std::endl;
	
	// DMA System setting
	DMA_SYSTEM dma_system(dma_descript, descriptors, mem0_trace, mem1_trace, mem2_trace, mem3_trace );
	
	return 0;

}
