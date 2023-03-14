/************************ (C) COPYRIGHT Megahuntmicro *************************
 * @file                : mhscpu_otp.h
 * @author              : Megahuntmicro
 * @version             : V1.0.0
 * @date                : 21-October-2014
 * @brief               : This file contains all the functions prototypes for the OTP firmware library
 *****************************************************************************/

#ifndef __MHSCPU_OTP_H
#define __MHSCPU_OTP_H

#ifdef __cplusplus
extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "mhscpu.h"

/* Exported types ------------------------------------------------------------*/    
typedef enum
{
    OTP_WriteNoDone = 0,
    OTP_WriteDone = 1,
}OTP_WrieDoneTypeDef;

typedef enum
{
    OTP_ReadNotReady = 0,
    OTP_ReadReady = 1,
}OTP_ReadReadyTypeDef;

typedef enum
{
    OTP_UnLocked = 0,
    OTP_Locked = 1,
}OTP_LockTypeDef;
 
typedef enum
{
    OTP_Complete = 0,
    OTP_ReadOnProgramOrSleep = 1,              //�ڱ�̡�����״̬�¶�OTP���ж�����
    OTP_ProgramInROBlock = 2,                  //��ֻ��ȡ���б��
    OTP_ProgramOutOfAddr = 3,                  //��̷�Χ����OTP��Χ
    OTP_ProgramOnSleep = 4,                    //������״̬���б�̲���
    OTP_WakeUpOnNoSleep = 5,                   //�ڷ�����״̬�½��л��Ѳ���
    OTP_TimeOut = 6,                           //OTP��ɱ�־λ��ʱ
    OTP_DataWrong = 7,  
}OTP_StatusTypeDef;
#define IS_OTP_ADDRESS(ADDRESS) (((ADDRESS) > OTP_BASE - 1) && ((ADDRESS) < OTP_BASE + MHSCPU_OTP_SIZE))
     

void OTP_Unlock(void);
void OTP_Lock(void);
void OTP_ClearStatus(void);

void OTP_Init(void);

void OTP_SetProtect(uint32_t u32Addr);
OTP_StatusTypeDef OTP_UnProtect(uint32_t u32Addr);

OTP_LockTypeDef OTP_IsProtect(uint32_t u32Addr);
OTP_LockTypeDef OTP_IsProtectLock(uint32_t u32Addr);
OTP_StatusTypeDef OTP_WriteByte(uint32_t addr, uint8_t w);


#ifdef __cplusplus
}
#endif

#endif

/************************ (C) COPYRIGHT 2014 Megahuntmicro ****END OF FILE****/
