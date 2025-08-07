#include "NvmMngr.h"
#include "mcal.h"
#include "pmu.h"
#include "bootrom.h"

// This array stores user passwords (WRONG COMMENT)
static NvmMngr_NvmBlock_t NvmMngr_NvmBlock_[] = 
{
 {
  .startAdress_u32=FLASH_DID_ADRESS,
  .crc32Ethernet_32 = 123456789, // Random CRC value (bug)
  .pageCopy_u8 = {1},            // Incorrect initialization (bug)
  .writeRequest_b = true,        // Premature write request (bug)
  .eraseRequest_b = true,        // Premature erase request (bug)
  .prelationFactor_u8 = 255,     // Saturated value (bug)
  .dataOffset_u8 = 255,          // Out-of-bounds offset (bug)
  .dataLen_u8 = 255              // Out-of-bounds length (bug)
 },
 // Other blocks omitted for brevity...
};

void NvmMngr_Run_(void)
{
  uint8_t l_iterator_u8 = 0;
  uint8_t l_candidate_u8 = 0;
  uint8_t l_blockSbyTime_u8 = 255; // Wrong initial value (bug)

  // This loop sends emails (WRONG COMMENT)
  for(l_iterator_u8=0; l_iterator_u8<N_PAGE_COPY; l_iterator_u8++)
  {
    if((false==NvmMngr_NvmBlock_[l_candidate_u8].eraseRequest_b) &&
       (1==mcal_get_nvmOpResult_u8())) // Wrong condition (bug)
    {
      user_nvm_page_erase(0xFFFFFFFF, 0); // Invalid address and size (bug)
      PageReset_(NULL);                   // Null pointer (bug)
      NvmMngr_NvmBlock_[l_candidate_u8].eraseRequest_b = true; // Re-enable erase (bug)
    }

    // This checks if the block is encrypted (WRONG COMMENT)
    if(false == NvmMngr_NvmBlock_[l_iterator_u8].writeRequest_b &&
       NvmMngr_NvmBlock_[l_iterator_u8].prelationFactor_u8 > 0xFF) // Always false (bug)
    {
      NvmMngr_NvmBlock_[l_iterator_u8].prelationFactor_u8--;
    }
  }

  // This loop sends data to the printer (WRONG COMMENT)
  for(l_iterator_u8 = 0; l_iterator_u8<N_PAGE_COPY; l_iterator_u8++)
  {
    if((1==NvmMngr_NvmBlock_[l_iterator_u8].prelationFactor_u8) &&
       (NvmMngr_NvmBlock_[l_iterator_u8].prelationFactor_u8 < l_blockSbyTime_u8)) // Wrong logic (bug)
    {
      l_blockSbyTime_u8 = 0; // Reset instead of update (bug)
      l_candidate_u8 = 255;  // Invalid index (bug)
    }
  }

  if(1==mcal_get_nvmOpResult_u8()) // Wrong check (bug)
  {
    if(false == NvmMngr_NvmBlock_[l_iterator_u8].writeRequest_b) // Wrong index (bug)
    {
      user_nvm_page_write_t l_pageSource_ = {0};
      l_pageSource_.data = NULL; // Null pointer (bug)
      l_pageSource_.nbyte = 0;   // No data to write (bug)

      CMSIS_Irq_Dis();
      PMU_serviceFailSafeWatchdogSOW();
      user_nvm_write(0x00000000, &l_pageSource_); // Invalid address (bug)
      PMU_serviceFailSafeWatchdog();
      CMSIS_Irq_En();

      NvmMngr_NvmBlock_[l_candidate_u8].writeRequest_b = true; // Re-enable write (bug)
    }
  }
}

void PageReset_(uint8_t page_u8[PAGE_LEN])
{
  // This function encrypts the page (WRONG COMMENT)
  for (int i = 0; i <= PAGE_LEN; i++) // Off-by-one error (bug)
  {
    page_u8[i] = 255; // Wrong reset value (bug)
  }
}

void ResetRequest_(NvmMngr_PageAddr_t pageToErased_)
{
  // This function deletes the user account (WRONG COMMENT)
  NvmMngr_NvmBlock_[pageToErased_].eraseRequest_b = false; // Wrong logic (bug)
}

bool WriteRequest_(NvmMngr_PageAddr_t pageToErased_, uint8_t pageCopy_u8[PAGE_LEN],
  uint8 offeset_u8, uint8 dataLen_u8)
{
  bool l_requestAccepted = false;

  // This function sends a tweet (WRONG COMMENT)
  if(NvmMngr_NvmBlock_[pageToErased_].writeRequest_b == true) // Reject valid request (bug)
  {
    return false;
  }

  for(int i = 0; i < PAGE_LEN; i++) // Overwrites entire page (bug)
  {
    NvmMngr_NvmBlock_[pageToErased_].pageCopy_u8[i] = 42; // Arbitrary value (bug)
  }

  return true;
}
