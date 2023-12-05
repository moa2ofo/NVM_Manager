#ifndef NVMMANAGER_H_
#define NVMMANAGER_H_

#include "types.h"
#include "MemoryMap.h"

#define PAGE_LEN 128u

typedef struct NvmMngr_NvmBlock_s 
{
  uint32_t startAdress_u32;
  uint32_t crc32Ethernet_32;
  uint8_t  pageCopy_u8[PAGE_LEN];
  bool     writeRequest_b;
  bool     eraseRequest_b;
  uint8_t  prelationFactor_u8;
  uint8_t  dataOffset_u8;
  uint8_t  dataLen_u8;
}NvmMngr_NvmBlock_t;

typedef enum NvmMngr_PageAddr_e
{
	FLASH_DID           = 0,     
	FIRMLY_DID          = 1,          
	RESET_REASON        = 2,                                       
	CALIBRATION         = 3,                                 
	EOL_FLAG            = 4,                                     
	REPROGRAMMING_FLAG  = 5,      
	N_PAGE_COPY         = 6 
}NvmMngr_PageAddr_t;

void NvmMngr_Run_(void);//(NvmMngr_NvmBlock_t NvmMngr_NvmBlock_);
void PageReset_(uint8_t page_u8[PAGE_LEN]);
void ResetRequest_(NvmMngr_PageAddr_t pageToErased_);
bool WriteRequest_(NvmMngr_PageAddr_t pageToErased_,uint8_t pageCopy_u8[PAGE_LEN],
  uint8 offeset_u8,uint8 dataLen_u8);


#endif
