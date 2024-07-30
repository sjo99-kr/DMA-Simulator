#include "Module.hh"

void System(const std::string& dma_descript, const std::string& descripts, const std::string& mem0, const std::string& mem1, const  std::string&  mem2, const std::string& mem3){
	// DMA System setting
	System_Module System_Module(dma_descript, descripts, mem0, mem1, mem2, mem3 );
	System_Module.clock();
	
	std::cout << "************************************************" << std::endl;
	std::cout << "Total Simulation Time = " << System_Module.clk << " cycle(s)" << std::endl;
	std::cout << "************************************************" << std::endl;

}



int main(int argc, char* argv[]){
	if(argc < 7){
		std::cerr << "Usage : " << argv[0] << " <dma_descript> <descriptors> <mem0_trace> <mem1_trace> <mem2_trace> <mem3_trace>" << std::endl;
		return 1;
	}
	
	///////////////////////// Memory setting ////////////////////////////////
	std::string dma_descript = argv[1]; // DMA Module descriptors
	std::string descripts = argv[2]; // DMA Channel descriptors
	std::string mem0 = argv[3];
	std::string mem1 = argv[4];
	std::string mem2 = argv[5];
	std::string mem3 = argv[6];
	/////////////////////////////////////////////////////////////////////////
	
	
	// DMA System setting
	System(dma_descript, descripts, mem0, mem1, mem2, mem3);

	return 0;

}
