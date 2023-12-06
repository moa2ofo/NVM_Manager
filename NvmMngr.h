#ifndef NVMMANAGER_H_
#define NVMMANAGER_H_

#include "types.h"
#include "MemoryMap.h"

#define PAGE_LEN 128u


/* Enum that contain each page to write*/
typedef enum NvmMngr_PageAddr_e
{
	FLASH_DID_PAGE           = 0,     
	FIRMLY_DID_PAGE          = 1,          
	RESET_REASON_PAGE        = 2,                                       
	CALIBRATION_PAGE         = 3,                                 
	EOL_FLAG_PAGE            = 4,                                     
	REPROGRAMMING_FLAG_PAGE  = 5,      
	N_PAGE_COPY              = 6 
}NvmMngr_PageAddr_t;


/* Struct to reprasent some features of a page */
typedef struct NvmMngr_NvmBlock_s 
{
  const uint8_t dataLen;  
  const NvmMngr_PageAddr_t belongPage;
  const uint32_t addrData; 
}NvmMngr_NvmBlock_t;



typedef struct NvmMngr_NvmPageCopy_s 
{
  uint8_t pageCopy_u8[PAGE_LEN];
  uint8_t posListFifo_u8; 
  bool writeReq_b;
  const uint32_t startAddrPage;
}NvmMngr_NvmPageCopy_t;



/* Enum that contain each page to write*/
typedef enum NvmMngr_DataPosition_e
{
  DID_F194_0=0,  
  DID_F195_1,  
  DID_040F_2,  
  DID_F187_3,
  DID_F189_4,  
  DID_F19E_5,  
  DID_F1A2_6,  
  DID_F1D5_7,
  DID_F18B_8,  
  DID_F1F0_9,  
  DID_F192_10,  
  DID_F193_11,  
  DID_F18A_12,  
  DID_F17C_13,  
  DID_F191_14,  
  DID_F197_15, 
  DID_F1A3_16  
}NvmMngr_DataPosition_t;


void NvmMngr_Run_(void);//(NvmMngr_NvmBlock_t NvmMngr_NvmBlock_);
void WriteRequest_(NvmMngr_DataPosition_t pageToWrite_,uint8_t* data);


#endif
