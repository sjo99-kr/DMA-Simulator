#include "dma_ch.hh"


// Initialization for Channel-Buffer
void DMA_CHANNEL::get_buffer(uint32_t size){
	buffer.resize(size);
};


// Reset for Channel Params
void DMA_CHANNEL::reset(){
	state = 0; // IDLE = 0;
	flag = 0;
	rd_pt = 0;
	wr_pt = 0;
	buf_reg = 0;
};


// Initialization channel params
void DMA_CHANNEL::set_channels(uint8_t* src_addr, uint8_t* dst_addr, uint32_t transaction_size,
			uint32_t transfer_count, bool burst_or_not, uint32_t burst_size, uint32_t buf_size){
			
			this->src_addr = src_addr;
			this->dst_addr = dst_addr;
			this->transaction_size = transaction_size;
			this->burst_or_not = burst_or_not;
			this->transfer_count = transfer_count;
			this->burst_size = burst_size;
			this->buf_size = buf_size;	
			get_buffer(buf_size);
};
			
// ready check for src_addr
bool DMA_CHANNEL::src_check(uint8_t* src_addr){
	tick();
	// we just calculation clock cycles for hand-shake
	return (src_addr != NULL) ? true : false;
};

// ready check for dst_addr
bool DMA_CHANNEL::dst_check(uint8_t* dst_addr){
	tick();
	// we just calculation clock cycles for hand-shake
	return (dst_addr != NULL ) ? true : false;
};


// Idle or busy state
bool DMA_CHANNEL::is_busy(){
	return (state) ? true : false;
};

// check flag for Done or Not
void DMA_CHANNEL::set_flag(){
	if(transfer_count !=0) flag = 0;
	else flag =1;
}

// check if flag is 1 or 0.
bool DMA_CHANNEL::check_flag(){
	return (flag) ? 1 : 0;
}

// read-data process for one-word in one-cylce
bool DMA_CHANNEL::dataTransferRead() {
	// channel is busy or not
	if(is_busy()) return false;
	
	state = 1; // setting state to busy before transfer-process
	
	src_check(src_addr); //check for ready-signal from source
	
	// read-data process in Cycle-stealing Mode
	if(!burst_or_not){

		// case for data-width < trnasfer_count
		if(transaction_size < transfer_count){
			for(uint32_t i = 0; i< transaction_size; ++i){
				buffer[buf_reg] = src_addr[i+rd_pt];

				buf_reg = buf_reg + 1;
				}
				rd_pt = rd_pt + transaction_size;
			tick(); // tick for transfer data for one-word
		}
		// case for data-width > transfer_count
		else {
			for(uint32_t i = 0; i< transfer_count; ++i){
				buffer[buf_reg] = src_addr[i + rd_pt];
				buf_reg = buf_reg + 1;
			}
			rd_pt = 0;
			tick(); // tick for transfer data for one-word
		}
		buf_reg = 0; //buf_reg initialization 
	}

	else {
		// burst mode;
		if(buf_size > burst_size){
			if(transaction_size * burst_size < transfer_count){
				for(uint32_t i =0; i< burst_size * transaction_size; ++i){
					buffer[buf_reg] = src_addr[i+rd_pt];
					buf_reg = buf_reg + 1;
				}
				rd_pt = rd_pt + transaction_size + rd_pt;
				for(uint32_t c = 0; c <burst_size; ++c){
					tick(); //  calculate ticks for burst-mode
				}
			}
			else {
				for(uint32_t i =0; i< transfer_count; ++i){
					buffer[buf_reg] = src_addr[i + rd_pt];
					buf_reg = buf_reg + 1;
				}
				rd_pt = 0;
				for(uint32_t d = 0; d< transfer_count / transaction_size +1; ++d){
					tick(); // calculate ticks for burst-mode
				}
			}
			buf_reg = 0; // buf_reg intilaization.
		}
		
		else {
			std::cout << "error occurred (buf_size < burst_size) " << std::endl;
			return 0;
		}
		
	}
	
	state = 0; // Change channel state to IDLE.
	return true;

};

bool DMA_CHANNEL::dataTransferWrite(){
	if(is_busy()) return false;
	state = 1; //busy setting
	
	dst_check(dst_addr); //check for ready-signal from destination
	
	if(!burst_or_not){
		
		if(transaction_size < transfer_count){
			for(uint32_t i = 0; i< transaction_size; ++i){
				dst_addr[i +  wr_pt] = buffer[buf_reg];

				transfer_count = transfer_count - 1;
				buf_reg = buf_reg + 1;
			}
			wr_pt = wr_pt + transaction_size;
			tick();
		}
		else {
			for(uint32_t p = 0; p< transfer_count; ++p){
				dst_addr[wr_pt + p] = buffer[buf_reg];

				buf_reg = buf_reg + 1;
			}
			transfer_count  = 0;
			wr_pt = 0;
			tick();
		}
		buf_reg = 0; //buf_reg initialization 
		
	}
	else {
		// burst mode;
		if(buf_size > burst_size){
			if(transaction_size * burst_size < transfer_count){
				for(uint32_t i =0; i< burst_size * transaction_size; ++i){
					dst_addr[i + wr_pt] = buffer[buf_reg];
					transfer_count = transfer_count  - 1;
					buf_reg = buf_reg + 1;
				}
				wr_pt = wr_pt + transaction_size * burst_size;

				for(uint32_t c = 0; c < burst_size; ++c){
					tick(); //  calculate ticks for burst-mode
				}
			}
			else {
				for(uint32_t i =0; i< transfer_count; ++i){
					dst_addr[wr_pt + i] = buffer[buf_reg];
					buf_reg = buf_reg + 1;
				}
				transfer_count = 0;
				wr_pt = 0;
				for(uint32_t d = 0; d< transfer_count / transaction_size +1; ++d){
					tick(); // calculate ticks for burst-mode
				}
			}
			buf_reg = 0; // buf_reg intilaization.
		}
		
		else {
			std::cout << "error occurred (buf_size < burst_size) " << std::endl;
			return 0;
		}
	}
	state = 0;
	set_flag();
	return true;
};

uint32_t DMA_CHANNEL::SimpleTransfer(){
	int rd = 0;
	int wr = 0;
	while(transfer_count != 0){
	
		access_bus_tick(); // get access for bus in read-data process
		dataTransferRead();
		rd = rd +1; // read-process counts;
		
		access_bus_tick(); // get access for bus in write-data process
		dataTransferWrite();
		wr = wr +1; // write-process counts;
	}
	std::cout << "Non-burst Mode, read count :" << rd << std::endl;
	std::cout << "Non-burst Mode, write count :" << wr << std::endl;
	uint32_t c = getClock();
	return c;
}

uint32_t DMA_CHANNEL::BurstTransfer(){
	int rd = 0;
	int wr = 0;
	while(transfer_count != 0){
		access_bus_tick(); // get access for bus in read-data process
		dataTransferRead();
		rd = rd +1;
		
		access_bus_tick(); // get access for bus in write-data process
		dataTransferWrite();
		wr = wr +1;
	
	}	
	std::cout << "Burst Mode, read count :" << rd << std::endl;
	std::cout << "Burst Mode, write count :" << wr << std::endl;
	uint32_t c = getClock();
	return c;
}
