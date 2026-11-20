/*
** $Id: //Department/DaVinci/BRANCHES/MT6620_WIFI_DRIVER_V2_3/mgmt/saa_fsm.c#2 $
*/

/*! \file   "saa_fsm.c"
    \brief  This file defines the FSM for SAA MODULE.

    This file defines the FSM for SAA MODULE.
*/



/*
** $Log: saa_fsm.c $
**
** 09 04 2013 cp.wu
** fix typo
**
** 09 03 2013 cp.wu
** add path for reassociation
 *
 * 07 17 2012 yuche.tsai
 * NULL
 * Compile no error before trial run.
 *
 * 04 20 2012 cp.wu
 * [WCXRP00000913] [MT6620 Wi-Fi] create repository of source code dedicated for MT6620 E6 ASIC
 * correct macro
 *
 * 11 24 2011 wh.su
 * [WCXRP00001078] [MT6620 Wi-Fi][Driver] Adding the mediatek log improment support : XLOG
 * Adjust code for DBG and CONFIG_XLOG.
 *
 * 11 11 2011 wh.su
 * [WCXRP00001078] [MT6620 Wi-Fi][Driver] Adding the mediatek log improment support : XLOG
 * modify the xlog related code.
 *
 * 11 04 2011 cp.wu
 * [WCXRP00001086] [MT6620 Wi-Fi][Driver] On Android, indicate an extra DISCONNECT for REASSOCIATED cases as an explicit trigger for Android framework
 * 1. for DEAUTH/DISASSOC cases, indicate for DISCONNECTION immediately.
 * 2. (Android only) when reassociation-and-non-roaming cases happened, indicate an extra DISCONNECT indication to Android Wi-Fi framework
 *
 * 11 02 2011 wh.su
 * [WCXRP00001078] [MT6620 Wi-Fi][Driver] Adding the mediatek log improment support : XLOG
 * adding the code for XLOG.
 *
 * 09 30 2011 cm.chang
 * [WCXRP00001020] [MT6620 Wi-Fi][Driver] Handle secondary channel offset of AP in 5GHz band
 * Add debug message about 40MHz bandwidth allowed
 *
 * 05 12 2011 cp.wu
 * [WCXRP00000720] [MT6620 Wi-Fi][Driver] Do not do any further operation in case STA-REC has been invalidated before SAA-FSM starts to roll
 * check for valid STA-REC before SAA-FSM starts to roll.
 *
 * 04 21 2011 terry.wu
 * [WCXRP00000674] [MT6620 Wi-Fi][Driver] Refine AAA authSendAuthFrame
 * Add network type parameter to authSendAuthFrame.
 *
 * 04 15 2011 chinghwa.yu
 * [WCXRP00000065] Update BoW design and settings
 * Add BOW short range mode.
 *
 * 04 14 2011 cm.chang
 * [WCXRP00000634] [MT6620 Wi-Fi][Driver][FW] 2nd BSS will not support 40MHz bandwidth for concurrency
 * .
 *
 * 03 31 2011 puff.wen
 * NULL
 * .
 *
 * 02 10 2011 yuche.tsai
 * [WCXRP00000431] [Volunteer Patch][MT6620][Driver] Add MLME support for deauthentication under AP(Hot-Spot) mode.
 * Add RX deauthentication & disassociation process under Hot-Spot mode.
 *
 * 01 26 2011 yuche.tsai
 * [WCXRP00000388] [Volunteer Patch][MT6620][Driver/Fw] change Station Type in station record.
 * .
 *
 * 01 25 2011 yuche.tsai
 * [WCXRP00000388] [Volunteer Patch][MT6620][Driver/Fw] change Station Type in station record.
 * Fix compile error of after Station Type Macro modification.
 *
 * 01 25 2011 yuche.tsai
 * [WCXRP00000388] [Volunteer Patch][MT6620][Driver/Fw] change Station Type in station record.
 * Change Station Type in Station Record, Modify MACRO definition for getting station type & network type index & Role.
 *
 * 11 29 2010 cp.wu
 * [WCXRP00000210] [MT6620 Wi-Fi][Driver][FW] Set RCPI value in STA_REC for initial TX rate selection of auto-rate algorithm
 * update ucRcpi of STA_RECORD_T for AIS when
 * 1) Beacons for IBSS merge is received
 * 2) Associate Response for a connecting peer is received
 *
 * 10 18 2010 cp.wu
 * [WCXRP00000053] [MT6620 Wi-Fi][Driver] Reset incomplete and might leads to BSOD when entering RF test with AIS associated
 * 1. remove redundant variables in STA_REC structure
 * 2. add STA-REC uninitialization routine for clearing pending events
 *
 * 09 03 2010 kevin.huang
 * NULL
 * Refine #include sequence and solve recursive/nested #include issue
 *
 * 08 30 2010 cp.wu
 * NULL
 * eliminate klockwork errors
 *
 * 08 24 2010 chinghwa.yu
 * NULL
 * Update for MID_SCN_BOW_SCAN_DONE mboxDummy.
 * Update saa_fsm for BOW.
 *
 * 08 16 2010 cp.wu
 * NULL
 * Replace CFG_SUPPORT_BOW by CFG_ENABLE_BT_OVER_WIFI.
 * There is no CFG_SUPPORT_BOW in driver domain source.
 *
 * 08 02 2010 yuche.tsai
 * NULL
 * Add support for P2P join event start.
 *
 * 07 12 2010 cp.wu
 *
 * SAA will take a record for tracking request sequence number.
 *
 * 07 08 2010 cp.wu
 *
 * [WPD00003833] [MT6620 and MT5931] Driver migration - move to new repository.
 *
 * 07 01 2010 cp.wu
 * [WPD00003833][MT6620 and MT5931] Driver migration
 * AIS-FSM integration with CNM channel request messages
 *
 * 06 23 2010 cp.wu
 * [WPD00003833][MT6620 and MT5931] Driver migration
 * sync. with main branch for reseting to state 1 when associating with another AP
 *
 * 06 21 2010 cp.wu
 * [WPD00003833][MT6620 and MT5931] Driver migration
 * refine TX-DONE callback.
 *
 * 06 21 2010 wh.su
 * [WPD00003840][MT6620 5931] Security migration
 * remove duplicate variable for migration.
 *
 * 06 18 2010 cm.chang
 * [WPD00003841][LITE Driver] Migrate RLM/CNM to host driver
 * Provide cnmMgtPktAlloc() and alloc/free function of msg/buf
 *
 * 06 18 2010 wh.su
 * [WPD00003840][MT6620 5931] Security migration
 * migration the security related function from firmware.
 *
 * 06 17 2010 yuche.tsai
 * [WPD00003839][MT6620 5931][P2P] Feature migration
 * Fix compile error when enable WiFi Direct function.
 *
 * 06 14 2010 cp.wu
 * [WPD00003833][MT6620 and MT5931] Driver migration
 * saa_fsm.c is migrated.
 *
 * 05 12 2010 kevin.huang
 * [BORA00000794][WIFISYS][New Feature]Power Management Support
 * Add Power Management - Legacy PS-POLL support.
 *
 * 04 24 2010 cm.chang
 * [BORA00000018]Integrate WIFI part into BORA for the 1st time
 * g_aprBssInfo[] depends on CFG_SUPPORT_P2P and CFG_SUPPORT_BOW
 *
 * 04 19 2010 kevin.huang
 * [BORA00000714][WIFISYS][New Feature]Beacon Timeout Support
 *
 *  *  * Add Connection Policy - Any and Rx Burst Deauth Support for WHQL
 *
 * 03 10 2010 kevin.huang
 * [BORA00000654][WIFISYS][New Feature] CNM Module - Ch Manager Support
 * Add Channel Manager for arbitration of JOIN and SCAN Req
 *
 * 02 26 2010 kevin.huang
 * [BORA00000603][WIFISYS] [New Feature] AAA Module Support
 * Add support of Driver STA_RECORD_T activation
 *
 * 02 04 2010 kevin.huang
 * [BORA00000603][WIFISYS] [New Feature] AAA Module Support
 * Add AAA Module Support, Revise Net Type to Net Type Index for array lookup
 *
 * 01 27 2010 wh.su
 * [BORA00000476][Wi-Fi][firmware] Add the security module initialize code
 * add and fixed some security function.
 *
 * 01 12 2010 kevin.huang
 * [BORA00000018]Integrate WIFI part into BORA for the 1st time
 * Fix compile warning due to declared but not used
 *
 * 01 11 2010 kevin.huang
 * [BORA00000018]Integrate WIFI part into BORA for the 1st time
 * Add Deauth and Disassoc Handler
 *
 * 01 08 2010 kevin.huang
 * [BORA00000018]Integrate WIFI part into BORA for the 1st time
 * Refine Debug Label
 *
 * 12 18 2009 cm.chang
 * [BORA00000018]Integrate WIFI part into BORA for the 1st time
 * .
 *
 * Dec 3 2009 mtk01461
 * [BORA00000018] Integrate WIFI part into BORA for the 1st time
 * Update comment
 *
 * Dec 1 2009 mtk01088
 * [BORA00000476] [Wi-Fi][firmware] Add the security module initialize code
 * rename the function
 *
 * Nov 24 2009 mtk01461
 * [BORA00000018] Integrate WIFI part into BORA for the 1st time
 * Revise MGMT Handler with Retain Status
 *
 * Nov 23 2009 mtk01461
 * [BORA00000018] Integrate WIFI part into BORA for the 1st time
 *
*/

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/
#include "precomp.h"

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/

