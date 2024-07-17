#include <stdint.h>
#include "dma_ch.cc"
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>


class DMA_v1 : public Module // state , mode , transaction_size
{
	private:
		uint8_t state;
		uint32_t num_of_ch;
		uint32_t addr_width;
		bool sg_or_not;
		bool burst_or_not;
		uint32_t max_burst_size;
		uint32_t BTT;
		DMA_CHANNEL channels[8];	
		
		uint32_t clock;

	public:
		DMA_v1(const std::string& dma_script);	

		// function setting
		void tick();
		void reset();
		void initChannels();

		void check(bool status);
		//
		bool check_btt(uint32_t transfer_count);
		uint32_t select_channel();
		
		// Read Descripotr informations and allocate channels.
		bool solveDescriptors(const std::string& filename, uint8_t* src_addr, uint8_t* dst_addr);
			// channel addr -> device id setting
		
		// channel active for Read
		uint32_t simpleMode(uint32_t channel_id);

		// channel active for Write
		uint32_t burstMode(uint32_t channel_id);
		
		//  DMA Enable or Disable
		bool is_busy();
		
		// set transfer count
		uint32_t setTransferCount();
		

};
