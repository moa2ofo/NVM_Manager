#include "NvmMngr.h"

#include "mcal.h"
#include "pmu.h"
#include "bootrom.h"
#include "product_cfg.h"

static uint8_t fifoPrelation_u8 = 0;

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
    .writeReq_b = false,
    .startAddrPage = EOL_DID_START_ADR
  },
  {
    .pageCopy_u8 = {0},
    .posListFifo_u8 = 0xFF,
    .writeReq_b = false,
    .startAddrPage = RESET_CNT_ADR
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
    .addrData   = RESET_CNT_ADR
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




void NvmMngr_Run_(void)
{
  for(uint32_t l_iterator_u32 = 0; l_iterator_u32<N_PAGE_COPY; l_iterator_u32++)
  {
    /* If there is at leat a data to write and the peripheral is not busy*/
    if(true == NvmMngr_NvmPageCopy_[l_iterator_u32].writeReq_b && 0==mcal_get_nvmOpResult_u8())
    {
      /*Copy the entire copy of the page in NVM*/
      user_nvm_page_write_t l_pageSource_ = {0};
      l_pageSource_.data = &NvmMngr_NvmPageCopy_[l_iterator_u32].pageCopy_u8[0]; 
      l_pageSource_.nbyte = PAGE_LEN;
      l_pageSource_.options = 1u;
      (void)CMSIS_Irq_Dis();
      /* Open SOW */
      PMU_serviceFailSafeWatchdogSOW();
      /* Write to the first page into the user data area of FLASH0 */
      user_nvm_write(NvmMngr_NvmPageCopy_[l_iterator_u32].startAddrPage, &l_pageSource_);
      /* Close SOW by regular WDT trigger */
      PMU_serviceFailSafeWatchdog();
      /* reenable suspended interrupts */
      CMSIS_Irq_En();
      /* Reset the prelation factor and the request*/
      NvmMngr_NvmPageCopy_[l_iterator_u32].writeReq_b=false;
      /* Add a free place in the list for the next block*/
      NvmMngr_NvmPageCopy_[l_iterator_u32].posListFifo_u8 = 0xFF;
      fifoPrelation_u8--;
      
    } 
  }
}





/* Handles the request to write in NVM of tghe other modules */
void WriteRequest_(NvmMngr_DataPosition_t dataToWrite_,uint8_t* data)
{
  uint8_t   l_pageToWrite_u8 = NvmMngr_NvmBlock_[dataToWrite_].belongPage;
  uint32_t l_pageStartAdd_u32 = NvmMngr_NvmPageCopy_[l_pageToWrite_u8].startAddrPage;
  uint32_t l_dataStartAdd_u32 = NvmMngr_NvmBlock_[dataToWrite_].addrData;
  uint32_t  l_relativeArrPos_u32 = l_dataStartAdd_u32-l_pageStartAdd_u32;
  uint32_t l_dataLen_u32 = NvmMngr_NvmBlock_[dataToWrite_].dataLen;
  NvmMngr_NvmPageCopy_[l_pageToWrite_u8].writeReq_b = true;

  /* Copy the data in the pageCopy_u8 the relative position */
  for(uint32_t l_iterator_u32=0;l_iterator_u32<l_dataLen_u32 ;l_iterator_u32++)
  {
    /* Write the datra in the copyPage contained in ram */
    NvmMngr_NvmPageCopy_[l_pageToWrite_u8].pageCopy_u8[l_relativeArrPos_u32+l_iterator_u32] = *data;
    data++;      
  }
  if(0xFF==NvmMngr_NvmPageCopy_[l_pageToWrite_u8].posListFifo_u8 )
  {
    NvmMngr_NvmPageCopy_[l_pageToWrite_u8].posListFifo_u8=fifoPrelation_u8;
    fifoPrelation_u8++;
  }  
}












