/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/

/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/

/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/
#if DBG
/*lint -save -e64 Type mismatch */
static PUINT_8 apucDebugAAState[AA_STATE_NUM] = {
    (PUINT_8)DISP_STRING("AA_STATE_IDLE"),
    (PUINT_8)DISP_STRING("SAA_STATE_SEND_AUTH1"),
    (PUINT_8)DISP_STRING("SAA_STATE_WAIT_AUTH2"),
    (PUINT_8)DISP_STRING("SAA_STATE_SEND_AUTH3"),
    (PUINT_8)DISP_STRING("SAA_STATE_WAIT_AUTH4"),
    (PUINT_8)DISP_STRING("SAA_STATE_SEND_ASSOC1"),
    (PUINT_8)DISP_STRING("SAA_STATE_WAIT_ASSOC2"),
    (PUINT_8)DISP_STRING("AAA_STATE_SEND_AUTH2"),
    (PUINT_8)DISP_STRING("AAA_STATE_SEND_AUTH4"),
    (PUINT_8)DISP_STRING("AAA_STATE_SEND_ASSOC2"),
    (PUINT_8)DISP_STRING("AA_STATE_RESOURCE")
};
/*lint -restore */
#endif /* DBG */

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/

/*******************************************************************************
*                   F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/

/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/
/*----------------------------------------------------------------------------*/
/*!
* @brief The Core FSM engine of SAA Module.
*
* @param[in] prStaRec           Pointer to the STA_RECORD_T
* @param[in] eNextState         The value of Next State
* @param[in] prRetainedSwRfb    Pointer to the retained SW_RFB_T for JOIN Success
*
* @return (none)
*/
/*----------------------------------------------------------------------------*/
VOID
saaFsmSteps (
    IN P_ADAPTER_T prAdapter,
    IN P_STA_RECORD_T prStaRec,
    IN ENUM_AA_STATE_T eNextState,
    IN P_SW_RFB_T prRetainedSwRfb
    )
{
    ENUM_AA_STATE_T ePreviousState;
    BOOLEAN fgIsTransition;


    ASSERT(prStaRec);
    if(!prStaRec) {
        return;
    }

    do {

#if DBG
        DBGLOG(SAA, STATE, ("TRANSITION: [%s] -> [%s]\n",
                            apucDebugAAState[prStaRec->eAuthAssocState],
                            apucDebugAAState[eNextState]));
#else
        DBGLOG(SAA, STATE, ("[%d] TRANSITION: [%d] -> [%d]\n",
                            DBG_SAA_IDX,
                            prStaRec->eAuthAssocState,
                            eNextState));
#endif
        ePreviousState = prStaRec->eAuthAssocState;

        /* NOTE(Kevin): This is the only place to change the eAuthAssocState(except initial) */
        prStaRec->eAuthAssocState = eNextState;


        fgIsTransition = (BOOLEAN)FALSE;
        switch (prStaRec->eAuthAssocState) {
        case AA_STATE_IDLE:
            {
                if (ePreviousState != prStaRec->eAuthAssocState) { /* Only trigger this event once */

                    if (prRetainedSwRfb) {

                        if (saaFsmSendEventJoinComplete(prAdapter,
                                    WLAN_STATUS_SUCCESS,
                                    prStaRec,
                                    prRetainedSwRfb) == WLAN_STATUS_SUCCESS) {
                        }
                        else {
                            eNextState = AA_STATE_RESOURCE;
                            fgIsTransition = TRUE;
                        }
                    }
                    else {
                        if (saaFsmSendEventJoinComplete(prAdapter,
                                    WLAN_STATUS_FAILURE,
                                    prStaRec,
                                    NULL) == WLAN_STATUS_RESOURCES) {
                            eNextState = AA_STATE_RESOURCE;
                            fgIsTransition = TRUE;
                        }
                    }

                }

                /* Free allocated TCM memory */
                if (prStaRec->prChallengeText) {
                    cnmMemFree(prAdapter, prStaRec->prChallengeText);
                    prStaRec->prChallengeText = (P_IE_CHALLENGE_TEXT_T)NULL;
                }
            }
            break;

        case SAA_STATE_SEND_AUTH1:
            {
                /* Do tasks in INIT STATE */
                if (prStaRec->ucTxAuthAssocRetryCount >=
                    prStaRec->ucTxAuthAssocRetryLimit) {

                    /* Record the Status Code of Authentication Request */
                    prStaRec->u2StatusCode = STATUS_CODE_AUTH_TIMEOUT;

                    eNextState = AA_STATE_IDLE;
                    fgIsTransition = TRUE;
                }
                else {
                    prStaRec->ucTxAuthAssocRetryCount++;

                    /* Update Station Record - Class 1 Flag */
                    cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_1);

#if !CFG_SUPPORT_AAA
                    if (authSendAuthFrame(prAdapter,
                                prStaRec,
                                AUTH_TRANSACTION_SEQ_1) != WLAN_STATUS_SUCCESS)
#else
                    if (authSendAuthFrame(
                                        prAdapter,
                                        prStaRec,
                                        prStaRec->ucNetTypeIndex,
                                        NULL,
                                        AUTH_TRANSACTION_SEQ_1,
                                        STATUS_CODE_RESERVED) != WLAN_STATUS_SUCCESS)
#endif /* CFG_SUPPORT_AAA */
                    {

                        cnmTimerInitTimer(prAdapter,
                                &prStaRec->rTxReqDoneOrRxRespTimer,
                                (PFN_MGMT_TIMEOUT_FUNC)saaFsmRunEventTxReqTimeOut,
                                (UINT_32)prStaRec);

                        cnmTimerStartTimer(prAdapter,
                                &prStaRec->rTxReqDoneOrRxRespTimer,
                                TU_TO_MSEC(TX_AUTHENTICATION_RETRY_TIMEOUT_TU));
                    }
                }
            }
            break;

        case SAA_STATE_WAIT_AUTH2:
            break;

        case SAA_STATE_SEND_AUTH3:
            {
                /* Do tasks in INIT STATE */
                if (prStaRec->ucTxAuthAssocRetryCount >=
                    prStaRec->ucTxAuthAssocRetryLimit) {

                    /* Record the Status Code of Authentication Request */
                    prStaRec->u2StatusCode = STATUS_CODE_AUTH_TIMEOUT;

                    eNextState = AA_STATE_IDLE;
                    fgIsTransition = TRUE;
                }
                else {
                    prStaRec->ucTxAuthAssocRetryCount++;

#if !CFG_SUPPORT_AAA
                    if (authSendAuthFrame(prAdapter,
                                prStaRec,
                                AUTH_TRANSACTION_SEQ_3) != WLAN_STATUS_SUCCESS)
#else
                    if (authSendAuthFrame(prAdapter,
                                prStaRec,
                                prStaRec->ucNetTypeIndex,
                                NULL,
                                AUTH_TRANSACTION_SEQ_3,
                                STATUS_CODE_RESERVED) != WLAN_STATUS_SUCCESS)
#endif /* CFG_SUPPORT_AAA */
                    {

                        cnmTimerInitTimer(prAdapter,
                                &prStaRec->rTxReqDoneOrRxRespTimer,
                                (PFN_MGMT_TIMEOUT_FUNC)saaFsmRunEventTxReqTimeOut,
                                (UINT_32)prStaRec);

                        cnmTimerStartTimer(prAdapter,
                                &prStaRec->rTxReqDoneOrRxRespTimer,
                                TU_TO_MSEC(TX_AUTHENTICATION_RETRY_TIMEOUT_TU));
                    }
                }
            }
            break;

        case SAA_STATE_WAIT_AUTH4:
            break;

        case SAA_STATE_SEND_ASSOC1:
                /* Do tasks in INIT STATE */
                if (prStaRec->ucTxAuthAssocRetryCount >=
                    prStaRec->ucTxAuthAssocRetryLimit) {

                    /* Record the Status Code of Authentication Request */
                    prStaRec->u2StatusCode = STATUS_CODE_ASSOC_TIMEOUT;

                    eNextState = AA_STATE_IDLE;
                    fgIsTransition = TRUE;
                }
                else {
                    prStaRec->ucTxAuthAssocRetryCount++;

                    if (assocSendReAssocReqFrame(prAdapter, prStaRec) != WLAN_STATUS_SUCCESS) {

                        cnmTimerInitTimer(prAdapter,
                                &prStaRec->rTxReqDoneOrRxRespTimer,
                                (PFN_MGMT_TIMEOUT_FUNC)saaFsmRunEventTxReqTimeOut,
                                (UINT_32)prStaRec);

                        cnmTimerStartTimer(prAdapter,
                                &prStaRec->rTxReqDoneOrRxRespTimer,
                                TU_TO_MSEC(TX_ASSOCIATION_RETRY_TIMEOUT_TU));
                    }
                }

            break;

        case SAA_STATE_WAIT_ASSOC2:
            break;

        case AA_STATE_RESOURCE:
            /* TODO(Kevin) Can setup a timer and send message later */
            break;

        default:
            DBGLOG(SAA, ERROR, ("Unknown AA STATE\n"));
            ASSERT(0);
            break;
        }

    }
    while (fgIsTransition);

    return;

} /* end of saaFsmSteps() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will send Event to AIS/BOW/P2P
*
* @param[in] rJoinStatus        To indicate JOIN success or failure.
* @param[in] prStaRec           Pointer to the STA_RECORD_T
* @param[in] prSwRfb            Pointer to the SW_RFB_T

* @return (none)
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS
saaFsmSendEventJoinComplete (
    IN P_ADAPTER_T prAdapter,
    IN WLAN_STATUS rJoinStatus,
    IN P_STA_RECORD_T prStaRec,
    IN P_SW_RFB_T prSwRfb
    )
{
    P_BSS_INFO_T prBssInfo;

    ASSERT(prStaRec);
    if(!prStaRec) {
        return WLAN_STATUS_INVALID_PACKET;
    }

    /* Store limitation about 40Mhz bandwidth capability during association */
    if (prStaRec->ucNetTypeIndex < NETWORK_TYPE_INDEX_NUM) {
        prBssInfo = &prAdapter->rWifiVar.arBssInfo[prStaRec->ucNetTypeIndex];

        if (rJoinStatus == WLAN_STATUS_SUCCESS) {
            prBssInfo->fg40mBwAllowed = prBssInfo->fgAssoc40mBwAllowed;
        }
        prBssInfo->fgAssoc40mBwAllowed = FALSE;
    }

    if(prStaRec->ucNetTypeIndex == NETWORK_TYPE_AIS_INDEX) {
        P_MSG_SAA_FSM_COMP_T prSaaFsmCompMsg;

        prSaaFsmCompMsg = cnmMemAlloc(prAdapter, RAM_TYPE_MSG, sizeof(MSG_SAA_FSM_COMP_T));
        if (!prSaaFsmCompMsg) {
            return WLAN_STATUS_RESOURCES;
        }

        prSaaFsmCompMsg->rMsgHdr.eMsgId = MID_SAA_AIS_JOIN_COMPLETE;
        prSaaFsmCompMsg->ucSeqNum = prStaRec->ucAuthAssocReqSeqNum;
        prSaaFsmCompMsg->rJoinStatus = rJoinStatus;
        prSaaFsmCompMsg->prStaRec = prStaRec;
        prSaaFsmCompMsg->prSwRfb = prSwRfb;

        /* NOTE(Kevin): Set to UNBUF for immediately JOIN complete */
        mboxSendMsg(prAdapter,
                MBOX_ID_0,
                (P_MSG_HDR_T)prSaaFsmCompMsg,
                MSG_SEND_METHOD_UNBUF);

        return WLAN_STATUS_SUCCESS;
    }
