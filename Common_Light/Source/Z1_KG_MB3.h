/*****************************************************************************
 *
 * MODULE:              Demo board BULB controls
 *
 * COMPONENT:           $RCSfile: BULBControl.h,v $
 *
 * VERSION:             $Name: JN514x_SDK_V0002_RC0 $
 *
 * REVISION:            $Revision: 1.5 $
 *
 * DATED:               $Date: 2008/10/22 12:28:15 $
 *
 * STATUS:              $State: Exp $
 *
 * AUTHOR:              CJG
 *
 * DESCRIPTION:
 * Macros to make it easier to drive BULBs on demo boards
 *
 * LAST MODIFIED BY:    $Author: pjtw $
 *                      $Modtime: $
 *
 ****************************************************************************
 *
 * This software is owned by Jennic and/or its supplier and is protected
 * under applicable copyright laws. All rights are reserved. We grant You,
 * and any third parties, a license to use this software solely and
 * exclusively on Jennic products. You, and any third parties must reproduce
 * the copyright and warranty notice and any other legend of ownership on
 * each copy or partial copy of the software.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS". JENNIC MAKES NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * ACCURACY OR LACK OF NEGLIGENCE. JENNIC SHALL NOT, IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR ANY DAMAGES, INCLUDING, BUT NOT LIMITED TO, SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER.
 *
 * Copyright Jennic Ltd 2009. All rights reserved
 *
 ***************************************************************************/

#ifndef Z1_KG_MB3_INCLUDED
#define Z1_KG_MB3_INCLUDED

#if defined __cplusplus
extern "C" {
#endif

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "AppHardwareApi.h"
#include "PlatformDIOAssignments.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define HI_HOME

/* bulb */
#define BULB_NUM 4

#define BULB_0_BIT            (8)
#define BULB_1_BIT            (9)
#define BULB_2_BIT            (10)
#define BULB_3_BIT            (11)

#define BULB_0_PIN            (1UL << BULB_0_BIT)
#define BULB_0_VAL            (0)
#define BULB_1_PIN            (1UL << BULB_1_BIT)
#define BULB_1_VAL            (1)
#define BULB_2_PIN            (1UL << BULB_2_BIT)
#define BULB_2_VAL            (2)
#define BULB_3_PIN            (1UL << BULB_3_BIT)
#define BULB_3_VAL            (3)

#define BULB_GENERIC_D1        BULB_0_VAL
#define BULB_GENERIC_D2        BULB_1_VAL
#define BULB_GENERIC_D3        BULB_2_VAL
#define BULB_GENERIC_D4        BULB_3_VAL

#define BULB_CTRL_MASK_FFD     (BULB_0_PIN | BULB_1_PIN | BULB_2_PIN | BULB_3_PIN)

#define LED_NUM 1
#define LED_0_BIT	(14)
#define LED_0_PIN	(1UL << LED_0_BIT)
#define LED_0_VAL            (0)
#define LED_CTRL_MASK_FFD     (LED_0_PIN)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#if defined __cplusplus
}
#endif

#endif  /* Z1_KG_MB3_INCLUDED */

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

