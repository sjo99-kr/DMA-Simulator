#include <stdint.h>
#include <vector>
#include <string>
#include "Module.hh"

class DMA_CHANNEL : public Module {
	public:	
		// constructor
		 DMA_CHANNEL(){}
		 
		// Public params 
		uint32_t transfer_count;  // transfer_count = total transfer size , e.g. 64Bytes , 128Bytes.. 1024Bytes.. 
		uint8_t transaction_size; // data_width based on bus data-width

		// Set Channal for transaction (Initailization)
		void set_channels(uint8_t* src_addr, uint8_t* dst_addr,uint32_t transaction_size, uint32_t transfer_count,
			bool burst, uint32_t burst_size, uint32_t buf_size);
		// src_addr : base address for device or Memory
		// dst_addr : base address for device or Memory
		// burst : burst mode or simple mode
		// burst_size : burst size for burst mode, Note: burst_size < buf_size	
			
		
		void reset();  // Reset DMA Channel parameters  
		bool is_busy(); // check DMA CHANNEL is busy or not
		void get_buffer(uint32_t size); // set buffer size based on buf_size parameters.
		bool check_flag(); // After write-data process, interrupt signal.
		void set_flag(); // set flag if write-data process is done or Not.
		 
		
		// ready check
		bool src_check(uint8_t* src_addr);
		bool dst_check(uint8_t* dst_addr);
		// data transfer for one word.. (8-bytes, 16-bytes, 32-bytes... )
		bool dataTransferRead();
		bool dataTransferWrite();
		
		// Simple Transfer for Cycle-Stealing Mode (Non-burst Mode)
		uint32_t SimpleTransfer();
		uint32_t BurstTransfer();
		
		
		uint8_t* src_addr; // Source base addr
		uint8_t* dst_addr; // Destination base addr

		bool burst_or_not; // Burst mode or Cycle-Stealing Mode  (true / false)


	private:

		// private params
		uint32_t channel_id; // set channel ID
		bool state; // Channel State (IDLE :0  BUSY ; 1)
		uint32_t buf_reg; // pointer for channel_buffer
		uint32_t burst_size; // burst_size , num of word for each transfer
		
		std::vector<uint8_t> buffer; // buffer in dma_channel
		uint32_t buf_size;  
		uint32_t rd_pt; // offset for read process
		uint32_t wr_pt; // offset for write process
		bool flag; // flag for done signal./
		
};