#if CFG_ENABLE_WIFI_DIRECT
    else if ((prAdapter->fgIsP2PRegistered) &&
        (IS_STA_IN_P2P(prStaRec))) {
        P_MSG_SAA_FSM_COMP_T prSaaFsmCompMsg;

        prSaaFsmCompMsg = cnmMemAlloc(prAdapter, RAM_TYPE_MSG, sizeof(MSG_SAA_FSM_COMP_T));
        if (!prSaaFsmCompMsg) {
            return WLAN_STATUS_RESOURCES;
        }

        prSaaFsmCompMsg->rMsgHdr.eMsgId = MID_SAA_P2P_JOIN_COMPLETE;
        prSaaFsmCompMsg->ucSeqNum = prStaRec->ucAuthAssocReqSeqNum;
        prSaaFsmCompMsg->rJoinStatus = rJoinStatus;
        prSaaFsmCompMsg->prStaRec = prStaRec;
        prSaaFsmCompMsg->prSwRfb = prSwRfb;

        /* NOTE(Kevin): Set to UNBUF for immediately JOIN complete */
        mboxSendMsg(prAdapter,
                MBOX_ID_0,
                (P_MSG_HDR_T)prSaaFsmCompMsg,
                MSG_SEND_METHOD_UNBUF);

        return WLAN_STATUS_SUCCESS;
    }
