#ifndef MCAL_H_
#define MCAL_H_

/* Includes */
#include "Constants.h"
#include "GlobalTypes.h"
#include "ConfigurableParameters.h"

#include "isr_defines.h"
#include "bootrom.h"

/*******************LinAppl************************/
void MCAL_uartBusEnableCommunication(void);
void MCAL_uartBusDisableCommunication(void);
/*******************Schm***************************/
void MCAL_gpt12StartT3(void);
void MCAL_gpt12SetT3Val(uint16 val);
/*******************MotCtrl_wrap*******************/
void MCAL_scuDelayUs(uint32 delay);
/*******************VoltMon************************/

/*******************TempMon************************/

/*******************SpeedMon***********************/

/*******************CurrentMon*********************/

/*******************HwRevIdentifier****************/

/***************************************************************************//**
 * \defgroup    HwRevId_Defines     HwRevIdentifier Defines/Macros
 * @{
 ******************************************************************************/
/** Number of possible voltage levels and, by consequence, HW revisions */
#define NUM_VOLT_LEVELS           9u
/** Dimension in ADC digits from one level to another (range) */
#define LEVEL_DIM_DIGITS        ( 0x3fffu / (NUM_VOLT_LEVELS - 1u) )
/** Tolerance for a voltage level definition in ADC digits */
#define TOLERANCE_DIGITS        ( LEVEL_DIM_DIGITS / 10u )
/** Default HW revision currently used */
#define DEFAULT_HW_REV          ( (uint8)HW_REV_NOT_RECOGNISED - 1u )

/** @} HwRevId_Defines */ 

/***************************************************************************//**
 * \defgroup    HwRevId_ADC1Interfaces  HwRevIdentifier ADC1 Interfaces
 * \ingroup     HwRevId
 * \brief       Interfaces toward ADC1 peripheral of TLE989x microcontroller 
 * @{
 ******************************************************************************/

HwRevIdentifier_hwRev_t MCAL_getHwRev(uint32 lvlIdx);
uint16 MCAL_getRawValHwVersTab(uint32 lvlIdx);
void MCAL_adc1StartMeasHwRevId(void);
uint8 MCAL_adc1IsMeasCompletedHwRevId(void);
uint16 MCAL_adc1RawValHwRevId(void);

/** @} HwRevId_ADC1Interfaces */

/*******************SingleShuntMeas****************/


/***************************************************************************//**
 * \defgroup    SSMM_CCU7Interfaces     SSMM CCU7 Interfaces
 * \ingroup     SSMM
 * \brief       Interfaces toward CCU7 peripheral of TLE989x microcontroller 
 * @{
 ******************************************************************************/
void MCAL_ccu7SetFirstTimeMeasTime(uint16 val);
void MCAL_ccu7SetSecondTimeMeasTime(uint16 val);
void MCAL_ccu7StartTimer(void);
void MCAL_ccu7StopTimer(void);
void MCAL_ccu7ReqShadowTrans(void);

/** @} SSMM_CCU7Interfaces */


/***************************************************************************//**
 * \defgroup    SSMM_CSACSC_Interfaces     SSMM CSACSC Interfaces
 * \ingroup     SSMM
 * \brief       Interfaces toward CSACSC peripheral of TLE989x microcontroller 
 * @{
 ******************************************************************************/

uint32 MCAL_csaGainVal(void);
sint8 MCAL_csaStartCalib(uint16 *pVal);
uint32 MCAL_csaRShuntWithGain(void);

/** @} SSMM_CSACSC_Interfaces */

/***************************************************************************//**
 * \defgroup    SSMM_ADC1_Defines     SSMM ADC1 Defines
 * \ingroup     SSMM
 * \brief       Defines for ADC1 module 
 * @{
 ******************************************************************************/
/** \brief Voltage reference for ADC conversion */
#define SSMM_ADC_VREF                         (5000u)

/** \brief ADC1 resolution (10 bits) */
#define SSMM_ADC_RESOLUTION                   (0x3FFFu)

/** @} SSMM_ADC1_Defines */

uint16 MCAL_adc1FirstMeas(void);
uint16 MCAL_adc1SecondMeas(void);

/*******************MotPwm*************************/
/***************************************************************************//**

 * \defgroup    MotPwm_CCU7RegVal            MotPwm CCU7 Register Macros

 * \ingroup     MotPwm

 * \brief       Interfaces toward CCU7 peripheral of TLE989x microcontroller 

 * @{

 ******************************************************************************/

/** \brief T12MODEN register macros */
#define MOTPWM_T12MODEN_EN_CC70_MODUL               0x01u
#define MOTPWM_T12MODEN_EN_COUT70_MODUL             0x02u
#define MOTPWM_T12MODEN_EN_CC71_MODUL               0x04u
#define MOTPWM_T12MODEN_EN_COUT71_MODUL             0x08u
#define MOTPWM_T12MODEN_EN_CC72_MODUL               0x10u
#define MOTPWM_T12MODEN_EN_COUT72_MODUL             0x20u

#define MOTPWM_T12MODEN_EN_3_PHASE_PWM    ( MOTPWM_T12MODEN_EN_CC70_MODUL   |  \
                                            MOTPWM_T12MODEN_EN_COUT70_MODUL |  \
                                            MOTPWM_T12MODEN_EN_CC71_MODUL   |  \
                                            MOTPWM_T12MODEN_EN_COUT71_MODUL |  \
                                            MOTPWM_T12MODEN_EN_CC72_MODUL   |  \
                                            MOTPWM_T12MODEN_EN_COUT72_MODUL    )
																						
#define MOTPWM_T12MODEN_DIS_3_PHASE_PWM             0x00u



void MCAL_pwmResetTimer(void);
void MCAL_pwmClearOMInt(void);
void MCAL_pwmClearPMInt(void);
void MCAL_pwmEnableOMInt(void);
void MCAL_pwmEnablePMInt(void);
void MCAL_pwmDisableOMInt(void);
void MCAL_pwmDisablePMInt(void);
void MCAL_pwmStartTimer(void);
void MCAL_pwmStopTimer(void);
void MCAL_pwmSetModulation(uint8 val);
void MCAL_pwmSetPhUCompValRise(uint16 val);
void MCAL_pwmSetPhUCompValFall(uint16 val);
void MCAL_pwmSetPhVCompValRise(uint16 val);
void MCAL_pwmSetPhVCompValFall(uint16 val);
void MCAL_pwmSetPhWCompValRise(uint16 val);
void MCAL_pwmSetPhWCompValFall(uint16 val);
void MCAL_pwmReqShadowTrans(void);
void MCAL_pwmSetBridge(void);
void MCAL_pwmClearBridgeSts(void);
/*******************TempMeas***********************/
sint16 MCAL_convertFromRawToDegrees(uint16 raw_val);
sint16 MCAL_calcTemp(uint16 vmeas);
sint8 MCAL_adc1ChannelRead(uint16 *u16p_digValue_mV, uint8 u8_channel);
sint8 MCAL_adc1ChannelRead_mV(uint16 *u16p_digValue_mV, uint8 u8_channel);
sint8 MCAL_adc1StartSequence(uint8 u8_seqIdx);
/*******************VoltMeas***********************/
/** Macro for adc2 channel read */
sint8 MCAL_adc2ChannelRead_mV(uint16 *u16p_digValue_mV, uint8 u8_channel);
/********************NvmMngr***********************/
uint32 mcal_get_nvmOpResult_u32(void);
void mcal_nvm_write(uint32 addrPage, uint8* data);

#endif
