/*****************************************************************************
 *
 * MODULE:             JN-AN-1218
 *
 * COMPONENT:          app_main.c
 *
 * DESCRIPTION:        Light bulb application main file
 *
 *****************************************************************************
 *
 * This software is owned by NXP B.V. and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on NXP products [NXP Microcontrollers such as JN5168, JN5179].
 * You, and any third parties must reproduce the copyright and warranty notice
 * and any other legend of ownership on each copy or partial copy of the
 * software.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Copyright NXP B.V. 2017. All rights reserved
 *
 ****************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <stdint.h>
#include "jendefs.h"
#include "ZQueue.h"
#include "ZTimer.h"
#include "portmacro.h"
#include "zps_apl_af.h"
#include "mac_vs_sap.h"
#include "AppHardwareApi.h"
#include "dbg.h"
#include "app_main.h"
#include "app_buttons.h"
#include "app_events.h"
#include "app_zcl_light_task.h"

#include "app_manage_temperature.h"
#include "PDM.h"
#include "app_zlo_light_node.h"
#include "app_power_on_counter.h"

#ifdef Light_ColorLight
#include "App_Light_ColorLight.h"
#endif

#ifdef Light_DimmableLight
#include "App_Light_DimmableLight.h"
#endif

#ifdef Light_DimmablePlug
#include "App_Light_DimmablePlug.h"
#endif

#ifdef Light_ExtendedColorLight
#include "App_Light_ExtendedColorLight.h"
#endif

#ifdef Light_ColorTemperatureLight
#include "APP_Light_ColorTemperatureLight.h"
#endif

#ifdef Light_OnOffLight
#include "APP_Light_OnOffLight.h"
#endif

#ifdef Light_OnOffPlug
#include "APP_Light_OnOffPlug.h"
#endif

#ifdef APP_NTAG_ICODE
#include "app_ntag_icode.h"
#endif
#ifdef APP_NTAG_AES
#include "app_ntag_aes.h"
#endif

#include "app_reporting.h"

#if JENNIC_CHIP_FAMILY == JN517x
#define NVIC_INT_PRIO_LEVEL_SYSCTRL (13)
#define NVIC_INT_PRIO_LEVEL_BBC     (7)
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef DEBUG_APP
#define TRACE_APP   FALSE
#else
#define TRACE_APP   TRUE
#endif

#define TIMER_QUEUE_SIZE             8
#define MLME_QUEQUE_SIZE             8
#define MCPS_QUEUE_SIZE             20
#define ZPS_QUEUE_SIZE                  1
#define BDB_QUEUE_SIZE               2
#define APP_QUEUE_SIZE                  8

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/


PUBLIC uint8 u8TimerButtonScan;
PUBLIC uint8 u8TimerRadioRecal;
PUBLIC uint8 u8TimerTick;
PUBLIC uint8 u8TimerPowerOn;
PUBLIC uint8 u8TimerReporting;

#define APP_NUM_STD_TMRS            4

#ifdef CLD_GREENPOWER
    PUBLIC uint8 u8GPTimerTick;
    #define APP_NUM_GP_TMRS             1
    #define GP_TIMER_QUEUE_SIZE         2
#else
    #define APP_NUM_GP_TMRS             0
#endif

#if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
    PUBLIC uint8 u8TimerNtag;
    #define APP_NUM_NTAG_TMRS          1
#else
    #define APP_NUM_NTAG_TMRS          0
#endif

#ifdef OTA_CLD_ATTR_REQUEST_DELAY
    PUBLIC uint8 u8TimerZclMsTick;
    #define APP_NUM_ZCL_MS_TMRS         1
#else
    #define APP_NUM_ZCL_MS_TMRS         0
#endif

#define APP_ZTIMER_STORAGE   (APP_NUM_STD_TMRS + APP_NUM_GP_TMRS + APP_NUM_NTAG_TMRS + APP_NUM_ZCL_MS_TMRS)

PUBLIC tszQueue APP_msgBdbEvents;
PUBLIC tszQueue APP_msgAppEvents;


/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

PRIVATE ZTIMER_tsTimer asTimers[APP_ZTIMER_STORAGE + BDB_ZTIMER_STORAGE];

PRIVATE zps_tsTimeEvent asTimeEvent[TIMER_QUEUE_SIZE];
PRIVATE MAC_tsMcpsVsDcfmInd asMacMcpsDcfmInd[MCPS_QUEUE_SIZE];
PRIVATE MAC_tsMlmeVsDcfmInd asMacMlmeVsDcfmInd[MLME_QUEQUE_SIZE];
PRIVATE BDB_tsZpsAfEvent asBdbEvent[BDB_QUEUE_SIZE];

PRIVATE APP_tsLightEvent asAppEvent[APP_QUEUE_SIZE];
#ifdef CLD_GREENPOWER
PUBLIC tszQueue APP_msgGPZCLTimerEvents;
uint8 au8GPZCLEvent[ GP_TIMER_QUEUE_SIZE];
uint8 u8GPZCLTimerEvent;
#endif
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

extern void zps_taskZPS(void);
extern void PWRM_vManagePower(void);

/****************************************************************************
 *
 * NAME: APP_vMainLoop
 *
 * DESCRIPTION:
 * Main application loop
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vMainLoop(void)
{

    while (TRUE)
    {
        DBG_vPrintf(FALSE, "APP: Entering zps_taskZPS\n");
        zps_taskZPS();
        DBG_vPrintf(FALSE, "APP: Entering bdb_taskBDB\n");
        bdb_taskBDB();
        DBG_vPrintf(FALSE, "APP: Entering ZTIMER_vTask\n");
        ZTIMER_vTask();

        DBG_vPrintf(FALSE, "APP: Entering APP_taskLight\n");
        APP_taskLight();

        /* Re-load the watch-dog timer. Execution must return through the idle
         * task before the CPU is suspended by the power manager. This ensures
         * that at least one task / ISR has executed within the watchdog period
         * otherwise the system will be reset.
         */
        vAHI_WatchdogRestart();

        /*
         * suspends CPU operation when the system is idle or puts the device to
         * sleep if there are no activities in progress
         */
        PWRM_vManagePower();
    }
}