#endif
#if CFG_ENABLE_BT_OVER_WIFI
    else if(prStaRec->ucNetTypeIndex == NETWORK_TYPE_BOW_INDEX) {
        //@TODO: BOW handler

        P_MSG_SAA_FSM_COMP_T prSaaFsmCompMsg;

        prSaaFsmCompMsg = cnmMemAlloc(prAdapter, RAM_TYPE_MSG, sizeof(MSG_SAA_FSM_COMP_T));
        if (!prSaaFsmCompMsg) {
            return WLAN_STATUS_RESOURCES;
        }

        prSaaFsmCompMsg->rMsgHdr.eMsgId = MID_SAA_BOW_JOIN_COMPLETE;
        prSaaFsmCompMsg->ucSeqNum = prStaRec->ucAuthAssocReqSeqNum;
        prSaaFsmCompMsg->rJoinStatus = rJoinStatus;
        prSaaFsmCompMsg->prStaRec = prStaRec;
        prSaaFsmCompMsg->prSwRfb = prSwRfb;

        /* NOTE(Kevin): Set to UNBUF for immediately JOIN complete */
        mboxSendMsg(prAdapter,
                MBOX_ID_0,
                (P_MSG_HDR_T)prSaaFsmCompMsg,
                MSG_SEND_METHOD_UNBUF);

        return WLAN_STATUS_SUCCESS;
    }
