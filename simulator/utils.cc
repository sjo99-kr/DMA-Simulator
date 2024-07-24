#include "dma_ch.hh"

// start system 
#include <cstdint>
#include <iostream>

void tick_clock(DMA_CHANNEL& a){
	std::cout << "[ nothing @ " <<a.clock << "]" << 
	" Nothing Process " 	<<
	"		channel id : " << a.channel_id 
	<<std::endl;
	
	a.clock = a.clock + 1;
}

void start_clock(DMA_CHANNEL& a){
	std::cout << "[ start_process @ " <<a.clock<< "]" << 
	" setting for params from scripts"  <<
	"		channel id : " << a.channel_id 
	<< std::endl;
	a.clock = a.clock + 1;
}

void reset_clock(DMA_CHANNEL& a){
	std::cout << "[ reset_process @ " << a.clock << "]" <<
	" reset for DMA channel" << 
	"		channel id : " << a.channel_id 
	<<std::endl;

	a.clock = a.clock + 1;
}	

void src2dma_clock(DMA_CHANNEL& a, uint8_t* data, uint32_t length){
	
	std::cout << "[ source to buffer @ " << a.clock << "] " <<
	" fetch data from source " <<
	"		channel id : " << a.channel_id << 
	" data :" << " ";

	for (uint32_t i = 0; i<length; ++i){
		std::cout<< std::dec << static_cast<int>(data[i]) << " ";
	}
	std::cout << "\tdata_width (bytes) : " << length  << std::endl;
	
	a.clock = a.clock + 1;
}

void src2dma_burst_clock(DMA_CHANNEL& a, uint8_t* data, uint32_t length, uint32_t offset){

	std::cout << "[ source to buffer @ " << a.clock << "]" << 
	" fetch data from source (burst mode) " <<
	"		channel id : " << a.channel_id << 
	" data :" << " ";
	
	for (uint32_t i =offset; i<length + offset; ++i){
		std::cout << std::dec << static_cast<int>(data[i]) << " ";
	}
	std::cout << "\tdata_width (bytes) : " << length  << std::endl;
	a.clock = a.clock + 1;
}

void dma2dst_clock(DMA_CHANNEL& a, uint8_t* data, uint32_t length){
	
	std::cout << "[ buffer to destination @ " << a.clock << "] "<<
	" send data to destination " <<
	"		channel id : " << a.channel_id <<
	" data :" << " ";
	
	for (uint32_t i = 0; i<length; ++i){
		std::cout<< std::dec << static_cast<int>(data[i]) << " ";
	}
	std::cout << "\tdata_width (bytes) : " << length  << std::endl;
	a.clock = a.clock + 1;
	
}

void dma2dst_burst_clock(DMA_CHANNEL& a, uint8_t* data, uint32_t length, uint32_t offset){

	std::cout << "[ buffer to destination @ " << a.clock << "]" << 
	" send data to destination (burst mode)" << 
	"		channel id : " << a.channel_id <<
	" data :" << " ";
	
	for (uint32_t i =offset; i<length + offset; ++i){
		std::cout << std::dec << static_cast<int>(data[i]) << " ";
	}
	std::cout << "\tdata_width (bytes) : " << length  << std::endl;
	a.clock = a.clock + 1;
	
}



void get_Access_clock(DMA_CHANNEL& a){
	std::cout << "[ wait for access to bus @ " << a.clock << "] " <<
	"Wait for access to System-bus" << std:: endl;
	a.clock = a.clock + 1; 	
	
	tick_clock(a);
}

void return_Access_clock(DMA_CHANNEL& a){	
	std::cout << "[ get for access to bus @ "<< a.clock << "] " <<
	"Get for access to System-bus" << std::endl;
	a.clock= a.clock + 1;

}

void wait_ready_clock(DMA_CHANNEL& a){
	std::cout << "[ wait for ready signal from Device @ "<< a.clock << "] " <<
	"Wait for Ready Signal from Device" <<std::endl;
	a.clock = a.clock + 1;
	
	tick_clock(a);
}

void get_ready_clock(DMA_CHANNEL& a) {
	std::cout << "[ get for access to bus @ "<< a.clock << "] " <<
	"Get a Ready Signal from Device" << std::endl;
	
	a.clock = a.clock +1;
}





