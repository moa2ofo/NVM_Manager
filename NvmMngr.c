/*******************************************************************************
 * \file      NvmMngr.c
 * \brief     NVM manager
 * \author    EAD1 SW Team - Andrea Monti
 * \date      10/12/2023
 * 
 * \par       Copyright (c) 2022, VHIT S.p.A. All rights reserved
 ******************************************************************************/

/*******************************************************************************
 *  Includes
 * ****************************************************************************/
#include "NvmMngr.h"

#include "mcal.h"
#include "pmu.h"
#include "bootrom.h"
#include "product_cfg.h"
#include "string.h"

#define FLASH_NOT_BUSY 0u


static uint8 fifoPrelation_u8 = 0;
static uint32 candidate_u32 = 0;

/**************************************************************************//**
 * \struct  NvmMngr_NvmPageCopy_  
 * \brief   Struct containing info about the copy of the NVM page 
 ******************************************************************************/
static NvmMngr_NvmPageCopy_t NvmMngr_NvmPageCopy_[] =
{
  {
    .pageCopy_u8 = {0},
    .posListFifo_u8 = 0xFF,
    .writeReq_b = false,
    .startAddrPage = FLASH_DID_ADRESS
  },
  {
    .pageCopy_u8 = {0},
    .posListFifo_u8 = 0xFF,
    .writeReq_b = false,
    .startAddrPage = EOL_DID_START_ADR
  },
  {
    .pageCopy_u8 = {0},
    .posListFifo_u8 = 0xFF,
    .writeReq_b = false,
    .startAddrPage = RESET_REASON_ADR
  },
  {
    .pageCopy_u8 = {0},
    .posListFifo_u8 = 0xFF,
    .writeReq_b = false,
    .startAddrPage = CALIBRATION_ADR
  },
  {
    .pageCopy_u8 = {0},
    .posListFifo_u8 = 0xFF,
    .writeReq_b = false,
    .startAddrPage = EOL_LOCK_FLAG_ADR
  },
  {
    .pageCopy_u8 = {0},
    .posListFifo_u8 = 0xFF,
    .writeReq_b = false,
    .startAddrPage = REPROG_FLAG_ADDRESS
  },
};


/**************************************************************************//**
 * \struct  NvmMngr_NvmPageCopy_  
 * \brief   Struct containing info about all data that can be written in NVM
 ******************************************************************************/