#endif
    else {
        ASSERT(0);
        return WLAN_STATUS_FAILURE;
    }

} /* end of saaFsmSendEventJoinComplete() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will handle the Start Event to SAA FSM.
*
* @param[in] prMsgHdr   Message of Join Request for a particular STA.
*
* @return (none)
*/
/*----------------------------------------------------------------------------*/
VOID
saaFsmRunEventStart (
    IN P_ADAPTER_T prAdapter,
    IN P_MSG_HDR_T prMsgHdr
    )
{
    P_MSG_SAA_FSM_START_T prSaaFsmStartMsg;
    P_STA_RECORD_T prStaRec;
    P_BSS_INFO_T prBssInfo;

    ASSERT(prAdapter);
    ASSERT(prMsgHdr);

    prSaaFsmStartMsg = (P_MSG_SAA_FSM_START_T)prMsgHdr;
    prStaRec = prSaaFsmStartMsg->prStaRec;

    if((!prStaRec) || (prStaRec->fgIsInUse == FALSE)) {
        cnmMemFree(prAdapter, prMsgHdr);
        return;
    }

    ASSERT(prStaRec);

    DBGLOG(SAA, LOUD, ("EVENT-START: Trigger SAA FSM.\n"));

    /* record sequence number of request message */
    prStaRec->ucAuthAssocReqSeqNum = prSaaFsmStartMsg->ucSeqNum;

    cnmMemFree(prAdapter, prMsgHdr);

    //4 <1> Validation of SAA Start Event
    if (!IS_AP_STA(prStaRec)) {

        DBGLOG(SAA, ERROR, ("EVENT-START: STA Type - %d was not supported.\n", prStaRec->eStaType));

        /* Ignore the return value because don't care the prSwRfb */
        saaFsmSendEventJoinComplete(prAdapter, WLAN_STATUS_FAILURE, prStaRec, NULL);

        return;
    }

    //4 <2> The previous JOIN process is not completed ?
    if (prStaRec->eAuthAssocState != AA_STATE_IDLE) {
        DBGLOG(SAA, ERROR, ("EVENT-START: Reentry of SAA Module.\n"));
        prStaRec->eAuthAssocState = AA_STATE_IDLE;
    }

    //4 <3> Reset Status Code and Time
    /* Update Station Record - Status/Reason Code */
    prStaRec->u2StatusCode = STATUS_CODE_SUCCESSFUL;

    /* Update the record join time. */
    GET_CURRENT_SYSTIME(&prStaRec->rLastJoinTime);

    prStaRec->ucTxAuthAssocRetryCount = 0;

    if (prStaRec->prChallengeText) {
        cnmMemFree(prAdapter, prStaRec->prChallengeText);
        prStaRec->prChallengeText = (P_IE_CHALLENGE_TEXT_T)NULL;
    }

    cnmTimerStopTimer(prAdapter, &prStaRec->rTxReqDoneOrRxRespTimer);

#if CFG_PRIVACY_MIGRATION
    //4 <4> Init the sec fsm
    secFsmInit(prAdapter, prStaRec);
#endif

    //4 <5> Reset the STA STATE
    /* Update Station Record - Class 1 Flag */
    /* NOTE(Kevin): Moved to AIS FSM for Reconnect issue -
     * We won't deactivate the same STA_RECORD_T and then activate it again for the
     * case of reconnection.
     */
    //cnmStaRecChangeState(prStaRec, STA_STATE_1);

    //4 <6> Decide if this BSS 20/40M bandwidth is allowed
    if (prStaRec->ucNetTypeIndex < NETWORK_TYPE_INDEX_NUM) {
        prBssInfo = &prAdapter->rWifiVar.arBssInfo[prStaRec->ucNetTypeIndex];

        if ((prAdapter->rWifiVar.ucAvailablePhyTypeSet & PHY_TYPE_SET_802_11N)
            && (prStaRec->ucPhyTypeSet & PHY_TYPE_SET_802_11N)) {
            prBssInfo->fgAssoc40mBwAllowed =
                cnmBss40mBwPermitted(prAdapter, prBssInfo->ucNetTypeIndex);
        }
        else {
            prBssInfo->fgAssoc40mBwAllowed = FALSE;
        }
        DBGLOG(RLM, INFO, ("STA 40mAllowed=%d\n", prBssInfo->fgAssoc40mBwAllowed));
    }

    //4 <7> Trigger SAA FSM
    if(prStaRec->ucStaState == STA_STATE_1) {
    	saaFsmSteps(prAdapter, prStaRec, SAA_STATE_SEND_AUTH1, (P_SW_RFB_T)NULL);
    }
    else if(prStaRec->ucStaState == STA_STATE_2 || prStaRec->ucStaState == STA_STATE_3) {
        saaFsmSteps(prAdapter, prStaRec, SAA_STATE_SEND_ASSOC1, (P_SW_RFB_T)NULL);
    }

    return;
} /* end of saaFsmRunEventStart() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will handle TxDone(Auth1/Auth3/AssocReq) Event of SAA FSM.
*
* @param[in] prMsduInfo     Pointer to the MSDU_INFO_T.
* @param[in] rTxDoneStatus  Return TX status of the Auth1/Auth3/AssocReq frame.
*
* @retval WLAN_STATUS_SUCCESS
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS
saaFsmRunEventTxDone (
    IN P_ADAPTER_T              prAdapter,
    IN P_MSDU_INFO_T            prMsduInfo,
    IN ENUM_TX_RESULT_CODE_T    rTxDoneStatus
    )
{

    P_STA_RECORD_T prStaRec;
    ENUM_AA_STATE_T eNextState;


    ASSERT(prMsduInfo);

    prStaRec = cnmGetStaRecByIndex(prAdapter, prMsduInfo->ucStaRecIndex);

    if(!prStaRec) {
        return WLAN_STATUS_INVALID_PACKET;
    }

    ASSERT(prStaRec);

    DBGLOG(SAA, LOUD, ("EVENT-TX DONE: Current Time = %ld\n", kalGetTimeTick()));

    eNextState = prStaRec->eAuthAssocState;

    switch (prStaRec->eAuthAssocState) {
    case SAA_STATE_SEND_AUTH1:
        {
            /* Strictly check the outgoing frame is matched with current AA STATE */
            if (authCheckTxAuthFrame(prAdapter,
                        prMsduInfo,
                        AUTH_TRANSACTION_SEQ_1) != WLAN_STATUS_SUCCESS) {
                break;
            }

            if (rTxDoneStatus == TX_RESULT_SUCCESS) {
                eNextState = SAA_STATE_WAIT_AUTH2;

                cnmTimerStopTimer(prAdapter,
                        &prStaRec->rTxReqDoneOrRxRespTimer);

                cnmTimerInitTimer(prAdapter,
                        &prStaRec->rTxReqDoneOrRxRespTimer,
                        (PFN_MGMT_TIMEOUT_FUNC)saaFsmRunEventRxRespTimeOut,
                        (UINT_32)prStaRec);

                cnmTimerStartTimer(prAdapter,
                        &prStaRec->rTxReqDoneOrRxRespTimer,
                        TU_TO_MSEC(DOT11_AUTHENTICATION_RESPONSE_TIMEOUT_TU));
            }

            /* if TX was successful, change to next state.
             * if TX was failed, do retry if possible.
             */
            saaFsmSteps(prAdapter, prStaRec, eNextState, (P_SW_RFB_T)NULL);
        }
        break;

    case SAA_STATE_SEND_AUTH3:
        {
            /* Strictly check the outgoing frame is matched with current JOIN STATE */
            if (authCheckTxAuthFrame(prAdapter,
                        prMsduInfo,
                        AUTH_TRANSACTION_SEQ_3) != WLAN_STATUS_SUCCESS) {
                break;
            }

            if (rTxDoneStatus == TX_RESULT_SUCCESS) {
                eNextState = SAA_STATE_WAIT_AUTH4;

                cnmTimerStopTimer(prAdapter,
                        &prStaRec->rTxReqDoneOrRxRespTimer);

                cnmTimerInitTimer(prAdapter,
                        &prStaRec->rTxReqDoneOrRxRespTimer,
                        (PFN_MGMT_TIMEOUT_FUNC)saaFsmRunEventRxRespTimeOut,
                        (UINT_32)prStaRec);

                cnmTimerStartTimer(prAdapter,
                        &prStaRec->rTxReqDoneOrRxRespTimer,
                        TU_TO_MSEC(DOT11_AUTHENTICATION_RESPONSE_TIMEOUT_TU));
            }

            /* if TX was successful, change to next state.
             * if TX was failed, do retry if possible.
             */
            saaFsmSteps(prAdapter, prStaRec, eNextState, (P_SW_RFB_T)NULL);
        }
        break;

    case SAA_STATE_SEND_ASSOC1:
        {
            /* Strictly check the outgoing frame is matched with current SAA STATE */
            if (assocCheckTxReAssocReqFrame(prAdapter, prMsduInfo) != WLAN_STATUS_SUCCESS) {
                break;
            }

            if (rTxDoneStatus == TX_RESULT_SUCCESS) {
                eNextState = SAA_STATE_WAIT_ASSOC2;

                cnmTimerStopTimer(prAdapter, &prStaRec->rTxReqDoneOrRxRespTimer);

                cnmTimerInitTimer(prAdapter,
                        &prStaRec->rTxReqDoneOrRxRespTimer,
                        (PFN_MGMT_TIMEOUT_FUNC)saaFsmRunEventRxRespTimeOut,
                        (UINT_32)prStaRec);

                cnmTimerStartTimer(prAdapter,
                        &(prStaRec->rTxReqDoneOrRxRespTimer),
                        TU_TO_MSEC(DOT11_ASSOCIATION_RESPONSE_TIMEOUT_TU));
            }

            /* if TX was successful, change to next state.
             * if TX was failed, do retry if possible.
             */
            saaFsmSteps(prAdapter, prStaRec, eNextState, (P_SW_RFB_T)NULL);
        }
        break;

    default:
        break; /* Ignore other cases */
    }


    return WLAN_STATUS_SUCCESS;

} /* end of saaFsmRunEventTxDone() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will send Tx Request Timeout Event to SAA FSM.
*
* @param[in] prStaRec           Pointer to the STA_RECORD_T
*
* @return (none)
*/
/*----------------------------------------------------------------------------*/
VOID
saaFsmRunEventTxReqTimeOut (
    IN P_ADAPTER_T prAdapter,
    IN P_STA_RECORD_T prStaRec
    )
{
    ASSERT(prStaRec);
    if(!prStaRec) {
        return;
    }

    DBGLOG(SAA, LOUD, ("EVENT-TIMER: TX REQ TIMEOUT, Current Time = %ld\n", kalGetTimeTick()));

    switch (prStaRec->eAuthAssocState) {
    case SAA_STATE_SEND_AUTH1:
    case SAA_STATE_SEND_AUTH3:
    case SAA_STATE_SEND_ASSOC1:
        saaFsmSteps(prAdapter, prStaRec, prStaRec->eAuthAssocState, (P_SW_RFB_T)NULL);
        break;

    default:
        return;
    }

    return;
} /* end of saaFsmRunEventTxReqTimeOut() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will send Rx Response Timeout Event to SAA FSM.
*
* @param[in] prStaRec           Pointer to the STA_RECORD_T
*
* @return (none)
*/
/*----------------------------------------------------------------------------*/
VOID
saaFsmRunEventRxRespTimeOut (
    IN P_ADAPTER_T prAdapter,
    IN P_STA_RECORD_T prStaRec
    )
{
    ENUM_AA_STATE_T eNextState;


    DBGLOG(SAA, LOUD, ("EVENT-TIMER: RX RESP TIMEOUT, Current Time = %ld\n", kalGetTimeTick()));

    ASSERT(prStaRec);
    if(!prStaRec) {
        return;
    }

    eNextState = prStaRec->eAuthAssocState;

    switch (prStaRec->eAuthAssocState) {
    case SAA_STATE_WAIT_AUTH2:
        /* Record the Status Code of Authentication Request */
        prStaRec->u2StatusCode = STATUS_CODE_AUTH_TIMEOUT;

        /* Pull back to earlier state to do retry */
        eNextState = SAA_STATE_SEND_AUTH1;
        break;

    case SAA_STATE_WAIT_AUTH4:
        /* Record the Status Code of Authentication Request */
        prStaRec->u2StatusCode = STATUS_CODE_AUTH_TIMEOUT;

        /* Pull back to earlier state to do retry */
        eNextState = SAA_STATE_SEND_AUTH3;
        break;

    case SAA_STATE_WAIT_ASSOC2:
        /* Record the Status Code of Authentication Request */
        prStaRec->u2StatusCode = STATUS_CODE_ASSOC_TIMEOUT;

        /* Pull back to earlier state to do retry */
        eNextState = SAA_STATE_SEND_ASSOC1;
        break;

    default:
        break; /* Ignore other cases */
    }


    if (eNextState != prStaRec->eAuthAssocState) {
        saaFsmSteps(prAdapter, prStaRec, eNextState, (P_SW_RFB_T)NULL);
    }

    return;
} /* end of saaFsmRunEventRxRespTimeOut() */


