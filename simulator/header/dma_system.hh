#include "dma_v1.cc"

class DMA_SYSTEM : public Module {
	private:
		DMA_v1* dma;
	public:
		DMA_SYSTEM(const std::string& dma_script, const std::string& descriptors,
		const std::string& mem0_trace, const std::string& mem1_trace, const std::string& mem2_trace, const std::string& mem3_trace );

};
