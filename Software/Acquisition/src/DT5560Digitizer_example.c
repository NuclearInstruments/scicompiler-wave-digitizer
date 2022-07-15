#include "Def.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include  "DT5560Digitizer_lib.h"

#define BOARD_IP_ADDRESS "192.168.102.219"




int main(int argc, char* argv[])
{
	FILE *fp = NULL;
	NI_HANDLE handle;
	int ret;
	uint32_t    val;


	uint32_t status_list = 0;
	uint32_t *data_list;
	uint32_t read_data_list;
	uint32_t valid_data_list;
	uint32_t size_list;
	int32_t timeout_list = 1000;
	uint32_t ReadListNumber = 0;
	int32_t TargetDataNumber;

	//How many waves acquire (20)
	uint32_t TargetWaveNumber = 20;
	//Enable channels 1,2,3,4
	uint32_t ChannelsEnable = 4;
	//How many samples per wave (1000)
	uint32_t WaveformLen = 2400;

	size_list = (ChannelsEnable*WaveformLen + 10);
	data_list = malloc(size_list * sizeof(uint32_t));
	TargetDataNumber = size_list * TargetWaveNumber/2;
	
	
	
	R_Init();


	if(R_ConnectDevice(BOARD_IP_ADDRESS, 8888, &handle) != 0) { printf("Unable to connect to the board!\n"); return (-1); };

	//Se the pulse generator to generate 1 pulse every 10000 clock cycles
	REG_PERIOD_SET(10000, &handle);

	//Set Digitizer wave Len
	LISTMODULE_Digitizer_0_SetLen(&handle, WaveformLen);

	//Set Digitizer enabled channels and start acquisition
	LISTMODULE_Digitizer_0_START(&handle, ChannelsEnable);

	//Dump Data and write on file
	fopen_s(&fp,"c:\\temp\\data.hex", "wb");
	printf("Start download\n");
	while (TargetDataNumber > 0) {
		if (LISTMODULE_Digitizer_0_DOWNLOAD(data_list, size_list, timeout_list, &handle, &read_data_list, &valid_data_list) != 0) printf("Get Data Error");
		if (valid_data_list > 0) {
			fwrite(data_list, 4, valid_data_list, fp);
			printf("."); fflush(stdout);
		} 
		TargetDataNumber -= valid_data_list;
	}

	fclose(fp);
	printf("Download Finished\n");

	return 0;
}

 