#ifdef CFG_AIS_SUPPORT_REJ_CNT_AVOID
VOID
saaFsmRunEventErrCountInc(
	IN P_ADAPTER_T prAdapter,
	IN P_STA_RECORD_T prStaRec,
	IN BOOLEAN fgCountReset)
{
    if (prStaRec->ucNetTypeIndex != NETWORK_TYPE_AIS_INDEX)
		return; /* only for AIS now */

	aisFsmRunEventErrCountInc(prAdapter, prStaRec, fgCountReset);
}
#endif /* CFG_AIS_SUPPORT_REJ_CNT_AVOID */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will process the Rx Auth Response Frame and then
*        trigger SAA FSM.
*
* @param[in] prSwRfb            Pointer to the SW_RFB_T structure.
*
* @return (none)
*/
/*----------------------------------------------------------------------------*/
VOID
saaFsmRunEventRxAuth (
    IN P_ADAPTER_T prAdapter,
    IN P_SW_RFB_T prSwRfb
    )
{
    P_STA_RECORD_T prStaRec;
    UINT_16 u2StatusCode;
    ENUM_AA_STATE_T eNextState;


    ASSERT(prSwRfb);
    prStaRec = cnmGetStaRecByIndex(prAdapter, prSwRfb->ucStaRecIdx);

    /* We should have the corresponding Sta Record. */
    if (!prStaRec) {
    	/* Peter: we can handle the packet without station record */
        //ASSERT(0);					
        return;
    }

    if (!IS_AP_STA(prStaRec)) {
        return;
    }

    switch(prStaRec->eAuthAssocState) {
    case SAA_STATE_SEND_AUTH1:
    case SAA_STATE_WAIT_AUTH2:
        /* Check if the incoming frame is what we are waiting for */
        if (authCheckRxAuthFrameStatus(prAdapter,
                    prSwRfb,
                    AUTH_TRANSACTION_SEQ_2,
                    &u2StatusCode) == WLAN_STATUS_SUCCESS) {

            cnmTimerStopTimer(prAdapter, &prStaRec->rTxReqDoneOrRxRespTimer);

            /* Record the Status Code of Authentication Request */
            prStaRec->u2StatusCode = u2StatusCode;

            if (u2StatusCode == STATUS_CODE_SUCCESSFUL) {

                authProcessRxAuth2_Auth4Frame(prAdapter, prSwRfb);

                if (prStaRec->ucAuthAlgNum ==
                    (UINT_8)AUTH_ALGORITHM_NUM_SHARED_KEY) {

                    eNextState = SAA_STATE_SEND_AUTH3;
                }
                else {
                    /* Update Station Record - Class 2 Flag */
                    cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_2);

                    eNextState = SAA_STATE_SEND_ASSOC1;
                }

#ifdef CFG_AIS_SUPPORT_REJ_CNT_AVOID
				/* reset error count for the BSS due to successful authentication */
				saaFsmRunEventErrCountInc(prAdapter, prStaRec, TRUE);
#endif /* CFG_AIS_SUPPORT_REJ_CNT_AVOID */
            }
            else {
                DBGLOG(SAA, INFO, ("Auth Req was rejected by ["MACSTR"], Status Code = %d\n",
                    MAC2STR(prStaRec->aucMacAddr), u2StatusCode));

                eNextState = AA_STATE_IDLE;

#ifdef CFG_AIS_SUPPORT_REJ_CNT_AVOID
				/* accumulate error count for the BSS */
				if (u2StatusCode == STATUS_CODE_ASSOC_DENIED_AP_OVERLOAD)
					saaFsmRunEventErrCountInc(prAdapter, prStaRec, FALSE);
#endif /* CFG_AIS_SUPPORT_REJ_CNT_AVOID */
            }

            /* Reset Send Auth/(Re)Assoc Frame Count */
            prStaRec->ucTxAuthAssocRetryCount = 0;

            saaFsmSteps(prAdapter, prStaRec, eNextState, (P_SW_RFB_T)NULL);
        }
        break;

    case SAA_STATE_SEND_AUTH3:
    case SAA_STATE_WAIT_AUTH4:
        /* Check if the incoming frame is what we are waiting for */
        if (authCheckRxAuthFrameStatus(prAdapter,
                    prSwRfb,
                    AUTH_TRANSACTION_SEQ_4,
                    &u2StatusCode) == WLAN_STATUS_SUCCESS) {

            cnmTimerStopTimer(prAdapter, &prStaRec->rTxReqDoneOrRxRespTimer);

            /* Record the Status Code of Authentication Request */
            prStaRec->u2StatusCode = u2StatusCode;

            if (u2StatusCode == STATUS_CODE_SUCCESSFUL) {

                authProcessRxAuth2_Auth4Frame(prAdapter, prSwRfb); /* Add for 802.11r handling */

                /* Update Station Record - Class 2 Flag */
                cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_2);

                eNextState = SAA_STATE_SEND_ASSOC1;
            }
            else {
                DBGLOG(SAA, INFO, ("Auth Req was rejected by ["MACSTR"], Status Code = %d\n",
                    MAC2STR(prStaRec->aucMacAddr), u2StatusCode));

                eNextState = AA_STATE_IDLE;
            }

            /* Reset Send Auth/(Re)Assoc Frame Count */
            prStaRec->ucTxAuthAssocRetryCount = 0;

            saaFsmSteps(prAdapter, prStaRec, eNextState, (P_SW_RFB_T)NULL);
        }
        break;

    default:
        break; /* Ignore other cases */
    }

    return;
} /* end of saaFsmRunEventRxAuth() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will process the Rx (Re)Association Response Frame and then
*        trigger SAA FSM.
*
* @param[in] prSwRfb            Pointer to the SW_RFB_T structure.
*
* @retval WLAN_STATUS_SUCCESS           if the status code was not success
* @retval WLAN_STATUS_BUFFER_RETAINED   if the status code was success
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS
saaFsmRunEventRxAssoc (
    IN P_ADAPTER_T prAdapter,
    IN P_SW_RFB_T prSwRfb
    )
{
    P_STA_RECORD_T prStaRec;
    UINT_16 u2StatusCode;
    ENUM_AA_STATE_T eNextState;
    P_SW_RFB_T prRetainedSwRfb = (P_SW_RFB_T)NULL;
    WLAN_STATUS rStatus = WLAN_STATUS_SUCCESS;


    ASSERT(prSwRfb);
    prStaRec = cnmGetStaRecByIndex(prAdapter, prSwRfb->ucStaRecIdx);

    /* We should have the corresponding Sta Record. */
    if (!prStaRec) {
        ASSERT(0);
        return rStatus;
    }

    if (!IS_AP_STA(prStaRec)) {
        return rStatus;
    }

    switch (prStaRec->eAuthAssocState) {
    case SAA_STATE_SEND_ASSOC1:
    case SAA_STATE_WAIT_ASSOC2:
        /* TRUE if the incoming frame is what we are waiting for */
        if (assocCheckRxReAssocRspFrameStatus(prAdapter,
                    prSwRfb,
                    &u2StatusCode) == WLAN_STATUS_SUCCESS) {

            cnmTimerStopTimer(prAdapter, &prStaRec->rTxReqDoneOrRxRespTimer);


            /* Record the Status Code of Authentication Request */
            prStaRec->u2StatusCode = u2StatusCode;

            if (u2StatusCode == STATUS_CODE_SUCCESSFUL) {

                /* Update Station Record - Class 3 Flag */
                /* NOTE(Kevin): Moved to AIS FSM for roaming issue -
                 * We should deactivate the STA_RECORD_T of previous AP before
                 * activate new one in Driver.
                 */
                //cnmStaRecChangeState(prStaRec, STA_STATE_3);

                prStaRec->ucJoinFailureCount = 0; // Clear history.

                prRetainedSwRfb = prSwRfb;
                rStatus = WLAN_STATUS_PENDING;
            }
            else {
                DBGLOG(SAA, INFO, ("Assoc Req was rejected by ["MACSTR"], Status Code = %d\n",
                    MAC2STR(prStaRec->aucMacAddr), u2StatusCode));
            }

            /* Reset Send Auth/(Re)Assoc Frame Count */
            prStaRec->ucTxAuthAssocRetryCount = 0;

            /* update RCPI */
            prStaRec->ucRCPI = prSwRfb->prHifRxHdr->ucRcpi;

            eNextState = AA_STATE_IDLE;

            saaFsmSteps(prAdapter, prStaRec, eNextState, prRetainedSwRfb);
        }
        break;

    default:
        break; /* Ignore other cases */
    }

    return rStatus;

} /* end of saaFsmRunEventRxAssoc() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will check the incoming Deauth Frame.
*
* @param[in] prSwRfb            Pointer to the SW_RFB_T structure.
*
* @retval WLAN_STATUS_SUCCESS   Always not retain deauthentication frames
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS
saaFsmRunEventRxDeauth (
    IN P_ADAPTER_T prAdapter,
    IN P_SW_RFB_T prSwRfb
    )
{
    P_STA_RECORD_T prStaRec;
//#if DBG
    P_WLAN_DEAUTH_FRAME_T prDeauthFrame;
//#endif /* DBG */


    ASSERT(prSwRfb);
    prStaRec = cnmGetStaRecByIndex(prAdapter, prSwRfb->ucStaRecIdx);

