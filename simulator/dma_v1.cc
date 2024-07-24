
#include "dma_v1.hh"

#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>


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

};

uint8_t* DMA_v1::set_memory(const std::string& mem_trace){
	std::ifstream file(mem_trace);
	if(!file.is_open()){
		std::cerr << "Error occurred from mem_trace" << std::endl;
	}
	std::string line;
	std::vector<uint8_t> data;
	while(std::getline(file, line)){
		std::istringstream iss(line);
		std::string value;
		while(iss>>value){
			try{
				uint8_t byte = static_cast<uint8_t>(std::stoul(value, nullptr, 16));
				data.push_back(byte);
			
			} catch(const std::invalid_argument& e){
				std::cerr<<"invalid value " << value << std::endl;
			}
		}
	}
	file.close();
	size_t dataSize = data.size();
	uint8_t* b = new uint8_t[dataSize];
	std::copy(data.begin(), data.end(), b);
	
	for (size_t i = 0; i < dataSize; ++i) {
        std::cout << std::dec << static_cast<int>(b[i]) << " ";
        	if((i!= 0) && (i % 5 == 0)){
        		std::cout<<std::endl;
        	}
    	}
    	std::cout << std::endl;
	return b;
}


DMA_v1::DMA_v1(const std::string& dma_script, const std::string& mem0_trace, const std::string& mem1_trace, 
const std::string& mem2_trace, const std::string& mem3_trace){
	
	std::cout<< "memory0 data " << std::endl;
	mem0 = set_memory(mem0_trace);
	std::cout<< "memory1 data " << std::endl;	
	mem1 = set_memory(mem1_trace);
	std::cout<< "memory2 data " << std::endl;
	mem2 = set_memory(mem2_trace);
	std::cout<< "memory3 data " << std::endl;
	mem3 = set_memory(mem3_trace);
	
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

bool DMA_v1::solveDescriptors(const std::string& filename){
	bool status;
	if(is_busy()) return false;

	uint8_t ch_idx = select_channel();
	channels[ch_idx].reset();
	
	std::ifstream infile(filename); 
	if(!infile) {
		std::cout << "Descripts Error !! "<< std::endl;
		return false;
	}

	uint32_t transaction_size; 
	uint32_t transfer_count;
	uint32_t burst_size;
	uint32_t count = 0;
	uint32_t value;
	uint8_t* src_addr;
	uint8_t* dst_addr;
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
			else if(variableName == "src_device") {
				std::string value;
				iss >> value;
				if(value == "0"){
					src_addr =  mem0;
				}
				else if(value == "1"){
					src_addr = mem1;
				}
				else if(value == "2"){
					src_addr = mem2;
				}
				else if(value == "3"){
					src_addr = mem3;
				}
			}
			else if(variableName == "dst_device"){
				std::string value;
				iss >> value;
				if(value == "0"){
					dst_addr =  mem0;
				}
				else if(value == "1"){
					dst_addr = mem1;
				}
				else if(value == "2"){
					dst_addr = mem2;
				}
				else if(value == "3"){
					dst_addr = mem3;
				}
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

	start_clock(channels[ch_idx]); // setting for start clock
	reset_clock(channels[ch_idx]); // setting for reset clock
		
	/// Cycle Stealing Mode /// 
	if(!channels[ch_idx].burst_or_not){
		std::cout << "simple transfer startd in dma_v1 " << std::endl;
	
		simpleMode(ch_idx); // src_ready and dst_ready signal set for 1 ,1
	}
	/// Burst Mode ///
	else {
		std::cout << "Burst transfer started in dma_v1" << std::endl;
		burstMode(ch_idx);
	}
	
	
	return status;
	
};


void DMA_v1::simpleMode(uint32_t ch_idx){
	DMA_CHANNEL channel = channels[ch_idx];
	if(channel.is_busy()) return ;
	channel.SimpleTransfer();
	
}

void DMA_v1::burstMode(uint32_t ch_idx){
	DMA_CHANNEL channel = channels[ch_idx];
	if(channel.is_busy()) return ;
	channel.BurstTransfer();

}




