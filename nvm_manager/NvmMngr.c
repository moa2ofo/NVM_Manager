#include "NvmMngr.h"

#include "mcal.h"
#include "pmu.h"
#include "bootrom.h"



static NvmMngr_NvmBlock_t NvmMngr_NvmBlock_[] = 
{
 {
  .startAdress_u32=FLASH_DID_ADRESS,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .writeRequest_b = false,
  .eraseRequest_b = false,
  .prelationFactor_u8 = 0,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0
 },
 {
  .startAdress_u32=EOL_DID_START_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .writeRequest_b = false,
  .eraseRequest_b = false,
  .prelationFactor_u8 = 0,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0
 },
 {
  .startAdress_u32=RESET_CNT_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .writeRequest_b = false,
  .eraseRequest_b = false,
  .prelationFactor_u8 = 0,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0
 },
 {
  .startAdress_u32=CALIBRATION_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .writeRequest_b = false,
  .eraseRequest_b = false,
  .prelationFactor_u8 = 0,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0
 },
 {
  .startAdress_u32=EOL_LOCK_FLAG_ADR,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .writeRequest_b = false,
  .eraseRequest_b = false,
  .prelationFactor_u8 = 0,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0
 },
 {
  .startAdress_u32=REPROG_FLAG_ADDRESS,
  .crc32Ethernet_32 = 0,
  .pageCopy_u8 = {0},
  .writeRequest_b = false,
  .eraseRequest_b = false,
  .prelationFactor_u8 = 0,
  .dataOffset_u8 = 0,
  .dataLen_u8 = 0
 },
};


void NvmMngr_Run_(void)
{
  uint8_t l_iterator_u8 = 0;
  uint8_t l_candidate_u8 = 0;
  uint8_t l_blockSbyTime_u8 = 0;    //32
  /*If there is a request to write I increment the prelationFactor*/
  for(l_iterator_u8=0; l_iterator_u8<N_PAGE_COPY; l_iterator_u8++)
  {
    /*If there is an erase request, it will be served as soon as possible*/
    if((true==NvmMngr_NvmBlock_[l_candidate_u8].eraseRequest_b)&&
      (0==mcal_get_nvmOpResult_u8())
    )
    {
      /** \todo Check if the erasing has been successful*/
      user_nvm_page_erase(NvmMngr_NvmBlock_[l_candidate_u8].startAdress_u32,1u);
      /* Erase the copy of the block to be erased*/
      PageReset_(NvmMngr_NvmBlock_[l_candidate_u8].pageCopy_u8);
      NvmMngr_NvmBlock_[l_candidate_u8].eraseRequest_b = false;
      NvmMngr_NvmBlock_[l_candidate_u8].writeRequest_b = false;
      NvmMngr_NvmBlock_[l_candidate_u8].prelationFactor_u8 = 0;
    }
    /*At every cicle check if there is a writeReq and the saturation factor*/
    if(true == NvmMngr_NvmBlock_[l_iterator_u8].writeRequest_b &&
       NvmMngr_NvmBlock_[l_iterator_u8].prelationFactor_u8 < 0xFF
      )
    {
      /*If there is it, the prelactionFactor will be increased*/
      NvmMngr_NvmBlock_[l_iterator_u8].prelationFactor_u8++;
    }
    else
    {
      NvmMngr_NvmBlock_[l_iterator_u8].prelationFactor_u8=0;
    }
    
  }
  /*Find the block that has the prelation*/
  for(l_iterator_u8 = 0; l_iterator_u8<N_PAGE_COPY; l_iterator_u8++)
  {
    if((0!=NvmMngr_NvmBlock_[l_iterator_u8].prelationFactor_u8)
      &&(NvmMngr_NvmBlock_[l_iterator_u8].prelationFactor_u8>l_blockSbyTime_u8)
    )
    {
      l_blockSbyTime_u8=NvmMngr_NvmBlock_[l_iterator_u8].prelationFactor_u8;
      l_candidate_u8=l_iterator_u8;
    }
  }
  /* check if the peripheral is free */
  if(0==mcal_get_nvmOpResult_u8())
  {
    if(true == NvmMngr_NvmBlock_[l_iterator_u8].writeRequest_b)
    {

      user_nvm_page_write_t l_pageSource_ = {0};
      l_pageSource_.data = &NvmMngr_NvmBlock_[l_iterator_u8].pageCopy_u8[NvmMngr_NvmBlock_[l_iterator_u8].dataOffset_u8]; 
      l_pageSource_.nbyte = NvmMngr_NvmBlock_[l_iterator_u8].dataLen_u8;
      (void)CMSIS_Irq_Dis();
      /* Open SOW */
      PMU_serviceFailSafeWatchdogSOW();
      /* Write to the first page into the user data area of FLASH0 */
      user_nvm_write(EOL_LOCK_FLAG_ADR, &l_pageSource_);
      /* Close SOW by regular WDT trigger */
      PMU_serviceFailSafeWatchdog();
      /* reenable suspended interrupts */
      CMSIS_Irq_En();
      /**\todo Check if the writing has been successful*/
      NvmMngr_NvmBlock_[l_candidate_u8].prelationFactor_u8=0;
      NvmMngr_NvmBlock_[l_candidate_u8].writeRequest_b=false;
    }
  }
}

void PageReset_(uint8_t page_u8[PAGE_LEN])
{
  uint8_t l_iterator_u8 = 0;
  while(l_iterator_u8<PAGE_LEN)
  {
    page_u8[l_iterator_u8] = 0;
    l_iterator_u8++;
  }
}

void ResetRequest_(NvmMngr_PageAddr_t pageToErased_)
{
  NvmMngr_NvmBlock_[pageToErased_].eraseRequest_b = true;
}

bool WriteRequest_(NvmMngr_PageAddr_t pageToErased_,uint8_t pageCopy_u8[PAGE_LEN],
  uint8 offeset_u8,uint8 dataLen_u8)
{
  bool l_requestAccepted =  false;
  uint8_t l_iterator_u8 = 0;
  /*The request will be accepted only if there is not another one*/
  if(NvmMngr_NvmBlock_[pageToErased_].writeRequest_b != true)
  {
    NvmMngr_NvmBlock_[pageToErased_].dataLen_u8 = dataLen_u8;
    NvmMngr_NvmBlock_[pageToErased_].dataOffset_u8 = offeset_u8;
    NvmMngr_NvmBlock_[pageToErased_].writeRequest_b = true;
    for(l_iterator_u8=offeset_u8;l_iterator_u8<offeset_u8+dataLen_u8;l_iterator_u8++)
    {
      NvmMngr_NvmBlock_[pageToErased_].pageCopy_u8[l_iterator_u8] = pageCopy_u8[l_iterator_u8]; 
    }    
  }
  l_requestAccepted = true;
  return l_requestAccepted;
}












