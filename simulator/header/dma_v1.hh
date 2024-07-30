#include <stdint.h>
#include "dma_ch.hh"
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>


class DMA_v1 // state , mode , transaction_size
{
	private:

		uint32_t num_of_ch;
		uint32_t addr_width;
		bool sg_or_not;
		bool burst_or_not;
		uint32_t max_burst_size;
		uint32_t BTT;
		DMA_CHANNEL channels[8];	
		
		uint8_t* mem0;
		uint8_t* mem1;
		uint8_t* mem2;
		uint8_t* mem3;
		
		
	public:
		DMA_v1(const std::string& dma_script, 
			   const std::string& descriptors, 
			   const std::string& mem0_trace, 
			   const std::string& mem1_trace, 
			   const std::string& mem2_trace, 
			   const std::string& mem3_trace);

		// function setting
		void reset();
		void initChannels();
		bool state;
				
		uint32_t reg_channelId; // register for channel_id which is used.
		
		uint8_t* set_memory(const std::string& mem_trace);
		uint32_t getClock(uint32_t channel_id);

		//
		bool check_btt(uint32_t transfer_count);
		uint32_t select_channel();
		
		// Read Descripotr informations and allocate channels.
		bool solveDescriptors(const std::string& filename);
			// channel addr -> device id setting
			
		// ACtion processs
		bool Action(uint32_t clk, bool dst_ready, bool src_ready, bool bus_access);
		
		//  DMA Enable or Disable
		bool is_busy();
		
		// set transfer count
		uint32_t setTransferCount();
		

};
