#include "Module.hh"
// clock tick
void System_Module::tick(){
	clk = clk + 1;
}

// Set Modules including DMA and others... (This simulator has only DMA Module)

System_Module::System_Module(const std::string& dma_descript, const std::string& descriptors, const std::string& mem0_trace,
	const std::string& mem1_trace, const std::string& mem2_trace, const std::string& mem3_trace){
	
	/// DMA MODULE INITIALIZATION ///
	DMA_Module = new DMA_v1(dma_descript, descriptors, mem0_trace, mem1_trace, mem2_trace, mem3_trace);
	
	// State Changes for Modules
	state = true;
	}
	
	
void System_Module::clock(){

	bool valid;
	bool src_ready = 1; // from Devie Module
	bool dst_ready = 1; // from Device Module
	bool bus_check = 1; // from CPU Module
	clk = 0;
	while(state==true){
		/// MODUELS ACTION (This simulator has only DMA Moduel, you can change this part if you add Other systems )
		DMA_Module->Action(clk, src_ready, dst_ready, bus_check);
		
		/// YOu CAN ADD MODULES LIKE THIS ///
		/// CPU_Module->Action(clk, bus_request) ... ///
		/// Bus_Module->Action(ready_req, ready_resp) ///
		
		/// State Check for Whole System Module
		state = state_check(DMA_Module->state);
		/// Clock Setting for Whole System
		tick();
	}

	if (state ==false){
		std::cout<< "[ System Module : clock @ " << clk << " ]" <<
		"END ALL PROCESS IN MAIN Module" <<std::endl;
		
	}
}

bool System_Module::state_check(bool DMA_STATE){
	return (DMA_STATE) ? true: false;

}
