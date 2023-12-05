#include "NvmMngr.h"

#include "mcal.h"
#include "pmu.h"
#include "bootrom.h"
#include "product_cfg.h"

static uint8_t fifoList = 0;



static NvmMngr_NvmBlock_t NvmMngr_NvmBlock_[] = 
{
 {
  .startAdress_u32=FLASH_DID_ADRESS,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
  },
  .prelationFactor_u8 = 0xFF,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0,
  .writeRequest_b = false,
  .eraseRequest_b = false

 },
 {
  .startAdress_u32=EOL_DID_START_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
  },
  .prelationFactor_u8 = 0xFF,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0,
  .position_u8 = 0,
  .writeRequest_b = false,
  .eraseRequest_b = false

 },
 {
  .startAdress_u32=RESET_CNT_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
  },
  .prelationFactor_u8 = 0xFF,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0,
  .position_u8 = 0,
  .writeRequest_b = false,
  .eraseRequest_b = false

 },
 {
  .startAdress_u32=CALIBRATION_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
  },
  .prelationFactor_u8 = 0xFF,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0,
  .position_u8 = 0,
  .writeRequest_b = false,
  .eraseRequest_b = false
 },
 {
  .startAdress_u32=EOL_LOCK_FLAG_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
  },
  .prelationFactor_u8 = 0xFF,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0,
  .position_u8 = 0,
  .writeRequest_b = false,
  .eraseRequest_b = false
 },
 {
  .startAdress_u32=REPROG_FLAG_ADDRESS,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .pageComposition={
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
    {DID_F18B_ADRESS,DID_F18B_SIZE},
  },
  .prelationFactor_u8 = 0xFF,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0,
  .position_u8 = 0,
  .writeRequest_b = false,
  .eraseRequest_b = false
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
    /*If there is a request to write I increment the prelationFactor*/
  //  for(l_iterator_u32=0; l_iterator_u32<N_PAGE_COPY; l_iterator_u32++)
  //  {
  //    /*If there is an erase request, it will be served as soon as possible*/
  //    if((true==NvmMngr_NvmBlock_[l_candidate_u32].eraseRequest_b)&&
  //      (0==mcal_get_nvmOpResult_u8())
  //    )
  //    {
  //      /** \todo Check if the erasing has been successful*/
  //      user_nvm_page_erase(NvmMngr_NvmBlock_[l_candidate_u32].startAdress_u32,1u);
  //      /* Erase the copy of the block to be erased*/
  //      PageReset_(NvmMngr_NvmBlock_[l_candidate_u32].pageCopy_u8);
  //      NvmMngr_NvmBlock_[l_candidate_u32].eraseRequest_b = false;
  //      NvmMngr_NvmBlock_[l_candidate_u32].writeRequest_b = false;
  //      NvmMngr_NvmBlock_[l_candidate_u32].prelationFactor_u8 = 0;
  //    }
      /*At every cicle check if there is a writeReq and the saturation factor*/
  //    if(true == NvmMngr_NvmBlock_[l_iterator_u32].writeRequest_b)
  //      
  //    {
  //      /*If there is it, the prelactionFactor will be increased*/
  //      NvmMngr_NvmBlock_[l_iterator_u32].prelationFactor_u8=fifoList;
  //      fifoList++;
  //    }
  //    else
  //    {
  //      NvmMngr_NvmBlock_[l_iterator_u32].prelationFactor_u8=0;
  //    }
      
  //  }
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
        l_pageSource_.data = &NvmMngr_NvmBlock_[l_candidate_u32].
          pageCopy_u8[NvmMngr_NvmBlock_[l_candidate_u32].position_u8]; 
        l_pageSource_.nbyte = NvmMngr_NvmBlock_[l_candidate_u32].pageComposition[
          NvmMngr_NvmBlock_[l_candidate_u32].position_u8].dataLen_u8;
        (void)CMSIS_Irq_Dis();
        /* Open SOW */
        PMU_serviceFailSafeWatchdogSOW();
        /* Write to the first page into the user data area of FLASH0 */
        user_nvm_write(NvmMngr_NvmBlock_[l_candidate_u32].pageComposition[
          NvmMngr_NvmBlock_[l_candidate_u32].position_u8].dataAdress_u32, &l_pageSource_);
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

void ResetRequest_(NvmMngr_PageAddr_t pageToErased_)
{
  NvmMngr_NvmBlock_[pageToErased_].eraseRequest_b = true;
}

bool WriteRequest_(NvmMngr_PageAddr_t pageToWrite_,uint8_t pageCopy_u8[PAGE_LEN],
  uint8 offeset_u8,uint8 dataLen_u8,uint8 position_u8)
{
  bool l_requestAccepted =  false;
  uint8_t l_iterator_u32 = 0;
  /*The request will be accepted only if there is not another one*/
  if(NvmMngr_NvmBlock_[pageToWrite_].writeRequest_b != true)
  {
    NvmMngr_NvmBlock_[pageToWrite_].writeRequest_b = true;
    NvmMngr_NvmBlock_[pageToWrite_].prelationFactor_u8=fifoList;
    NvmMngr_NvmBlock_[pageToWrite_].position_u8 = position_u8;
    fifoList++;
    for(l_iterator_u32=offeset_u8;l_iterator_u32<offeset_u8+dataLen_u8;l_iterator_u32++)
    {
      NvmMngr_NvmBlock_[pageToWrite_].pageCopy_u8[l_iterator_u32] = pageCopy_u8[l_iterator_u32]; 
    }
    l_requestAccepted = true;    
  }
  return l_requestAccepted;
}












