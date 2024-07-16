#include "dma_v1.cc"



class DMA_SYSTEM : public Module {
	private:
		DMA_v1* dma;
	public:
		DMA_SYSTEM(const std::string& dma_script, const std::string& descriptors, uint8_t* src_addr, uint8_t* dst_addr);
};
