/****************************************************************************
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
 * Copyright NXP B.V. 2016. All rights reserved
 ****************************************************************************/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/* SDK includes */
#include <jendefs.h>
#include "AppHardwareApi.h"
#include "dbg.h"
#include "dbg_uart.h"

/* Ó²¼þÆ½Ì¨*/
#include "Z1_KG_MB3.h"

/* Device includes */
#include "DriverBulb.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
//#define DEBUG_DRIVER
#ifndef DEBUG_DRIVER
#define TRACE_DRIVER FALSE
#else
#define TRACE_DRIVER TRUE
#endif


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Global Variables                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE bool_t bBulbOn[] = {FALSE,FALSE,FALSE,FALSE};
PRIVATE uint8_t u8BulbNo = BULB_0_VAL;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void DriverBulb_vInit(void)
{
	static bool_t bFirstCalled = TRUE;

	if (bFirstCalled)
	{
		vAHI_DioSetDirection(0, BULB_CTRL_MASK_FFD);
		bFirstCalled = FALSE;
	}
}

PUBLIC void DriverBulb_bSetNo(uint8_t bulb)
{
	u8BulbNo = bulb;
}

PUBLIC void DriverBulb_vOn(void)
{
	DriverBulb_vSetOnOff(TRUE);
}

PUBLIC void DriverBulb_vOff(void)
{
	DriverBulb_vSetOnOff(FALSE);
}

PUBLIC void DriverBulb_vSetOnOff(bool_t bOn)
{
    uint32 u32PinHigh = 0;
    uint32 u32PinLow  = 0;
    switch (u8BulbNo) {
        /* Generic shield LED's are active-high */
        case BULB_0_VAL:
            u32PinHigh = BULB_0_PIN;
            break;
        case BULB_1_VAL:
            u32PinHigh = BULB_1_PIN;
            break;
        case BULB_2_VAL:
            u32PinHigh = BULB_2_PIN;
            break;
        case BULB_3_VAL:
			u32PinHigh = BULB_3_PIN;
			break;
        default:
            break;
    }
    if (bOn){
       vAHI_DioSetOutput(u32PinHigh, u32PinLow);
    } else {
       vAHI_DioSetOutput(u32PinLow, u32PinHigh);
    }
     bBulbOn[u8BulbNo] =  bOn;
     DBG_vPrintf(TRACE_DRIVER, "\nS:%s",(bOn ? "ON" : "OFF"));
}

PUBLIC void DriverBulb_vSetLevel(uint32 u32Level)
{

	if (bBulbOn[u8BulbNo])
	{

	}

	DBG_vPrintf(TRACE_DRIVER, "\nL%d",u32Level);
}

PUBLIC void DriverBulb_vSetColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue)
{

}

PUBLIC bool_t DriverBulb_bOn(void)
{
	return (bBulbOn[u8BulbNo]);
}

PUBLIC bool_t DriverBulb_bReady(void)
{
	return (TRUE);
}

PUBLIC bool_t DriverBulb_bFailed(void)
{
	return (FALSE);
}

PUBLIC void DriverBulb_vTick(void)
{
/* No timing behaviour needed in DR1175 */
}


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