/****************************************************************************
 *
 * NAME: APP_vSetUpHardware
 *
 * DESCRIPTION:
 * Set up interrupts
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vSetUpHardware(void)
{
#if (JENNIC_CHIP_FAMILY == JN517x)

    vAHI_SysCtrlRegisterCallback ( vISR_SystemController );
    u32AHI_Init();
    vAHI_InterruptSetPriority ( MICRO_ISR_MASK_BBC,        NVIC_INT_PRIO_LEVEL_BBC );
    vAHI_InterruptSetPriority ( MICRO_ISR_MASK_SYSCTRL, NVIC_INT_PRIO_LEVEL_SYSCTRL );
#endif


#if (JENNIC_CHIP_FAMILY == JN516x)
    TARGET_INITIALISE();
    /* clear interrupt priority level  */
    SET_IPL(0);
    portENABLE_INTERRUPTS();
#endif
}


/****************************************************************************
 *
 * NAME: APP_vInitResources
 *
 * DESCRIPTION:
 * Initialise resources (timers, queue's etc)
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitResources(void)
{

       DBG_vPrintf(TRACE_APP, "APP: Initialising resources...\n");

    /* Initialise the Z timer module */
    ZTIMER_eInit(asTimers, sizeof(asTimers) / sizeof(ZTIMER_tsTimer));


    /* Create Z timers */
    ZTIMER_eOpen(&u8TimerButtonScan,    APP_cbTimerButtonScan,      NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerRadioRecal,    APP_cbTimerRadioRecal,      NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerTick,          APP_cbTimerZclTick,         NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    ZTIMER_eOpen(&u8TimerPowerOn,       APP_cbTimerPowerCount,      NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    #ifdef CLD_GREENPOWER
        ZTIMER_eOpen(&u8GPTimerTick,    APP_cbTimerGPZclTick,       NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    #endif
    #if (defined APP_NTAG_ICODE) || (defined APP_NTAG_AES)
        ZTIMER_eOpen(&u8TimerNtag,      APP_cbNtagTimer,            NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    #endif
    #ifdef OTA_CLD_ATTR_REQUEST_DELAY
        ZTIMER_eOpen(&u8TimerZclMsTick, APP_cbTimerZclMsTick,       NULL, ZTIMER_FLAG_PREVENT_SLEEP);
    #endif
    ZTIMER_eOpen(&u8TimerReporting,          APP_cbTimerReporting,      NULL, ZTIMER_FLAG_PREVENT_SLEEP);

    /* Create all the queues */
    ZQ_vQueueCreate(&APP_msgBdbEvents,        BDB_QUEUE_SIZE,         sizeof(BDB_tsZpsAfEvent),   (uint8*)asBdbEvent);
    ZQ_vQueueCreate(&APP_msgAppEvents,         APP_QUEUE_SIZE,            sizeof(APP_tsLightEvent),    (uint8*)asAppEvent);
    ZQ_vQueueCreate(&zps_msgMlmeDcfmInd,         MLME_QUEQUE_SIZE,        sizeof(MAC_tsMlmeVsDcfmInd),(uint8*)asMacMlmeVsDcfmInd);
    ZQ_vQueueCreate(&zps_msgMcpsDcfmInd,         MCPS_QUEUE_SIZE,        sizeof(MAC_tsMcpsVsDcfmInd),(uint8*)asMacMcpsDcfmInd);
    ZQ_vQueueCreate(&zps_TimeEvents,            TIMER_QUEUE_SIZE,        sizeof(zps_tsTimeEvent),    (uint8*)asTimeEvent);
#ifdef CLD_GREENPOWER
    ZQ_vQueueCreate(&APP_msgGPZCLTimerEvents,        GP_TIMER_QUEUE_SIZE,         sizeof(uint8),   (uint8*)au8GPZCLEvent);
#endif
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