static NvmMngr_NvmBlock_t NvmMngr_NvmBlock_[] = 
{
  /* Flash Did */
  {
    .dataLen    = DID_F194_SIZE,
    .belongPage = FLASH_DID_PAGE,
    .addrData   = DID_F194_ADRESS
  },
  {
    .dataLen    = DID_F195_SIZE,
    .belongPage = FLASH_DID_PAGE,
    .addrData   = DID_F195_ADRESS
  },
  {
    .dataLen    = DID_040F_SIZE,
    .belongPage = FLASH_DID_PAGE,
    .addrData   = DID_040F_ADRESS
  },
  {
    .dataLen    = DID_F187_SIZE,
    .belongPage = FLASH_DID_PAGE,
    .addrData   = DID_F189_ADRESS
  },
  {
    .dataLen    = DID_F19E_SIZE,
    .belongPage = FLASH_DID_PAGE,
    .addrData   = DID_F19E_ADRESS
  },
  {
    .dataLen    = DID_F19E_SIZE,
    .belongPage = FLASH_DID_PAGE,
    .addrData   = DID_F19E_ADRESS
  },
  {
    .dataLen    = DID_F1A2_SIZE,
    .belongPage = FLASH_DID_PAGE,
    .addrData   = DID_F1A2_ADRESS
  },
  {
    .dataLen    = DID_F1D5_SIZE,
    .belongPage = FLASH_DID_PAGE,
    .addrData   = DID_F1D5_ADRESS
  },
  /* Firmly Did */  
  {
    .dataLen    = DID_F18B_SIZE,
    .belongPage = FIRMLY_DID_PAGE,
    .addrData   = DID_F18B_ADRESS
  },
  {
    .dataLen    = DID_F1F0_SIZE,
    .belongPage = FIRMLY_DID_PAGE,
    .addrData   = DID_F1F0_ADRESS
  },
  {
    .dataLen    = DID_F192_SIZE,
    .belongPage = FIRMLY_DID_PAGE,
    .addrData   = DID_F192_ADRESS
  },
  {
    .dataLen    = DID_F193_SIZE,
    .belongPage = FIRMLY_DID_PAGE,
    .addrData   = DID_F193_ADRESS
  },
  {
    .dataLen    = DID_F18A_SIZE,
    .belongPage = FIRMLY_DID_PAGE,
    .addrData   = DID_F18A_ADRESS
  },
  {
    .dataLen    = DID_F17C_SIZE,
    .belongPage = FIRMLY_DID_PAGE,
    .addrData   = DID_F17C_ADRESS
  },
  {
    .dataLen    = DID_F191_SIZE,
    .belongPage = FIRMLY_DID_PAGE,
    .addrData   = DID_F191_ADRESS
  },
  {
    .dataLen    = DID_F197_SIZE,
    .belongPage = FIRMLY_DID_PAGE,
    .addrData   = DID_F197_ADRESS
  },
  {
    .dataLen    = DID_F1A3_SIZE,
    .belongPage = FIRMLY_DID_PAGE,
    .addrData   = DID_F1A3_ADRESS
  },
  /* Reset reason */
  {
    .dataLen    = RESET_CNT_SIZE,
    .belongPage = RESET_REASON_PAGE,
    .addrData   = RESET_REASON_ADR
  },
  /* Calibration */
  {
    .dataLen    = CALIBRATION_SIZE,
    .belongPage = CALIBRATION_PAGE,
    .addrData   = CALIBRATION_ADR
  },
  /* EOL Flag */
  {
    .dataLen    = EOL_LOCK_FLAG_SIZE,
    .belongPage = EOL_FLAG_PAGE,
    .addrData   = EOL_LOCK_FLAG_ADR                                          
  },
  /* Reprogramming Flag */
  {
    .dataLen    = REPROG_FLAG_SIZE,
    .belongPage = REPROGRAMMING_FLAG_PAGE,
    .addrData   = REPROG_FLAG_ADDRESS
  },
};


/***************************************************************************//**
@brief         PageWrite_
@details       Used to write data in NVM
@param[in]     none
@return        l_writingResult_u32
*******************************************************************************/
INLINE void PageWrite_(uint32 opStatus_u32)
{
  if(FLASH_NOT_BUSY==opStatus_u32)
  {

    mcal_nvm_write(NvmMngr_NvmPageCopy_[candidate_u32].startAddrPage, &NvmMngr_NvmPageCopy_[candidate_u32].pageCopy_u8[0]);

    /* Reset the prelation factor and the request*/
    /**\todo uncheck it only after chech the result of PageWrite_ function*/
    NvmMngr_NvmPageCopy_[candidate_u32].writeReq_b=false;
    /* Add a free place in the list for the next block*/
    NvmMngr_NvmPageCopy_[candidate_u32].posListFifo_u8 = 0xFF;
    /* Free a place in the FIFO list*/
    fifoPrelation_u8--;
  } 
}


