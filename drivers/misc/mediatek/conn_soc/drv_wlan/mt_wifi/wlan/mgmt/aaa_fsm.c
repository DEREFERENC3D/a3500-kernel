/*
** $Id: //Department/DaVinci/BRANCHES/MT6620_WIFI_DRIVER_V2_3/mgmt/aaa_fsm.c#2 $
*/

/*! \file   "aaa_fsm.c"
    \brief  This file defines the FSM for AAA MODULE.

    This file defines the FSM for AAA MODULE.
*/



/*
** $Log: aaa_fsm.c $
 *
 * 07 17 2012 yuche.tsai
 * NULL
 * Compile no error before trial run.
 *
 * 06 13 2012 yuche.tsai
 * NULL
 * Update maintrunk driver.
 * Add support for driver compose assoc request frame.
 *
 * 03 02 2012 terry.wu
 * NULL
 * Sync CFG80211 modification from branch 2,2.
 *
 * 02 22 2012 yuche.tsai
 * NULL
 * Solve sigma test 5.1.3 issue, assoc response should have P2P IE.
 *
 * 12 02 2011 yuche.tsai
 * NULL
 * Resolve inorder issue under AP mode.
 * 
 * data frame may TX before assoc response frame.
 *
 * 11 18 2011 yuche.tsai
 * NULL
 * CONFIG P2P support RSSI query, default turned off.
 *
 * 06 17 2011 terry.wu
 * NULL
 * Add BoW 11N support.
 *
 * 06 02 2011 eddie.chen
 * [WCXRP00000759] [MT6620 Wi-Fi][DRV] Update RCPI in AAA
 * Update RCPI when receiving Assoc request.
 *
 * 04 21 2011 terry.wu
 * [WCXRP00000674] [MT6620 Wi-Fi][Driver] Refine AAA authSendAuthFrame
 * Add network type parameter to authSendAuthFrame.
 *
 * 04 15 2011 chinghwa.yu
 * [WCXRP00000065] Update BoW design and settings
 * Add BOW short range mode.
 *
 * 04 09 2011 chinghwa.yu
 * [WCXRP00000065] Update BoW design and settings
 * Change Link connection event procedure and change skb length check to 1512 bytes.
 *
 * 03 09 2011 wh.su
 * [WCXRP00000530] [MT6620 Wi-Fi] [Driver] skip doing p2pRunEventAAAComplete after send assoc response Tx Done
 * Skip to call p2pRunEventAAAComplete to avoid indicate STA connect twice.
 *
 * 03 04 2011 terry.wu
 * [WCXRP00000515] [MT6620 Wi-Fi][Driver] Surpress compiler warning which is identified by GNU compiler collection
 * Remove unused variable.
 *
 * 02 16 2011 yuche.tsai
 * [WCXRP00000429] [Volunteer Patch][MT6620][Driver] Hot Spot Client Limit Issue
 * Add more check after RX assoc frame under Hot-Spot mode.
 *
 * 02 09 2011 yuche.tsai
 * [WCXRP00000429] [Volunteer Patch][MT6620][Driver] Hot Spot Client Limit Issue
 * Fix Client Limit Issue.
 *
 * 01 25 2011 yuche.tsai
 * [WCXRP00000388] [Volunteer Patch][MT6620][Driver/Fw] change Station Type in station record.
 * Change Station Type in Station Record, Modify MACRO definition for getting station type & network type index & Role.
 *
 * 01 15 2011 puff.wen
 * NULL
 * [On behalf of Frog] Add CFG_ENABLE_WIFI_DIRECT to p2pRunEventAAAComplete
 *
 * 01 14 2011 yuche.tsai
 * [WCXRP00000352] [Volunteer Patch][MT6620][Driver] P2P Statsion Record Client List Issue
 * Modify AAA flow according to CM's comment.
 *
 * 09 03 2010 kevin.huang
 * NULL
 * Refine #include sequence and solve recursive/nested #include issue
 *
 * 08 29 2010 yuche.tsai
 * NULL
 * Fix Compile warning, type cast from UINT_32 to UINT_16.
 *
 * 08 26 2010 yuche.tsai
 * NULL
 * In P2P AT GO test mode under WinXP, we would not indicate connected event to host.
 *
 * 08 24 2010 cm.chang
 * NULL
 * Support RLM initail channel of Ad-hoc, P2P and BOW
 *
 * 08 23 2010 chinghwa.yu
 * NULL
 * Update for BOW.
 *
 * 08 20 2010 kevin.huang
 * NULL
 * Modify AAA Module for changing STA STATE 3 at p2p/bowRunEventAAAComplete()
 *
 * 08 17 2010 yuche.tsai
 * NULL
 * Fix bug while enabling P2P GO.
 *
 * 08 16 2010 kevin.huang
 * NULL
 * Refine AAA functions
 *
 * 07 08 2010 cp.wu
 *
 * [WPD00003833] [MT6620 and MT5931] Driver migration - move to new repository.
 *
 * 06 21 2010 cp.wu
 * [WPD00003833][MT6620 and MT5931] Driver migration
 * refine TX-DONE callback.
 *
 * 06 21 2010 yuche.tsai
 * [WPD00003839][MT6620 5931][P2P] Feature migration
 * modify due to P2P functino call prototype change.
 *
 * 06 17 2010 yuche.tsai
 * [WPD00003839][MT6620 5931][P2P] Feature migration
 * First draft for migration P2P FSM from FW to Driver.
 *
 * 04 02 2010 kevin.huang
 * [BORA00000603][WIFISYS] [New Feature] AAA Module Support
 * Modify CFG flags
 *
 * 02 26 2010 kevin.huang
 * [BORA00000603][WIFISYS] [New Feature] AAA Module Support
 * add support of Driver STA_RECORD_T activation
 *
 * 02 04 2010 kevin.huang
 * [BORA00000603][WIFISYS] [New Feature] AAA Module Support
 * Add AAA Module Support, Revise Net Type to Net Type Index for array lookup
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


#if CFG_SUPPORT_AAA
/*----------------------------------------------------------------------------*/
/*!
* @brief This function will process the Rx Auth Request Frame and then
*        trigger AAA FSM.
*
* @param[in] prAdapter          Pointer to the Adapter structure.
* @param[in] prSwRfb            Pointer to the SW_RFB_T structure.
*
* @return (none)
*/
/*----------------------------------------------------------------------------*/
VOID
aaaFsmRunEventRxAuth (
    IN P_ADAPTER_T prAdapter,
    IN P_SW_RFB_T prSwRfb
    )
{
    P_BSS_INFO_T prBssInfo;
    P_STA_RECORD_T prStaRec = (P_STA_RECORD_T)NULL;
    UINT_16 u2StatusCode;
    BOOLEAN fgReplyAuth = FALSE;
    ENUM_NETWORK_TYPE_INDEX_T eNetTypeIndex;


    ASSERT(prAdapter);

    do {


        //4 <1> Check P2P network conditions
#if CFG_ENABLE_WIFI_DIRECT
        if(prAdapter->fgIsP2PRegistered){
            prBssInfo = &(prAdapter->rWifiVar.arBssInfo[NETWORK_TYPE_P2P_INDEX]);

            if (prBssInfo->fgIsNetActive) {

                //4 <1.1> Validate Auth Frame by Auth Algorithm/Transation Seq
                if (WLAN_STATUS_SUCCESS ==
                    authProcessRxAuth1Frame(prAdapter,
                                            prSwRfb,
                                            prBssInfo->aucBSSID,
                                            AUTH_ALGORITHM_NUM_OPEN_SYSTEM,
                                            AUTH_TRANSACTION_SEQ_1,
                                            &u2StatusCode)) {

                    if (STATUS_CODE_SUCCESSFUL == u2StatusCode) {
                        //4 <1.2> Validate Auth Frame for Network Specific Conditions
                        fgReplyAuth = p2pFuncValidateAuth(
                                            prAdapter,
                                            prSwRfb,
                                            &prStaRec,
                                            &u2StatusCode);
                    }
                    else {
                        fgReplyAuth = TRUE;
                    }
                    eNetTypeIndex = NETWORK_TYPE_P2P_INDEX;
                    break;
                }
            }
        }
#endif /* CFG_ENABLE_WIFI_DIRECT */

        //4 <2> Check BOW network conditions
#if CFG_ENABLE_BT_OVER_WIFI
        {
            prBssInfo = &(prAdapter->rWifiVar.arBssInfo[NETWORK_TYPE_BOW_INDEX]);

            if ((prBssInfo->fgIsNetActive) &&
            (OP_MODE_BOW == prBssInfo->eCurrentOPMode)) {

                //4 <2.1> Validate Auth Frame by Auth Algorithm/Transation Seq
                /* Check if for this BSSID */
                if (WLAN_STATUS_SUCCESS ==
                    authProcessRxAuth1Frame(prAdapter,
                                            prSwRfb,
                                            prBssInfo->aucBSSID,
                                            AUTH_ALGORITHM_NUM_OPEN_SYSTEM,
                                            AUTH_TRANSACTION_SEQ_1,
                                            &u2StatusCode)) {

                    if (STATUS_CODE_SUCCESSFUL == u2StatusCode) {

                        //4 <2.2> Validate Auth Frame for Network Specific Conditions
                        fgReplyAuth = bowValidateAuth(prAdapter, prSwRfb, &prStaRec, &u2StatusCode);

                    }
                    else {

                        fgReplyAuth = TRUE;
                    }
                    eNetTypeIndex = NETWORK_TYPE_BOW_INDEX;
                    /* TODO(Kevin): Allocate a STA_RECORD_T for new client */
                    break;
                }
            }
        }
#endif /* CFG_ENABLE_BT_OVER_WIFI */

        return;
    } while (FALSE);

    if(prStaRec) {
        /* update RCPI */
        prStaRec->ucRCPI = prSwRfb->prHifRxHdr->ucRcpi;
    }

    //4 <3> Update STA_RECORD_T and reply Auth_2(Response to Auth_1) Frame
    if (fgReplyAuth) {

        if (prStaRec) {

            if (u2StatusCode == STATUS_CODE_SUCCESSFUL) {
                if (prStaRec->eAuthAssocState != AA_STATE_IDLE) {
                    DBGLOG(AAA, WARN, ("Previous AuthAssocState (%d) != IDLE.\n",
                        prStaRec->eAuthAssocState));
                }

                prStaRec->eAuthAssocState = AAA_STATE_SEND_AUTH2;
            }
            else {
                prStaRec->eAuthAssocState = AA_STATE_IDLE;

                /* NOTE(Kevin): Change to STATE_1 */
                cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_1);
            }

            /* Update the record join time. */
            GET_CURRENT_SYSTIME(&prStaRec->rUpdateTime);

            /* Update Station Record - Status/Reason Code */
            prStaRec->u2StatusCode = u2StatusCode;

            prStaRec->ucAuthAlgNum = AUTH_ALGORITHM_NUM_OPEN_SYSTEM;
        }
        else {
            /* NOTE(Kevin): We should have STA_RECORD_T if the status code was successful */
            ASSERT(!(u2StatusCode == STATUS_CODE_SUCCESSFUL));
        }

        /* NOTE: Ignore the return status for AAA */
        //4 <4> Reply  Auth
        authSendAuthFrame(prAdapter,
                        prStaRec,
                        eNetTypeIndex,
                        prSwRfb,
                        AUTH_TRANSACTION_SEQ_2,
                        u2StatusCode);

    }

    return;
} /* end of aaaFsmRunEventRxAuth() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will process the Rx (Re)Association Request Frame and then
*        trigger AAA FSM.
*
* @param[in] prAdapter          Pointer to the Adapter structure.
* @param[in] prSwRfb            Pointer to the SW_RFB_T structure.
*
* @retval WLAN_STATUS_SUCCESS           Always return success
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS
aaaFsmRunEventRxAssoc (
    IN P_ADAPTER_T prAdapter,
    IN P_SW_RFB_T prSwRfb
    )
{
    P_BSS_INFO_T prBssInfo;
    P_STA_RECORD_T prStaRec = (P_STA_RECORD_T)NULL;
    UINT_16 u2StatusCode = STATUS_CODE_RESERVED;
    BOOLEAN fgReplyAssocResp = FALSE;


    ASSERT(prAdapter);

    do {

        //4 <1> Check if we have the STA_RECORD_T for incoming Assoc Req
        prStaRec = cnmGetStaRecByIndex(prAdapter, prSwRfb->ucStaRecIdx);

        /* We should have the corresponding Sta Record. */
        if ((!prStaRec) || (!prStaRec->fgIsInUse)) {
            ASSERT(0); // Only for debug phase
            break;
        }

        if (!IS_CLIENT_STA(prStaRec)) {
            break;
        }

        if (prStaRec->ucStaState == STA_STATE_3) {
            /* Do Reassocation */
        }
        else if ((prStaRec->ucStaState == STA_STATE_2) &&
                 (prStaRec->eAuthAssocState == AAA_STATE_SEND_AUTH2)) {
            /* Normal case */
        }
        else {
            DBGLOG(AAA, WARN, ("Previous AuthAssocState (%d) != SEND_AUTH2.\n",
                prStaRec->eAuthAssocState));
            break;
        }

        /* update RCPI */
        prStaRec->ucRCPI = prSwRfb->prHifRxHdr->ucRcpi;

        //4 <2> Check P2P network conditions
