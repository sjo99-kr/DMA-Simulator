#include "dma_system.hh"




DMA_SYSTEM::DMA_SYSTEM(const std::string& dma_script , const std::string& descriptors,
		const std::string& mem0_trace, const std::string& mem1_trace, const std::string& mem2_trace, const std::string& mem3_trace ){

	dma = new DMA_v1(dma_script, mem0_trace, mem1_trace, mem2_trace, mem3_trace);

	
	dma->solveDescriptors(descriptors);
	
	std::cout<<"Total Cycles :"  << dma->getClock() << std::endl;


}
