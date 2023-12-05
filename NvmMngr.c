#include "NvmMngr.h"

#include "mcal.h"
#include "pmu.h"
#include "bootrom.h"
#include "product_cfg.h"

static uint8_t fifoList = 0;



static NvmMngr_NvmBlock_t NvmMngr_NvmBlock_[] = 
{
 /* Flash Did */
 {
  .startAdress_u32=FLASH_DID_ADRESS,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {DID_F194_ADRESS,DID_F194_SIZE},
//    {DID_F195_ADRESS,DID_F195_SIZE},
//    {DID_040F_ADRESS,DID_040F_SIZE},
//    {DID_F187_ADRESS,DID_F187_SIZE},
//    {DID_F189_ADRESS,DID_F189_SIZE},
//    {DID_F19E_ADRESS,DID_F19E_SIZE},
//    {DID_F1A2_ADRESS,DID_F1A2_SIZE},
//    {DID_F1D5_ADRESS,DID_F1D5_SIZE}
  },
  .prelationFactor_u8 = 0xFF,
  .writeRequest_b = false

 },
 /* Firmly Did */
 {
  .startAdress_u32=EOL_DID_START_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F1F0_ADRESS,DID_F1F0_SIZE},
    {DID_F192_ADRESS,DID_F192_SIZE},
    {DID_F193_ADRESS,DID_F193_SIZE},
    {DID_F18A_ADRESS,DID_F18A_SIZE},
    {DID_F17C_ADRESS,DID_F17C_SIZE},
    {DID_F191_ADRESS,DID_F191_SIZE},
    {DID_F197_ADRESS,DID_F197_SIZE},
    {DID_F1A3_ADRESS,DID_F1A3_SIZE}
  },
  .prelationFactor_u8 = 0xFF,
  .writeRequest_b = false

 },
 /* Reset reason */
 {
  .startAdress_u32=RESET_CNT_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {RESET_CNT_ADR,RESET_CNT_SIZE}
  },
  .prelationFactor_u8 = 0xFF,
  .writeRequest_b = false

 },
 /* Calibration */
 {
  .startAdress_u32=CALIBRATION_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {CALIBRATION_ADR,CALIBRATION_SIZE}
  },
  .prelationFactor_u8 = 0xFF,
  .writeRequest_b = false
 },
 /*EOL Flag */
 {
  .startAdress_u32=EOL_LOCK_FLAG_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {EOL_LOCK_FLAG_ADR,EOL_LOCK_FLAG_SIZE}
  },
  .prelationFactor_u8 = 0xFF,
  .writeRequest_b = false
 },
 /* Reprogramming Flag */
 {
  .startAdress_u32=REPROG_FLAG_ADDRESS,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {REPROG_FLAG_ADDRESS,REPROG_FLAG_SIZE}
  },
  .prelationFactor_u8 = 0xFF,
  .writeRequest_b = false
 },
};


void NvmMngr_Run_(void)
{
  /* There is at least one request */
  if(fifoList > 0)
  {
    uint32_t l_iterator_u32 = 0;
    uint32_t l_candidate_u32 = 0;
    uint32_t l_fifoOrder_u32 = 0xFF;    //32

    /*Find the block that has arrived first*/
    for(l_iterator_u32 = 0; l_iterator_u32<N_PAGE_COPY; l_iterator_u32++)
    {
      if((true ==NvmMngr_NvmBlock_[l_iterator_u32].writeRequest_b)
        &&(NvmMngr_NvmBlock_[l_iterator_u32].prelationFactor_u8<l_fifoOrder_u32)
      )
      {
        l_fifoOrder_u32=NvmMngr_NvmBlock_[l_iterator_u32].prelationFactor_u8;
        l_candidate_u32=l_iterator_u32;
      }
    }
    /* check if the peripheral is free */
    //if(0==mcal_get_nvmOpResult_u8())
    //{
      if(true == NvmMngr_NvmBlock_[l_candidate_u32].writeRequest_b)
      {

        user_nvm_page_write_t l_pageSource_ = {0};
        l_pageSource_.data = &NvmMngr_NvmBlock_[l_candidate_u32].pageCopy_u8[0]; 
        l_pageSource_.nbyte = PAGE_LEN;
        (void)CMSIS_Irq_Dis();
        /* Open SOW */
        PMU_serviceFailSafeWatchdogSOW();
        /* Write to the first page into the user data area of FLASH0 */
        user_nvm_write(NvmMngr_NvmBlock_[l_candidate_u32].startAdress_u32, &l_pageSource_);
        /* Close SOW by regular WDT trigger */
        PMU_serviceFailSafeWatchdog();
        /* reenable suspended interrupts */
        CMSIS_Irq_En();
        /**\todo Check if the writing has been successful*/
        NvmMngr_NvmBlock_[l_candidate_u32].prelationFactor_u8=0xFF;
        NvmMngr_NvmBlock_[l_candidate_u32].writeRequest_b=false;
        /* Add a free place in the list for the next block*/
        fifoList--;
      }
    //}
  }
}


void PageReset_(uint8_t page_u8[PAGE_LEN])
{
  uint8_t l_iterator_u32 = 0;
  while(l_iterator_u32<PAGE_LEN)
  {
    page_u8[l_iterator_u32] = 0;
    l_iterator_u32++;
  }
}



bool WriteRequest_(NvmMngr_PageAddr_t pageToWrite_,uint8_t* data,uint8 pagePosition_u8)
{
  bool l_requestAccepted =  false;
  /* len of the data to write */
  uint8 dataToWriteLen = NvmMngr_NvmBlock_[pageToWrite_].pageComposition[pagePosition_u8].dataLen_u8;
  /*Position used in the pageCopy_u8 */
  uint32 relativeArrPos =  NvmMngr_NvmBlock_[pageToWrite_].pageComposition[pagePosition_u8].dataAdress_u32-
    NvmMngr_NvmBlock_[pageToWrite_].startAdress_u32;
  /*The request will be accepted for the first data to write in the page*/
  if(NvmMngr_NvmBlock_[pageToWrite_].writeRequest_b != true)
  {
    
    NvmMngr_NvmBlock_[pageToWrite_].writeRequest_b = true;
    NvmMngr_NvmBlock_[pageToWrite_].prelationFactor_u8=fifoList;
    fifoList++; 
  }
  /* Copy the data in the pageCopy_u8 the relative position */
  for(uint32_t l_iterator_u32=0;l_iterator_u32<dataToWriteLen ;l_iterator_u32++)
  {
    NvmMngr_NvmBlock_[pageToWrite_].pageCopy_u8[relativeArrPos+l_iterator_u32] = *data;
    data++;      
  }
  l_requestAccepted = true;
  return l_requestAccepted;
}












































