#include <stdint.h>
#include <assert.h>
#include <iostream>

class Module {

	protected:
		uint32_t clock;
	public: 
		Module() { clock = 0;}
		virtual ~ Module() {}

		void tick(){
			clock = clock + 1;
		};
		void trans_tick(){
			clock = clock + 3; // trans penalty;
		}
		void latency(){
			clock = clock + 5;
		}
		void add_clock(uint32_t num){
			clock = clock + num;
		}
		uint32_t getClock() const  { return clock; };

};