//#if DBG
    prDeauthFrame = (P_WLAN_DEAUTH_FRAME_T) prSwRfb->pvHeader;

    DBGLOG(SAA, INFO, ("Rx Deauth frame from BSSID=["MACSTR"].\n",
        MAC2STR(prDeauthFrame->aucBSSID)));
//#endif /* DBG */

    do {

        /* We should have the corresponding Sta Record. */
        if (!prStaRec) {
            break;
        }

        if (IS_STA_IN_AIS(prStaRec)) {
            P_AIS_BSS_INFO_T prAisBssInfo;


            if (!IS_AP_STA(prStaRec)) {
                break;
            }

            prAisBssInfo = &(prAdapter->rWifiVar.arBssInfo[NETWORK_TYPE_AIS_INDEX]);

            if (prStaRec->ucStaState > STA_STATE_1) {

                /* Check if this is the AP we are associated or associating with */
                if (authProcessRxDeauthFrame(prSwRfb,
                            prStaRec->aucMacAddr,
                            &prStaRec->u2ReasonCode) == WLAN_STATUS_SUCCESS) {

					DBGLOG(SAA, INFO, ("Deauth reason = %d\n",
						prStaRec->u2ReasonCode));

                    if (STA_STATE_3 == prStaRec->ucStaState) {
                        P_MSG_AIS_ABORT_T prAisAbortMsg;

                        /* NOTE(Kevin): Change state immediately to avoid starvation of
                         * MSG buffer because of too many deauth frames before changing
                         * the STA state.
                         */
                        cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_1);

                        prAisAbortMsg = (P_MSG_AIS_ABORT_T)cnmMemAlloc(prAdapter, RAM_TYPE_MSG, sizeof(MSG_AIS_ABORT_T));
                        if (!prAisAbortMsg) {
                            break;
                        }

                        prAisAbortMsg->rMsgHdr.eMsgId = MID_SAA_AIS_FSM_ABORT;
                        prAisAbortMsg->ucReasonOfDisconnect = DISCONNECT_REASON_CODE_DEAUTHENTICATED;
                        prAisAbortMsg->fgDelayIndication = FALSE;

                        mboxSendMsg(prAdapter,
                                MBOX_ID_0,
                                (P_MSG_HDR_T) prAisAbortMsg,
                                MSG_SEND_METHOD_BUF);
                    }
                    else {

                        /* TODO(Kevin): Joining Abort */
                    }

                }

            }

        }
