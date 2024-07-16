#include <stdint.h>
#include <vector>
#include <string>
#include "Module.hh"

class DMA_CHANNEL : public Module {
	public:	
		// constructor
//	DMA_CHANNEL() {}
		 DMA_CHANNEL(){}
				
		uint32_t transfer_count; // trnasfer number count
		uint8_t transaction_size; // data width

		// Set Channal for transaction
		void set_channels(uint8_t* src_addr, uint8_t* dst_addr,uint32_t transaction_size, uint32_t transfer_count,
			bool burst, uint32_t burst_size, uint32_t buf_size);
			
		void reset();
		// check DMA CHANNEL is busy or not
		bool is_busy();
		// set buffer_size
		void get_buffer(uint32_t size);
		bool done_flag();
		void check_flag();
		 
		
		// ready check
		bool get_ready_check(bool ReadySignal);
		
		// Transfer data from DMA buffer to other place 
		bool dataTransferRead();
		bool dataTransferWrite(bool ready_signal);
		uint32_t SimpleTransfer(bool ready_signal);
		
		uint8_t* src_addr; // Source base addr
		uint8_t* dst_addr; // Destination base addr
		


	private:


		uint32_t channel_id; // set channel ID
		bool state; // Channel State
		uint32_t buf_reg;
		bool burst_or_not; // Burst mode or Normal Mode  (true / false)
		uint32_t burst_size; // burst Size check
		std::vector<uint8_t> buffer;
		uint32_t buf_size;
		uint32_t rd_pt;
		uint32_t wr_pt;
		bool flag;
		
};


