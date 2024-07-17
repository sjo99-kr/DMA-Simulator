#include "dma_v1.hh"
#include <cstdint>
#include <sstream>



bool DMA_v1::is_busy(){
	if(state != 0x00 || state != 0x03) return false;
	else return true;
}

void DMA_v1::reset(){
	state = 0x00; // 00 or 11 is IDLE state
	num_of_ch = 0;
	addr_width = 0;
	sg_or_not = false;
	burst_or_not = false;
	max_burst_size = 0;
	BTT  = 0;
	clock = 0;
};

DMA_v1::DMA_v1(const std::string& dma_script){
	std::ifstream inFile(dma_script);
	if(!inFile) {
		std::cout << "DMA SCRIPT IS NOT VALID" << std::endl;
	}
	std::string line;
	
	std::cout << "script inserted " <<std::endl;
	
	while(std::getline(inFile,line)) {
		std::istringstream iss(line);
		std::string variableName;
		std::string equalSign;
		
		if( iss >> variableName >> equalSign){
			if(variableName == "num_of_ch"){
				iss >> num_of_ch;
			}
			else if(variableName == "addr_width") {
				iss >> addr_width;
			}
			else if (variableName == "sg_or_not") {
				std::string value;
				iss >> value;
				sg_or_not = (value =="true");
			}
			else if (variableName == "max_burst_size"){
				iss >> max_burst_size;
			}
			else if (variableName == "BTT") {
				iss >> BTT;
			}
			else if (variableName == "burst_or_not") {
				std::string value;
				iss >> value;
				burst_or_not = (value=="true");
			}
		}
	}

	std::cout<<"------------------DMA Descriptors--------------------"<<std::endl;
   	std::cout << "num_of_ch: " << num_of_ch << std::endl;
        std::cout<< "addr_width: " << addr_width << std::endl;
        std::cout<< "sg_or_not (false: 0, true: 1): " << sg_or_not << std::endl;
        std::cout<< "max_burst_size: " << max_burst_size << std::endl;
        std::cout<< "BTT: " << static_cast<int>(BTT) << std::endl;
        std::cout<< "burst_or_not (false: 0, true: 1): " << burst_or_not <<std::endl;

        
        for(uint32_t i =0; i<num_of_ch; ++i){
        	channels[i].reset();
        }
 
};

bool DMA_v1::check_btt(uint32_t transfer_count){
	if(transfer_count < (pow(2, BTT)-1)) return true;
	else return false;
};

uint32_t DMA_v1::select_channel(){
	for(uint32_t i =0; i< num_of_ch; ++i){
		if(!channels[i].is_busy()) return i;
	}
};

bool DMA_v1::solveDescriptors(const std::string& filename, uint8_t* src_addr, uint8_t* dst_addr){
	bool status;
	if(is_busy()) return false;
	
	uint8_t ch_idx = select_channel();
	channels[ch_idx].reset();
	
	std::ifstream infile(filename); 
	if(!infile) {
		std::cout << "Descripts Error !! "<< std::endl;
		return false;
	}

	uint32_t transaction_size; uint32_t transfer_count;
	uint32_t burst_size;
	uint32_t count = 0;
	uint32_t value;
	channels[ch_idx].src_addr = src_addr;
	channels[ch_idx].dst_addr = dst_addr;
	
	std::string line;

	while(std::getline(infile,line)) {
		std::istringstream iss(line);
		std::string variableName;
		std::string equalSign;
		
		if( iss >> variableName >> equalSign){
			if(variableName == "transaction_size"){
				iss >> transaction_size;
			}
			else if(variableName == "transfer_count") {
				iss >> transfer_count;
			}
			else if (variableName == "burst_size") {
				iss >> burst_size;
			}
		}
	}

	std::cout<<"------------------descriptor image-------------------"<<std::endl;
	std::cout << "transaction_size: " << transaction_size << std::endl;
        std::cout << "transfer_count: " << transfer_count << std::endl;
        std::cout << "burst_size: " << burst_size  << std::endl;
              
        std::cout<<"----------------------------------------------------"<<std::endl;
	// channel setting
	channels[ch_idx].set_channels(src_addr, dst_addr, transaction_size, transfer_count, burst_or_not, burst_size, 32); // buf_size = 32 byte;
	status = check_btt(channels[ch_idx].transfer_count);
	
	latency();

	std::cout << "simple transfer startd in dma_v1 " << std::endl;
	
	uint32_t cycle = simpleMode(ch_idx, 1); // ready_signal set to be 1;
	add_clock(cycle);
	/// transfer ready /////
	
	return status;
	
};


uint32_t DMA_v1::simpleMode(uint32_t ch_idx, bool ready_signal){
	DMA_CHANNEL channel = channels[ch_idx];
	if(channel.is_busy()) return false;
	uint32_t clock = channel.SimpleTransfer(ready_signal);
	
	return clock;
}



