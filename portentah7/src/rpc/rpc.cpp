#include "rpc.hpp"


/******************************************************************************/
void printCPU(int cpu)
{
	Serial.println(String("Running on M") + cpu);
}


/******************************************************************************/
void rpcSetup()
{
	if (!Serial) {
		Serial.begin(112500);
		while (!Serial);
	}
	RPC.begin();

#ifdef CORE_CM7
	bootM4();
	RPC.bind("remotePrint", printCPU);
#else
	delay(200);
#endif
}


/******************************************************************************/
void rpcLoop()
{
	while (1) {
		if (RPC.cpu_id() == CM7_CPUID) {
			printCPU(7);
		}
		else {
			RPC.call("remotePrint", 4);
		}
		delay(random(200, 501));
	}
}