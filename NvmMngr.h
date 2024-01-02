/*******************************************************************************
 * \file      NvmMngr.h
 * \brief     NVM manager
 * \author    EAD1 SW Team - Andrea Monti
 * \date      10/12/2023
 * 
 * \par       Copyright (c) 2022, VHIT S.p.A. All rights reserved
 ******************************************************************************/

#ifndef NVMMANAGER_H_
#define NVMMANAGER_H_

/***************************************************************************//**
 *  Includes
 * ****************************************************************************/
#include "types.h"
#include "MemoryMap.h"
#include "tle_variants.h"


/**
 * \enum    NvmMngr_PageAddr_t
 * \brief   This enum lists all possible pages that can be written
 **/
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


/* Struct to represent some features of a page */
typedef struct NvmMngr_NvmBlock_s 
{
  /*size of the data*/
  const uint8 dataLen;
  /*page in which will be written the data*/
  const NvmMngr_PageAddr_t belongPage;
  /*nvm adress of the data */
  const uint32 addrData; 
}NvmMngr_NvmBlock_t;


/* Struct that contains information about the copy in the ram */
typedef struct NvmMngr_NvmPageCopy_s 
{
  /*Contain the data that will be copied in NVM */
  uint8 pageCopy_u8[UC_FLASH_PAGE_SIZE];
  /*used in the FIFO list */
  uint8 posListFifo_u8; 
  /*used to requestyo write*/
  bool writeReq_b;
  /*starting nvm adress of the page*/
  const uint32 startAddrPage;
}NvmMngr_NvmPageCopy_t;


/**
 * \enum    NvmMngr_DataPosition_t
 * \brief   This enum lists all possible data that can be written
 **/
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
  DID_F1A3_16,
  RESET_REASON_17,
  CALIBRATION_18,
  EOL_FLAG_19,
  REPROGRAMMING_FLAG_20
}NvmMngr_DataPosition_t;


void NvmMngr_Run_(void);
void WriteRequest_(NvmMngr_DataPosition_t dataToWrite_,uint8* data);
void NvmMngr_Read_(uint8* output_u8,NvmMngr_DataPosition_t dataToWrite_);


#endif
