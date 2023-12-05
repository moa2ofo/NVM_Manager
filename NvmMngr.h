#ifndef NVMMANAGER_H_
#define NVMMANAGER_H_

#include "types.h"
#include "MemoryMap.h"

#define PAGE_LEN 128u



typedef struct NvmMngr_DataParameter_s 
{
  uint32_t dataAdress_u32;
  uint8_t  dataLen_u8;
}NvmMngr_DataParameter_t;

/* Struct to reprasent some features of a page */
typedef struct NvmMngr_NvmBlock_s 
{
  uint32_t startAdress_u32;
  uint32_t crc32Ethernet_32;
  uint8_t  pageCopy_u8[PAGE_LEN];
  NvmMngr_DataParameter_t pageComposition[10];
  uint8_t  prelationFactor_u8;
  bool     writeRequest_b;
}NvmMngr_NvmBlock_t;

/* Enum that contain each page to write*/
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


/* Enum that contain the position of each data in a single page*/
typedef enum NvmMngr_Flash_Did_Page_e
{
  FLASH_PAGE_1_F194=0,
  FLASH_PAGE_2_F195,
  FLASH_PAGE_3_040F,
  FLASH_PAGE_4_F187,
  FLASH_PAGE_5_F189,
  FLASH_PAGE_6_F19E,
  FLASH_PAGE_7_F1A2,
  FLASH_PAGE_8_F1D5
  
}NvmMngr_Flash_Did_Page_t;

typedef enum NvmMngr_Firmly_Did_Page_e
{
  FIRMLY_PAGE_1_F18B=0,
  FIRMLY_PAGE_2_F1F0,
  FIRMLY_PAGE_3_F192,
  FIRMLY_PAGE_4_F193,
  FIRMLY_PAGE_5_F18A,
  FIRMLY_PAGE_6_F17C,
  FIRMLY_PAGE_7_F191,
  FIRMLY_PAGE_8_F197,
  FIRMLY_PAGE_9_F1A3  
}NvmMngr_Firmly_Did_Page_e;

typedef enum NvmMngr_Reset_Page_e
{
  RESET_REASON_PAGE_0=0
}NvmMngr_Reset_Page_e;

typedef enum NvmMngr_Calibration_Page_e
{
  CALIBRATION_PAGE_0=0
}NvmMngr_Calibration_Page_e;

typedef enum NvmMngr_EolFlag_Page_e
{
  EOL_FLAG_PAGE_0=0
}NvmMngr_EolFlag_Page_e;

typedef enum NvmMngr_ReprogrammingFlag_Page_e
{
  REPROGRAMMING_FLAG_PAGE_0=0
}NvmMngr_ReprogrammingFlag_Page_e;



void NvmMngr_Run_(void);//(NvmMngr_NvmBlock_t NvmMngr_NvmBlock_);
void PageReset_(uint8_t page_u8[PAGE_LEN]);
void ResetRequest_(NvmMngr_PageAddr_t pageToErased_);
bool WriteRequest_(NvmMngr_PageAddr_t pageToWrite_,uint8_t* data,uint8 pagePosition_u8);


#endif
