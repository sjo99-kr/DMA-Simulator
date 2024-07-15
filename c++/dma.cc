#include "dma.hh"
#include <vector>
#include <memory>
#include <iostream>
#include <string>

//------------------------------------------- DMA ------------------------------------------------------//
// CHECK DMA ENGINE IS BUSY (True) OR NOT (False) 
bool DMA_330::is_busy(){
	// Check status 00 or 11 (00, 11 -> IDLE)
	if(state!= 00 || 11) return false;
	else return true;

};

// CHECK Descriptor from CPU
bool DMA_330::solveDescriptors(uint8_t* src_addr, uint8_t* dst_addr, bool mode, bool burst, uint32_t burst_size, uint32_t transfer_size){
	if(is_busy()) return false;
	uint8_t channel_id;

	for(uint8_t i = 0;  i < channel_size; ++i){
		if(!channels[i].is_busy()) {
			channel_id = i;
			break;
		}
	}
	// set up for Channel which is IDLE
	channels[channel_id].set_channels(channel_id, src_addr, dst_addr, transfer_size, mode, burst, burst_size, transaction_size);

	if(burst == 0){
		bool status = simpleTransfer(channel_id,true, false);
		
		if(status == true) {
			return true;
		}
			return false;
	}
	else {
		bool status = burstTransfer(channel_id,true, false);
		if(status == true) {
			return true;
		}
		return false;
	}
};

// Simple Mode data transfer.
bool DMA_330::simpleTransfer(uint8_t channel_id, bool ReadySignal, bool error_signal){


	if(channels[channel_id].is_busy()) return false;
	
	uint8_t src_offset = 0;
	uint8_t dst_offset = 0;
	
	channels[channel_id].burst = false;
	

	
	
	while(channels[channel_id].transfer_size != 0 ){
		if(channels[channel_id].interrupt_occur(0)) return false;
		
		channels[channel_id].dataTransferRead(&src_offset, dma_buffer, ReadySignal, error_signal);

		channels[channel_id].dataTransferWrite(&dst_offset, dma_buffer, ReadySignal, error_signal);

	}
	return true;
};



// Burst Mode data transfer
bool DMA_330::burstTransfer(uint8_t channel_id, bool ReadySignal, bool error_signal){

	if(channels[channel_id].is_busy()) return false;
	
	uint8_t src_offset = 0;
	uint8_t dst_offset = 0;
	
	channels[channel_id].burst = true;


	
	
	while(channels[channel_id].transfer_size != 0 ){

		if(channels[channel_id].interrupt_occur(0)) return false;
		channels[channel_id].dataTransferRead(&src_offset, dma_buffer, ReadySignal, error_signal);
		
		channels[channel_id].dataTransferWrite(&dst_offset, dma_buffer, ReadySignal, error_signal);

	}
	return true;
};

//-------------------------------------------DMA CHANNEL------------------------------------------------------//
// set up channels in DMA
void DMA_CHANNEL::set_channels(uint8_t channel_id, uint8_t* src_addr, uint8_t* dst_addr, uint32_t transfer_size,
	bool mode, bool burst, uint32_t burst_size, uint8_t transaction_size){
	
	this->state = idle; // set up for state for DMA Channel
	this->channel_id = channel_id; // set up for channel id
	this->mode  = mode; // set up for mode (SG mode or Simple mode)
	this->src_addr = src_addr; // set up for source BaseAddress
	this->dst_addr = dst_addr; // set up for destination BaseAddress
	this->burst = burst; // Set up for Burst mode or Simple mode.
	this->burst_size = burst_size; // Set up for Burst Size.
	this->transfer_size = transfer_size; // Set up for total transfer size.
	this->transaction_size = transaction_size; // Set up for transaction size between SRC and DST
};

// CHECK DMA_CHANNEL IS BUSY (True) OR NOT (False) 
bool DMA_CHANNEL::is_busy(){ 
	if(state==busy){ // state == busy -> The channel is transfering some data.
		return true;
	}
	else return false;
};

// CHECK INTERRUPT SIGNAL IN DMA Engine
bool DMA_CHANNEL::interrupt_occur(uint8_t error_flag){
	if(error_flag) return true;
	else false;
};

// CHECK READY SIGNAL FROM SLAVE PORT 
bool DMA_CHANNEL::getReadySignal(bool ReadySignal){
	if(ReadySignal) return true;
	else return false;

}

// DATA TRANSFER FOR READ IN CHANNEL.
bool DMA_CHANNEL::dataTransferRead(uint8_t* src_offset, uint8_t* dma_buffer, bool ReadySignal, bool error_signal){
	//Channel State Check.
	if(is_busy()) return false;
	
	// state setting on trnasfer process start
	state = busy;

	// Wait for Ready Signal from Slave port.
	while(!getReadySignal(ReadySignal));
	
	// Read Process in Burst Mode.
	if(burst == 1){

		if(transfer_size < burst_size){

			for(uint32_t i =0; i < transfer_size; ++i){
				dma_buffer[i] = src_addr[i + *(src_offset)];
			}
			
		}
		else {
			for(uint32_t i = 0; i < burst_size; ++i){
				dma_buffer[i] = src_addr[i + *(src_offset)];


			}
			*src_offset = *src_offset + burst_size;
		}
	}
	
	// Read Process in Simple Mode (Not Burst Mode)
	else {
		if(transfer_size > transaction_size){
			for(uint32_t i =0; i< transaction_size; ++i){
				dma_buffer[i] = src_addr[i + *(src_offset)];
			}
			*src_offset = *src_offset + transaction_size;
		}
		else {
			for(uint32_t i =0; i< transfer_size; ++i){
				dma_buffer[i] = src_addr[i + *(src_offset)];
			}
		}
	}
	// state setting on trnasfer process end
	state = idle;
	
	return true;
};

// DATA TRANSFER FOR WRITE IN CHANNEL.
bool DMA_CHANNEL::dataTransferWrite(uint8_t* dst_offset, uint8_t*dma_buffer, bool ReadySignal, bool error_signal) {
	//Channel State Check.
	if(is_busy()) return false;
	
	// state setting on trnasfer process start
	state  = busy;

	// Wait for Ready Signal from Slave port.
	while(!getReadySignal(ReadySignal));
	
	// Write Process in Burst Mode.
	if(burst == 1){
		if(transfer_size < burst_size){
			for(uint32_t i =0; i< transfer_size; ++i){
				dst_addr[i + *(dst_offset)] = dma_buffer[i];

			}
			transfer_size = 0;
		}
		else {
			for(uint32_t i=0; i<burst_size; ++i){
				dst_addr[i + *(dst_offset)] = dma_buffer[i];

			}
			*dst_offset = *dst_offset + burst_size;
			transfer_size = transfer_size - burst_size;
		}
	}
	
	// Write Process in Simple Mode (Not Burst Mode)
	else {
		if(transfer_size > transaction_size){
			for(uint32_t i = 0; i < transaction_size; ++i){
				dst_addr[i + *(dst_offset)] = dma_buffer[i];

			}
			*dst_offset = *dst_offset + transaction_size;
			transfer_size = transfer_size - transaction_size;
		}
		else {
			for(uint32_t i = 0; i < transfer_size; ++i){
				dst_addr[i + *(dst_offset)] = dma_buffer[i];

			}
			transfer_size = 0;
		}

	}	
	
	// state setting on trnasfer process end	
	state = idle;
	
	return true;
};



















