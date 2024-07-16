/*

	DMA MODELING 

*/
#include <stdint.h>
#include <string>

#define busy true
#define idle false

class DMA_CHANNEL
{
	private:

	public:	
		bool state; // Channel State
		uint8_t* src_addr; // Source base addr
		uint8_t* dst_addr; // Destination base addr
		uint8_t channel_id; // set channel ID
		uint32_t transfer_size; // trnasfer number count
		uint32_t burst_size; // burst Size check
		uint8_t transaction_size; // data width
		
		bool mode; // SG mode or Simple Mode (true / false)
		bool burst; // Burst mode or Normal Mode  (true / false)
		
		// check DMA CHANNEL is busy or not
		bool is_busy();
		
		// Set Channal for transaction
		void set_channels(uint8_t channel_id, uint8_t* src_addr, uint8_t* dst_addr, uint32_t transfer_size,
			bool mode, bool burst, uint32_t burst_size, uint8_t transaction_size );

		// transfer data to DST 
		// mode_selection -> burst mode or simple mode
		bool interrupt_occur(uint8_t error_signal);
		
		// ready check
		bool getReadySignal(bool ReadySignal);
		
		// Transfer data from DMA buffer to other place 
		bool dataTransferRead(uint8_t* src_offset, uint8_t* dma_buffer, bool ReadySignal, bool error_signal);
		bool dataTransferWrite(uint8_t* dst_offset, uint8_t* dma_buffer, bool ReadySignal, bool error_signal);
};




class DMA_v1 // state , mode , transaction_size
{
	private:
	public:
		DMA_330(uint8_t state, uint32_t burst_size, bool mode, uint8_t transaction_size) :
			transaction_size(transaction_size), mode(mode), burst_size(burst_size), state(state){
			dma_buffer =   new uint8_t[burst_size];
		}
	
		uint8_t channel_size = 8; // default value for DMA 330
		
		// dma state
		uint8_t state; // 00, 11 -> Idle state
		
		// base address setting
		uint8_t* source_addr; 
		uint8_t* destination_addr;
		
		// burst mode setting
		uint32_t burst_size;
		uint8_t* dma_buffer;
		// transaction setting
		uint8_t transaction_size;  // data width for transaction
		
		// dma mode setting
		bool mode; // SG mode : true, simple mode : false
		
		DMA_CHANNEL channels[8];

		// Read Descripotr informations and allocate channels.
		bool solveDescriptors(uint8_t* src_addr, uint8_t* dst_addr,
			bool mode, bool burst, uint32_t burst_size, uint32_t transfer_size);
		
		// channel active for Read
		bool simpleTransfer(uint8_t channel_id, bool ready_signal, bool error_signal);

		// channel active for Write
		bool burstTransfer(uint8_t channel_id, bool ready_signal, bool error_signal);
		
		//  DMA Enable or Disable
		bool is_busy();
		
		// set transfer count
		uint32_t setTransferCount();
		

};



