#include "dma_ch.hh"


void DMA_CHANNEL::get_buffer(uint32_t size){
	buffer.resize(size);
};




void DMA_CHANNEL::reset(){
	state = 0; // IDLE = 0;
	flag = 0;
	rd_pt = 0;
	wr_pt = 0;
	buf_reg = 0;

};



void DMA_CHANNEL::set_channels(uint8_t* src_addr, uint8_t* dst_addr, uint32_t transaction_size,
			uint32_t transfer_count, bool burst_or_not, uint32_t burst_size, uint32_t buf_size){
			
			this-> src_addr = src_addr;
			this->dst_addr = dst_addr;
			this->transaction_size = transaction_size;
			this->burst_or_not = burst_or_not;
			this->transfer_count = transfer_count;
			this->burst_size = burst_size;
			this->buf_size = buf_size;
			
			get_buffer(buf_size);
};
			
bool DMA_CHANNEL::get_ready_check(bool ready_signal){

	return (ready_signal) ? true : false;
};


bool DMA_CHANNEL::is_busy(){

	return (state) ? true : false;
};

void DMA_CHANNEL::check_flag(){
	if(transfer_count !=0) flag = 0;
	else flag =1;
}

bool DMA_CHANNEL::done_flag(){
	return (flag) ? 1 : 0;
}

bool DMA_CHANNEL::dataTransferRead() {
	if(is_busy()) return false;
	
	state = 1; //busy setting
	if(!burst_or_not){

		if(transaction_size < transfer_count){
			for(uint32_t i = 0; i< transaction_size; ++i){
				buffer[buf_reg] = src_addr[i+rd_pt];

				buf_reg = buf_reg + 1;
				}
				rd_pt = rd_pt + transaction_size;
			tick();
		}else {
			for(uint32_t i = 0; i< transfer_count; ++i){
				buffer[buf_reg] = src_addr[i + rd_pt];

				buf_reg = buf_reg + 1;
			}
			rd_pt = 0;
			tick();
		}
		buf_reg = 0; //buf_reg initialization 
	}

	else {
		/// burst case
	
	}
	state = 0;
	return true;

};

bool DMA_CHANNEL::dataTransferWrite(bool ready_signal){
	if(is_busy()) return false;
	state = 1; //busy setting
	
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
	}
	state = 0;
	check_flag();
	return true;
};

uint32_t DMA_CHANNEL::SimpleTransfer(bool ready_signal){
	int rd = 0;
	int wr = 0;
	while(transfer_count != 0){
		dataTransferRead();
		rd = rd +1;
		trans_tick();
		dataTransferWrite(ready_signal);
		wr = wr +1;
	}
	std::cout << "read count :" << rd << std::endl;
	std::cout << "write count :" << wr << std::endl;
	uint32_t c = getClock();
	return c;
}