#if CFG_ENABLE_WIFI_DIRECT
        else if (prAdapter->fgIsP2PRegistered && IS_STA_IN_P2P(prStaRec)) {
            /* TODO(Kevin) */
            p2pFsmRunEventRxDeauthentication(prAdapter, prStaRec, prSwRfb);
        }
#endif
#if CFG_ENABLE_BT_OVER_WIFI
        else if (IS_STA_IN_BOW(prStaRec)) {
            bowRunEventRxDeAuth(prAdapter, prStaRec, prSwRfb);
        }
#endif
        else {
            ASSERT(0);
        }

    } while (FALSE);

    return WLAN_STATUS_SUCCESS;

} /* end of saaFsmRunEventRxDeauth() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will check the incoming Disassociation Frame.
*
* @param[in] prSwRfb            Pointer to the SW_RFB_T structure.
*
* @retval WLAN_STATUS_SUCCESS   Always not retain disassociation frames
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS
saaFsmRunEventRxDisassoc (
    IN P_ADAPTER_T prAdapter,
    IN P_SW_RFB_T prSwRfb
    )
{
    P_STA_RECORD_T prStaRec;
//#if DBG
    P_WLAN_DISASSOC_FRAME_T prDisassocFrame;
//#endif /* DBG */


    ASSERT(prSwRfb);
    prStaRec = cnmGetStaRecByIndex(prAdapter, prSwRfb->ucStaRecIdx);

//#if DBG
    prDisassocFrame = (P_WLAN_DISASSOC_FRAME_T) prSwRfb->pvHeader;

    DBGLOG(SAA, INFO, ("Rx Disassoc frame from BSSID=["MACSTR"].\n",
        MAC2STR(prDisassocFrame->aucBSSID)));
//#endif /* DBG */

    do {

        /* We should have the corresponding Sta Record. */
        if (!prStaRec) {
            break;
        }

        if (IS_STA_IN_AIS(prStaRec)) {
            P_AIS_BSS_INFO_T prAisBssInfo;


            if (!IS_AP_STA(prStaRec)) {
                break;
            }

            prAisBssInfo = &(prAdapter->rWifiVar.arBssInfo[NETWORK_TYPE_AIS_INDEX]);

            if (prStaRec->ucStaState > STA_STATE_1) {

                /* Check if this is the AP we are associated or associating with */
                if (assocProcessRxDisassocFrame(prAdapter,
                            prSwRfb,
                            prStaRec->aucMacAddr,
                            &prStaRec->u2ReasonCode) == WLAN_STATUS_SUCCESS) {

					DBGLOG(SAA, INFO, ("Disassoc reason = %d\n",
						prStaRec->u2ReasonCode));

                    if (STA_STATE_3 == prStaRec->ucStaState) {
                        P_MSG_AIS_ABORT_T prAisAbortMsg;

                        prAisAbortMsg = (P_MSG_AIS_ABORT_T)cnmMemAlloc(prAdapter, RAM_TYPE_MSG, sizeof(MSG_AIS_ABORT_T));
                        if (!prAisAbortMsg) {
                            break;
                        }

                        prAisAbortMsg->rMsgHdr.eMsgId = MID_SAA_AIS_FSM_ABORT;
                        prAisAbortMsg->ucReasonOfDisconnect = DISCONNECT_REASON_CODE_DISASSOCIATED;
                        prAisAbortMsg->fgDelayIndication = FALSE;

                        mboxSendMsg(prAdapter,
                                MBOX_ID_0,
                                (P_MSG_HDR_T) prAisAbortMsg,
                                MSG_SEND_METHOD_BUF);
                    }
                    else {

                        /* TODO(Kevin): Joining Abort */
                    }

                }

            }

        }
#if CFG_ENABLE_WIFI_DIRECT
        else if (prAdapter->fgIsP2PRegistered && IS_STA_IN_P2P(prStaRec)) {
            /* TODO(Kevin) */
            p2pFsmRunEventRxDisassociation(prAdapter, prStaRec, prSwRfb);
        }
#endif
#if CFG_ENABLE_BT_OVER_WIFI
        else if (IS_STA_IN_BOW(prStaRec)) {
            /* TODO(Kevin) */
        }
#endif
        else {
            ASSERT(0);
        }

    } while (FALSE);

    return WLAN_STATUS_SUCCESS;

} /* end of saaFsmRunEventRxDisassoc() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will handle the Abort Event to SAA FSM.
*
* @param[in] prMsgHdr   Message of Abort Request for a particular STA.
*
* @return none
*/
/*----------------------------------------------------------------------------*/
VOID
saaFsmRunEventAbort (
    IN P_ADAPTER_T prAdapter,
    IN P_MSG_HDR_T prMsgHdr
    )
{
    P_MSG_SAA_FSM_ABORT_T prSaaFsmAbortMsg;
    P_STA_RECORD_T prStaRec;


    ASSERT(prMsgHdr);

    prSaaFsmAbortMsg = (P_MSG_SAA_FSM_ABORT_T)prMsgHdr;
    prStaRec = prSaaFsmAbortMsg->prStaRec;

    ASSERT(prStaRec);
    if(!prStaRec) {
        cnmMemFree(prAdapter, prMsgHdr);
        return;
    }

    DBGLOG(SAA, LOUD, ("EVENT-ABORT: Stop SAA FSM.\n"));

    cnmMemFree(prAdapter, prMsgHdr);


    /* Reset Send Auth/(Re)Assoc Frame Count */
    prStaRec->ucTxAuthAssocRetryCount = 0;

    /* Cancel JOIN relative Timer */
    cnmTimerStopTimer(prAdapter, &prStaRec->rTxReqDoneOrRxRespTimer);

    if (prStaRec->eAuthAssocState != AA_STATE_IDLE) {
#if DBG
        DBGLOG(SAA, LOUD, ("EVENT-ABORT: Previous Auth/Assoc State == %s.\n",
            apucDebugAAState[prStaRec->eAuthAssocState]));
#else
        DBGLOG(SAA, LOUD, ("EVENT-ABORT: Previous Auth/Assoc State == %d.\n",
            prStaRec->eAuthAssocState));
#endif
    }

    /* Free this StaRec */
    cnmStaRecFree(prAdapter, prStaRec, FALSE);

    return;
} /* end of saaFsmRunEventAbort() */


/* TODO(Kevin): following code will be modified and move to AIS FSM */

