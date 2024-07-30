#include <stdint.h>
#include <assert.h>
#include <iostream>
#include "dma_v1.hh"

class System_Module {

	protected:
	public: 
		System_Module(const std::string& dma_descript, const std::string& descriptors,
		const std::string& mem0_trace, const std::string& mem1_trace, const std::string& mem2_trace, 
		const std::string& mem3_trace);
		
		DMA_v1* DMA_Module;
		uint32_t clk;
		bool state;
		void tick();
		void clock();
		bool state_check(bool DMA_State);

};