#if CFG_ENABLE_WIFI_DIRECT
        if ((prAdapter->fgIsP2PRegistered) && (IS_STA_IN_P2P(prStaRec))) {

            prBssInfo = &(prAdapter->rWifiVar.arBssInfo[NETWORK_TYPE_P2P_INDEX]);

            if (prBssInfo->fgIsNetActive) {

                //4 <2.1> Validate Assoc Req Frame and get Status Code
                /* Check if for this BSSID */
                if (WLAN_STATUS_SUCCESS ==
                    assocProcessRxAssocReqFrame(prAdapter,
                                                prSwRfb,
                                                &u2StatusCode)) {

                    if (STATUS_CODE_SUCCESSFUL == u2StatusCode) {
                        //4 <2.2> Validate Assoc Req  Frame for Network Specific Conditions
                        fgReplyAssocResp = p2pFuncValidateAssocReq(
                                                prAdapter,
                                                prSwRfb,
                                                (PUINT_16)&u2StatusCode);
                    }
                    else {
                        fgReplyAssocResp = TRUE;
                    }

                    break;
                }
            }
        }
#endif /* CFG_ENABLE_WIFI_DIRECT */

        //4 <3> Check BOW network conditions
#if CFG_ENABLE_BT_OVER_WIFI
        if (IS_STA_IN_BOW(prStaRec)) {

            prBssInfo = &(prAdapter->rWifiVar.arBssInfo[NETWORK_TYPE_BOW_INDEX]);

            if ((prBssInfo->fgIsNetActive) &&
                (OP_MODE_BOW == prBssInfo->eCurrentOPMode)){

                //4 <3.1> Validate Auth Frame by Auth Algorithm/Transation Seq
                /* Check if for this BSSID */
                if (WLAN_STATUS_SUCCESS ==
                    assocProcessRxAssocReqFrame(prAdapter,
                                                prSwRfb,
                                                &u2StatusCode)) {

                    if (STATUS_CODE_SUCCESSFUL == u2StatusCode) {

                        //4 <3.2> Validate Auth Frame for Network Specific Conditions
                        fgReplyAssocResp = bowValidateAssocReq(prAdapter, prSwRfb, &u2StatusCode);

                    }
                    else {

                        fgReplyAssocResp = TRUE;
                    }

                    /* TODO(Kevin): Allocate a STA_RECORD_T for new client */
                    break;
                }
            }
        }
