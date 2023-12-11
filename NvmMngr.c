#include "NvmMngr.h"

#include "mcal.h"
#include "pmu.h"
#include "bootrom.h"
#include "product_cfg.h"
#include "string.h"

static uint8 fifoPrelation_u8 = 0;
static uint32 candidate_u32 = 0;

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

sint32 PageWrite_(void)
{
  sint32 l_writingResult_u32 = ERR_LOG_ERROR;
  user_nvm_page_write_t l_pageSource_ = {0};
  l_pageSource_.data = &NvmMngr_NvmPageCopy_[candidate_u32].pageCopy_u8[0]; 
  l_pageSource_.nbyte = UC_FLASH_PAGE_SIZE;
  /* To disable the RWW option*/
  l_pageSource_.options = 1u;
  (void)CMSIS_Irq_Dis();
  /* Open SOW */
  PMU_serviceFailSafeWatchdogSOW();
  /* Write the entire data */
  l_writingResult_u32 = user_nvm_write(NvmMngr_NvmPageCopy_[candidate_u32].startAddrPage, &l_pageSource_);
  /* Close SOW by regular WDT trigger */
  PMU_serviceFailSafeWatchdog();
  /* reenable suspended interrupts */
  CMSIS_Irq_En();
  /* Reset the prelation factor and the request*/
  NvmMngr_NvmPageCopy_[candidate_u32].writeReq_b=false;
  /* Add a free place in the list for the next block*/
  NvmMngr_NvmPageCopy_[candidate_u32].posListFifo_u8 = 0xFF;
  /* Free a place in the FIFO list*/
  fifoPrelation_u8--;
  return l_writingResult_u32; 
}

void NvmMngr_Run_(void)
{
  /* Check if there is al least one page to copy in NVM*/
  if(fifoPrelation_u8>0)
  {
    candidate_u32 = 0;
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

    /* If there is at leat a data to write and the peripheral is not busy*/
    if(true == NvmMngr_NvmPageCopy_[candidate_u32].writeReq_b && ERR_LOG_SUCCESS==mcal_get_nvmOpResult_u8())
    {
      /** \todo Check the result of the operation */
      PageWrite_();
    } 
  }
}

/* Handles the request to write in NVM of the other modules */
void WriteRequest_(NvmMngr_DataPosition_t dataToWrite_,uint8* data)
{
  uint32   l_pageToWrite_u32 = NvmMngr_NvmBlock_[dataToWrite_].belongPage;
  uint32 l_pageStartAdd_u32 = NvmMngr_NvmPageCopy_[l_pageToWrite_u32].startAddrPage;
  uint32 l_dataStartAdd_u32 = NvmMngr_NvmBlock_[dataToWrite_].addrData;
  /* Position used to populate the array that rapresents the page*/
  uint32  l_relativeArrPos_u32 = l_dataStartAdd_u32-l_pageStartAdd_u32;
  uint32 l_dataLen_u32 = NvmMngr_NvmBlock_[dataToWrite_].dataLen;
  NvmMngr_NvmPageCopy_[l_pageToWrite_u32].writeReq_b = true;

  /* Copy the data in the pageCopy_u8 the relative position */
  for(uint32 l_iterator_u32=0;l_iterator_u32<l_dataLen_u32 ;l_iterator_u32++)
  {
    /* Write the datra in the copyPage contained in ram */
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

/* Read the data in NVM */
sint32 NvmMgr_Read_(uint8* output_pu8,NvmMngr_DataPosition_t dataToRead_)
{
  sint32 l_readingResult_u32 = ERR_LOG_ERROR;
//  if(output_pu8 != NULL)
//  {
    uint32 l_pageToWrite_u32  = NvmMngr_NvmBlock_[dataToRead_].belongPage;
    uint32 l_pageStartAdd_u32 = NvmMngr_NvmPageCopy_[l_pageToWrite_u32].startAddrPage;
    uint32 l_dataStartAdd_u32 = NvmMngr_NvmBlock_[dataToRead_].addrData;
    /* Position used to populate the array that rapresents the page*/
    uint32  l_relativeArrPos_u32 = l_dataStartAdd_u32-l_pageStartAdd_u32;
    uint32 l_dataLen_u32 = NvmMngr_NvmBlock_[dataToRead_].dataLen;
    /* Copy the data page in the output array */
    memcpy(output_pu8, &NvmMngr_NvmPageCopy_[l_pageToWrite_u32].pageCopy_u8[l_relativeArrPos_u32], l_dataLen_u32);
    l_readingResult_u32 = ERR_LOG_SUCCESS;
//  }
  return l_readingResult_u32;
}



