// start system 
#include <cstdint>
#include <iostream>

void src2dma_clock(uint32_t clock, uint8_t* data, uint32_t length){
	
	std::cout << "[ DMA_Module::Channel \033[31m clock @ " << clock << "\033[0m ] " <<
	"\t fetch data from source " <<
	"\033[32m data :" << " ";

	for (uint32_t i = 0; i < length; ++i){
		std::cout<< std::dec  << static_cast<int>(data[i]) << " ";
	}
	std::cout << "\033[0m  \tdata_width (bytes) : " << length  << std::endl;
}

void src2dma_burst_clock(uint32_t clock, uint8_t* data, uint32_t length, uint32_t offset){

	std::cout << "[ DMA_Module::Channel \033[31m clock @ " << clock << "\033[0m ]" << 
	"\t fetch data from source (burst mode) " <<
	"\033[32m data :" << " ";
	
	for (uint32_t i =offset; i<length + offset; ++i){
		std::cout << std::dec << static_cast<int>(data[i]) << " ";
	}
	std::cout << "\033[0m  \tdata_width (bytes) : " << length  << std::endl;

}

void dma2dst_clock(uint32_t clock, uint8_t* data, uint32_t length){
	
	std::cout << "[ DMA_Module::Channel \033[31m clock @ " << clock << "\033[0m ] "<<
	"\t send data to destination " <<
	"\033[32m  data :" << " ";
	
	for (uint32_t i = 0; i<length; ++i){
		std::cout<< std::dec << static_cast<int>(data[i]) << " ";
	}
	std::cout << "\033[0m  \tdata_width (bytes) : " << length  << std::endl;

	
}

void dma2dst_burst_clock(uint32_t clock, uint8_t* data, uint32_t length, uint32_t offset){

	std::cout << "[ DMA_Module::Channel \033[31m clock @ " << clock << "\033[0m ]" << 
	"\t send data to destination (burst mode)" << 
	" \033[32m data :" << " ";
	
	for (uint32_t i =offset; i<length + offset; ++i){
		std::cout << std::dec << static_cast<int>(data[i]) << " ";
	}
	std::cout << "\033[0m \tdata_width (bytes) : " << length  << std::endl;
}