#endif /* CFG_ENABLE_BT_OVER_WIFI */

        return WLAN_STATUS_SUCCESS; // To release the SW_RFB_T
    } while (FALSE);


    //4 <4> Update STA_RECORD_T and reply Assoc Resp Frame
    if (fgReplyAssocResp) {
        UINT_16     u2IELength;
        PUINT_8     pucIE;

        if ((((P_WLAN_ASSOC_REQ_FRAME_T)(prSwRfb->pvHeader))->u2FrameCtrl & MASK_FRAME_TYPE) ==
            MAC_FRAME_REASSOC_REQ) {

            u2IELength = prSwRfb->u2PacketLen -
                (UINT_16)OFFSET_OF(WLAN_REASSOC_REQ_FRAME_T, aucInfoElem[0]);

            pucIE = ((P_WLAN_REASSOC_REQ_FRAME_T)(prSwRfb->pvHeader))->aucInfoElem;
        }
        else {
            u2IELength = prSwRfb->u2PacketLen -
                (UINT_16)OFFSET_OF(WLAN_ASSOC_REQ_FRAME_T, aucInfoElem[0]);

            pucIE = ((P_WLAN_ASSOC_REQ_FRAME_T)(prSwRfb->pvHeader))->aucInfoElem;
        }

        rlmProcessAssocReq(prAdapter, prSwRfb, pucIE, u2IELength);

        //4 <4.1> Assign Association ID
        if (u2StatusCode == STATUS_CODE_SUCCESSFUL) {

#if CFG_ENABLE_WIFI_DIRECT
            if ((prAdapter->fgIsP2PRegistered) && (IS_STA_IN_P2P(prStaRec))) {
                if (p2pRunEventAAAComplete(prAdapter, prStaRec) == WLAN_STATUS_SUCCESS) {
                    prStaRec->u2AssocId = bssAssignAssocID(prStaRec);
                    //prStaRec->eAuthAssocState = AA_STATE_IDLE;
                    prStaRec->eAuthAssocState = AAA_STATE_SEND_ASSOC2; // NOTE(Kevin): for TX done

                    /* NOTE(Kevin): Method A: Change to STATE_3 before handle TX Done */
                    //cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_3);
                }
                else {
                    /* Client List FULL. */
                    u2StatusCode = STATUS_CODE_REQ_DECLINED;

                    prStaRec->u2AssocId = 0; /* Invalid Assocation ID */

                    /* If (Re)association fail, the peer can try Assocation w/o Auth immediately */
                    prStaRec->eAuthAssocState = AAA_STATE_SEND_AUTH2;

                    /* NOTE(Kevin): Better to change state here, not at TX Done */
                    cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_2);
                }
            }
#endif

#if CFG_ENABLE_BT_OVER_WIFI
            if ((IS_STA_IN_BOW(prStaRec))) {

//                    if (bowRunEventAAAComplete(prAdapter, prStaRec) == WLAN_STATUS_SUCCESS) {
                prStaRec->u2AssocId = bssAssignAssocID(prStaRec);
                prStaRec->eAuthAssocState = AAA_STATE_SEND_ASSOC2; // NOTE(Kevin): for TX done

                /* NOTE(Kevin): Method A: Change to STATE_3 before handle TX Done */
                //cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_3);
            }
#endif
        }
        else {
            prStaRec->u2AssocId = 0; /* Invalid Assocation ID */

            /* If (Re)association fail, the peer can try Assocation w/o Auth immediately */
            prStaRec->eAuthAssocState = AAA_STATE_SEND_AUTH2;

            /* NOTE(Kevin): Better to change state here, not at TX Done */
            cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_2);
        }

        /* Update the record join time. */
        GET_CURRENT_SYSTIME(&prStaRec->rUpdateTime);

        /* Update Station Record - Status/Reason Code */
        prStaRec->u2StatusCode = u2StatusCode;

        /* NOTE: Ignore the return status for AAA */
        //4 <4.2> Reply  Assoc Resp
        assocSendReAssocRespFrame(prAdapter, prStaRec);

    }

    return WLAN_STATUS_SUCCESS;

} /* end of aaaFsmRunEventRxAssoc() */