/***************************************************************************//**
@brief         NvmMngr_Run_
@details       NvmMngr runnable.
@param[in]     none
@return        none
*******************************************************************************/
void NvmMngr_Run_(void)
{
  /* Check if there is al least one page to copy in NVM*/
  if(fifoPrelation_u8>0)
  {
    candidate_u32 = 0;
    uint32 operationStatus = 1;
    uint32 l_fifoOrder_u32 = 0xFF;
    /*Find the first block that has request to be served */
    for(uint32 l_iterator_u32 = 0; l_iterator_u32<N_PAGE_COPY; l_iterator_u32++)
    {
      if((true ==NvmMngr_NvmPageCopy_[l_iterator_u32].writeReq_b)
        &&(NvmMngr_NvmPageCopy_[l_iterator_u32].posListFifo_u8<l_fifoOrder_u32)
      )
      {
        l_fifoOrder_u32=NvmMngr_NvmPageCopy_[l_iterator_u32].posListFifo_u8;
        candidate_u32=l_iterator_u32;
      }
    }
    operationStatus = mcal_get_nvmOpResult_u32();
    /* If there is at leat a data to write and the peripheral is not busy*/
    /*
      This ondition can be deleted because if fifoprelation is higher than 0 there is at least a request
      and NvmMngr_NvmPageCopy_[candidate_u32].writeReq_b will be always true 
    */
    //if(true == NvmMngr_NvmPageCopy_[candidate_u32].writeReq_b)
    //{
      /** \todo Check the result of the operation */
    PageWrite_(operationStatus);
    //} 
  }
}


/***************************************************************************//**
@brief         WriteRequest_
@details       Handles the request to write in NVM of the other modules
@param[in]     dataToWrite_ - contains position and len of data to write
               data         - data to write
@return        none
*******************************************************************************/
void WriteRequest_(NvmMngr_DataPosition_t dataToWrite_,uint8* data)
{
  uint32 l_pageToWrite_u32 = NvmMngr_NvmBlock_[dataToWrite_].belongPage;
  uint32 l_pageStartAdd_u32 = NvmMngr_NvmPageCopy_[l_pageToWrite_u32].startAddrPage;
  uint32 l_dataStartAdd_u32 = NvmMngr_NvmBlock_[dataToWrite_].addrData;
  /* Position used to populate the array that rapresents the page*/
  uint32  l_relativeArrPos_u32 = l_dataStartAdd_u32-l_pageStartAdd_u32;
  uint32 l_dataLen_u32 = NvmMngr_NvmBlock_[dataToWrite_].dataLen;
  NvmMngr_NvmPageCopy_[l_pageToWrite_u32].writeReq_b = true;

  /* Copy the data in the pageCopy_u8 the relative position */
  for(uint32 l_iterator_u32=0;l_iterator_u32<l_dataLen_u32 ;l_iterator_u32++)
  {
    /* Write the data in the copyPage contained in ram */
    NvmMngr_NvmPageCopy_[l_pageToWrite_u32].pageCopy_u8[l_relativeArrPos_u32+l_iterator_u32] = *data;
    data++;      
  }
  /* A position in the list is assigned only the first time after the page it has been copied*/
  if(0xFF==NvmMngr_NvmPageCopy_[l_pageToWrite_u32].posListFifo_u8 )
  {
    NvmMngr_NvmPageCopy_[l_pageToWrite_u32].posListFifo_u8=fifoPrelation_u8;
    /* Increment the position for the next page that will enter */
    fifoPrelation_u8++;
  }  
}


/***************************************************************************//**
@brief         NvmMngr_Read_
@details       Read the data contained in the copy of the NVM
@param[in]     output_pu8  - buffer data to write,
               dataToRead_ - contains position and len of data to read
@return        none
*******************************************************************************/
void NvmMngr_Read_(uint8* output_pu8,NvmMngr_DataPosition_t dataToRead_)
{
    uint32 l_pageToWrite_u32  = NvmMngr_NvmBlock_[dataToRead_].belongPage;
    uint32 l_pageStartAdd_u32 = NvmMngr_NvmPageCopy_[l_pageToWrite_u32].startAddrPage;
    uint32 l_dataStartAdd_u32 = NvmMngr_NvmBlock_[dataToRead_].addrData;
    /* Position used to populate the array that rapresents the page*/
    uint32  l_relativeArrPos_u32 = l_dataStartAdd_u32-l_pageStartAdd_u32;
    uint32 l_dataLen_u32 = NvmMngr_NvmBlock_[dataToRead_].dataLen;
    /* Copy the data page in the output array */
    memcpy(output_pu8,&NvmMngr_NvmPageCopy_[l_pageToWrite_u32].pageCopy_u8[l_relativeArrPos_u32],l_dataLen_u32);
}



