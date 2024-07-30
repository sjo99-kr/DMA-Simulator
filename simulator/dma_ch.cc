#include "utils.hh"
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
bool DMA_CHANNEL::src_check(bool ready){
	if(ready) src_ready = 1;
	else src_ready = 0;
	return src_ready;
};

// ready check for dst_addr
bool DMA_CHANNEL::dst_check(bool ready){

	// we just calculation clock cycles for hand-shake
	if(ready) dst_ready = 1;
	else dst_ready = 0;
	return dst_ready;
};

bool DMA_CHANNEL::bus_check(bool access){

	if(access) bus_access = 1;
	else bus_access = 0;
	return access;
}


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
bool DMA_CHANNEL::dataTransferRead(uint32_t clock) {
	
	uint8_t data_buf[buf_size];	
	
	// read-data process in Cycle-stealing Mode
	if(!burst_or_not){
	
		
		// case for data-width < trnasfer_count
		if(transaction_size < transfer_count){
			for(uint32_t i = 0; i< transaction_size; ++i){
				buffer[buf_reg] = src_addr[i+rd_pt];
				data_buf[buf_reg] = src_addr[i+rd_pt];
				buf_reg = buf_reg + 1;
				}
				rd_pt = rd_pt + transaction_size;
			//tick(); // tick for transfer data for one-word
			src2dma_clock(clock, data_buf, transaction_size);
		}
		// case for data-width > transfer_count
		else {
			for(uint32_t i = 0; i< transfer_count; ++i){
				buffer[buf_reg] = src_addr[i + rd_pt];
				data_buf[buf_reg] = src_addr[i + rd_pt];
				buf_reg = buf_reg + 1;
			}
			rd_pt = 0;
			//tick(); // tick for transfer data for one-word
			src2dma_clock(clock, data_buf, transfer_count);
		}
		buf_reg = 0; //buf_reg initialization 
	}

	else {
		// burst mode;
		if(buf_size > burst_size){
		
			if(transaction_size * burst_size < transfer_count){
				for(uint32_t i =0; i< burst_size * transaction_size; ++i){
					buffer[buf_reg] = src_addr[i + rd_pt];
					data_buf[buf_reg] = src_addr[i + rd_pt];
					buf_reg = buf_reg + 1;
				}
				
				rd_pt = rd_pt + transaction_size + rd_pt;
				for(uint32_t c = 0; c <burst_size; ++c){
					src2dma_burst_clock(clock, data_buf, transaction_size, transaction_size * c);
				}
			}
		
		
			

			
			else {
			
				for(uint32_t i =0; i< transfer_count; ++i){
					buffer[buf_reg] = src_addr[i + rd_pt];
					data_buf[buf_reg] = src_addr[i + rd_pt];
					buf_reg = buf_reg + 1;
				}
				
				rd_pt = 0;
				

				for(uint32_t i= 0; i< (transfer_count / transaction_size) + 1; ++i){
					if(i == transfer_count / transaction_size){
						src2dma_burst_clock(clock, data_buf, transfer_count, transaction_size * i);
					}
					else {
						src2dma_burst_clock(clock, data_buf, transaction_size, transaction_size * i);
						}
				}
				
			}
			buf_reg = 0; // buf_reg intilaization.
		}
		
		else {
			std::cout << "error occurred (buf_size < burst_size) " << std::endl;
			return 0;
		}
		
	}
	
	return true;

};

bool DMA_CHANNEL::dataTransferWrite(uint32_t clock){
	
	uint8_t data_buf[buf_size];	
	
	if(!burst_or_not){
		
		if(transaction_size < transfer_count){
			for(uint32_t i = 0; i< transaction_size; ++i){
				dst_addr[i +  wr_pt] = buffer[buf_reg];
				data_buf[buf_reg] = buffer[buf_reg];
				transfer_count = transfer_count - 1;
				buf_reg = buf_reg + 1;
			}
			wr_pt = wr_pt + transaction_size;
			//tick();
			dma2dst_clock(clock, data_buf, transaction_size);
			
		}
		else {
			for(uint32_t p = 0; p< transfer_count; ++p){
				dst_addr[wr_pt + p] = buffer[buf_reg];
				data_buf[buf_reg] = buffer[buf_reg];	
				buf_reg = buf_reg + 1;
			}
			
			dma2dst_clock(clock, data_buf, transfer_count);
			
			transfer_count  = 0;
			wr_pt = 0;
		}
		buf_reg = 0; //buf_reg initialization 
		
	}
	else {
		// burst mode;
		if(buf_size > burst_size){
			if(transaction_size * burst_size < transfer_count){
				for(uint32_t i =0; i< burst_size * transaction_size; ++i){
					dst_addr[i + wr_pt] = buffer[buf_reg];
					
					data_buf[buf_reg] = buffer[buf_reg];
					
					transfer_count = transfer_count  - 1;
					buf_reg = buf_reg + 1;
				}
				wr_pt = wr_pt + transaction_size * burst_size;


				for(uint32_t c = 0; c <burst_size; ++c){
					dma2dst_burst_clock(clock, data_buf, transaction_size, transaction_size * c);
				}
			}
			
			
			else {
				for(uint32_t i =0; i< transfer_count; ++i){
					dst_addr[wr_pt + i] = buffer[buf_reg];
					data_buf[buf_reg] = buffer[buf_reg];
					
					buf_reg = buf_reg + 1;
				}
				
				for(uint32_t i= 0; i< (transfer_count / transaction_size) + 1; ++i){
					if(i == transfer_count / transaction_size){
						dma2dst_burst_clock(clock, data_buf, transfer_count, transaction_size * i);
					}
					else {
						dma2dst_burst_clock(clock, data_buf, transaction_size, transaction_size * i);
						}
				}
				transfer_count = 0;
				wr_pt = 0;
			}
			buf_reg = 0; // buf_reg intilaization.
		}
		
		else {
			std::cout << "error occurred (buf_size < burst_size) " << std::endl;
			return 0;
		}
	}


	return true;
};


