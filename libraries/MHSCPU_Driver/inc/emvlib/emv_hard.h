#ifndef EMV_HARD_H
#ifdef __cplusplus
extern "C" {
#endif
#define EMV_HARD_H

#include "emv_core.h"
    
#define  EMV_TERMINAL_NUM       1
/**
 * these following functions is primary for implementing protocol
 * they are implemented in "xxx_emv_hard.c" and "xxx_emv_hard.h".
 * they provide implemetations to apis layer, in "emv_apis.c" or 
 * "iso7816_3.c".
 * these data-structures( e.g. struct emv_core ) are defined in
 * "emv_core.h".
 *
 * provide hardware code in "xxx_emv_hard.c" version information.
 */
// char* emv_hard_info( char *emv_hard_info );

typedef enum
{
    SCI_Voltage_3000mV = (unsigned int)0x02,
    SCI_Voltage_1800mV = (unsigned int)0x03,
}SCI_VoltageTypeDef;
#define IS_SCI_VOLTAGE(VOLTAGE)                     (((VOLTAGE) == SCI_Voltage_3000mV) || \
                                                     ((VOLTAGE) == SCI_Voltage_1800mV))
    
typedef enum
{
    SCI_CardDetectHighEffective = (unsigned int)0x00,
	SCI_CardDetectLowEffective  = (unsigned int)0x01,
}SCI_CardDetectTypeDef;
#define IS_SCI_CARDDETECT(CARDDETECT)               (((CARDDETECT) == SCI_CardDetectHighEffective) || \
                                                     ((CARDDETECT) == SCI_CardDetectLowEffective))

typedef enum
{
    SCI_VCCENHighEffective      = (unsigned int)0x00,
	SCI_VCCENLowEffective       = (unsigned int)0x01,
}SCI_VCCENTypeDef;
#define IS_SCI_VCCEN(VCCEN)                         (((VCCEN) == SCI_VCCENHighEffective) || \
                                                     ((VCCEN) == SCI_VCCENLowEffective))
	

/**
 * tell channel is whether valid or not?
 */
int isvalid_channel( int slot );

/**
 * tell slot but not hardware convert
 */
int select_slot( int slot );

/**
 * implement hardware initializing.
 */
int   emv_hard_init( void );

/**
 * release device interruption, avoid to exception after rmmod module.
 */
// int emv_hard_release( void );

/**
 * enable/disable insert/extract card interrupts, avoid disturbing
 * SAM card communication. 
 */
int   emv_disturb_interruption( struct emv_core *pdev, int enable );

/**
 * power on with card assigned
 */
int   emv_hard_power_pump( struct emv_core *pdev );

/**
 * executing cold reset with card assigned order by iso7816-3
 */
int   emv_hard_cold_reset( struct emv_core *pdev );

/**
 * executing warn reset with card assigned order by iso7816-3
 */
int   emv_hard_warm_reset( struct emv_core *pdev );

/**
 * executing deactivation with card assigned order by iso7816-3
 */
int   emv_hard_power_dump( struct emv_core *pdev );

/**
 * communicating with card assigned order by iso7816-3
 */
int   emv_hard_xmit( struct emv_core *pdev );

/**
 * detect card with assigned card whether is in socket or not?
 */
int   emv_hard_detect( struct emv_core *pdev );

/**
 * return insert or extract card count.
 */
int   emv_alarm_count( void );

/**
 * Set the glitches filter out the maximum count, and the glitches that occur before
 * the counter counts out are filtered out.
 * The maximum is 0xFFFFF
 */
void emv_hard_SetCardNormalGlf(int clock);

/**
 * select card voltage, can choose 1800mV/3000mV
 */
void emv_hard_VoltageSelect(SCI_VoltageTypeDef SCI_Voltage);

/**
 * select card detect level, can choose SCI_CardDetectHighEffective/SCI_CardDetectLowEffective
 */
void emv_hard_CardDetectSelect(unsigned char SCI_Bitmap, SCI_CardDetectTypeDef CardDetect);

/**
 * select card VCCEN level, can choose SCI_VCCENHighEffective/SCI_VCCENLowEffective
 */
void emv_hard_VCCENSelect(unsigned char SCI_Bitmap, SCI_VCCENTypeDef VccEn);

#ifdef __cplusplus
}
#endif
#endif
