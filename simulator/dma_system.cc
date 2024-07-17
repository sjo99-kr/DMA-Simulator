#include "dma_system.hh"




DMA_SYSTEM::DMA_SYSTEM(const std::string& dma_script , const std::string& descriptors, uint8_t* src_addr, uint8_t* dst_addr){

	dma = new DMA_v1(dma_script);
	dma->solveDescriptors(descriptors, src_addr, dst_addr);
	
	std::cout<<"Total Cycles :"  << dma->getClock() << std::endl;


}