/*----------------------------------------------------------------------------*/
/*!
* @brief This function will handle TxDone(Auth2/AssocReq) Event of AAA FSM.
*
* @param[in] prAdapter      Pointer to the Adapter structure.
* @param[in] prMsduInfo     Pointer to the MSDU_INFO_T.
* @param[in] rTxDoneStatus  Return TX status of the Auth1/Auth3/AssocReq frame.
*
* @retval WLAN_STATUS_SUCCESS
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS
aaaFsmRunEventTxDone (
    IN P_ADAPTER_T              prAdapter,
    IN P_MSDU_INFO_T            prMsduInfo,
    IN ENUM_TX_RESULT_CODE_T    rTxDoneStatus
    )
{
    P_STA_RECORD_T prStaRec;
    P_BSS_INFO_T prBssInfo;


    ASSERT(prAdapter);
    ASSERT(prMsduInfo);

    DBGLOG(AAA, LOUD, ("EVENT-TX DONE: Current Time = %ld\n", kalGetTimeTick()));

    prStaRec = cnmGetStaRecByIndex(prAdapter, prMsduInfo->ucStaRecIndex);

    if ((!prStaRec) || (!prStaRec->fgIsInUse)) {
        return WLAN_STATUS_SUCCESS; /* For the case of replying ERROR STATUS CODE */
    }

    ASSERT(prStaRec->ucNetTypeIndex < NETWORK_TYPE_INDEX_NUM);

    prBssInfo = &(prAdapter->rWifiVar.arBssInfo[prStaRec->ucNetTypeIndex]);

    switch (prStaRec->eAuthAssocState) {
    case AAA_STATE_SEND_AUTH2:
        {
            /* Strictly check the outgoing frame is matched with current AA STATE */
            if (authCheckTxAuthFrame(prAdapter,
                                    prMsduInfo,
                                    AUTH_TRANSACTION_SEQ_2) != WLAN_STATUS_SUCCESS) {
                break;
            }

            if (STATUS_CODE_SUCCESSFUL == prStaRec->u2StatusCode) {
                if (TX_RESULT_SUCCESS == rTxDoneStatus) {

                    /* NOTE(Kevin): Change to STATE_2 at TX Done */
                    cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_2);
                }
                else {

                    prStaRec->eAuthAssocState = AA_STATE_IDLE;

                    /* NOTE(Kevin): Change to STATE_1 */
                    cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_1);

#if CFG_ENABLE_WIFI_DIRECT
                    if ((prAdapter->fgIsP2PRegistered) &&
                        (IS_STA_IN_P2P(prStaRec))) {
                        p2pRunEventAAATxFail(prAdapter, prStaRec);
                    }
#endif /* CFG_ENABLE_WIFI_DIRECT */

#if CFG_ENABLE_BT_OVER_WIFI
                    if (IS_STA_IN_BOW(prStaRec)) {
                        bowRunEventAAATxFail(prAdapter, prStaRec);
                    }
#endif /* CFG_ENABLE_BT_OVER_WIFI */
                }

            }
            /* NOTE(Kevin): Ignore the TX Done Event of Auth Frame with Error Status Code */

        }
        break;

    case AAA_STATE_SEND_ASSOC2:
        {
            /* Strictly check the outgoing frame is matched with current SAA STATE */
            if (assocCheckTxReAssocRespFrame(prAdapter, prMsduInfo) != WLAN_STATUS_SUCCESS) {
                break;
            }

            if (STATUS_CODE_SUCCESSFUL == prStaRec->u2StatusCode) {
                if (TX_RESULT_SUCCESS == rTxDoneStatus) {

                    prStaRec->eAuthAssocState = AA_STATE_IDLE;

                    /* NOTE(Kevin): Change to STATE_3 at TX Done */
#if CFG_ENABLE_WIFI_DIRECT
                    if ((prAdapter->fgIsP2PRegistered) &&
                        (IS_STA_IN_P2P(prStaRec))) {
                        p2pRunEventAAASuccess(prAdapter, prStaRec);
                    }
#endif /* CFG_ENABLE_WIFI_DIRECT */

#if CFG_ENABLE_BT_OVER_WIFI

                    if (IS_STA_IN_BOW(prStaRec))
                        {
                        bowRunEventAAAComplete(prAdapter, prStaRec);
                    }
#endif /* CFG_ENABLE_BT_OVER_WIFI */

                }
                else {

                    prStaRec->eAuthAssocState = AAA_STATE_SEND_AUTH2;

                    /* NOTE(Kevin): Change to STATE_2 */
                    cnmStaRecChangeState(prAdapter, prStaRec, STA_STATE_2);

#if CFG_ENABLE_WIFI_DIRECT
                    if ((prAdapter->fgIsP2PRegistered) &&
                        (IS_STA_IN_P2P(prStaRec))) {
                        p2pRunEventAAATxFail(prAdapter, prStaRec);
                    }
#endif /* CFG_ENABLE_WIFI_DIRECT */

#if CFG_ENABLE_BT_OVER_WIFI
                    if (IS_STA_IN_BOW(prStaRec)) {
                        bowRunEventAAATxFail(prAdapter, prStaRec);
                    }
#endif /* CFG_ENABLE_BT_OVER_WIFI */

                }
            }
            /* NOTE(Kevin): Ignore the TX Done Event of Auth Frame with Error Status Code */
        }
        break;

    default:
        break; /* Ignore other cases */
    }


    return WLAN_STATUS_SUCCESS;

} /* end of aaaFsmRunEventTxDone() */
#endif /* CFG_SUPPORT_AAA */




/* TODO(Kevin): following code will be modified and move to AIS FSM */

