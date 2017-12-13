/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*	(c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*	2011-2011, All Rights Reserved					*/
/*									*/
/* MODULE NAME : udp_pub.c						*/
/* PRODUCT(S)  : MMS-EASE Lite						*/
/*									*/
/* MODULE DESCRIPTION :							*/
/*	UDP Publisher sample application				*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	NONE								*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 08/31/11  JRB	   Fix CLTP encoding.				*/
/* 07/22/11  JRB	   Initial revision				*/
/************************************************************************/
#include "iec_glbtypes.h"
#include "iec_sysincs.h"

#include <sys/types.h>
#include "signal.h"
#include "process.h"
#include "winsock2.h"



//#include <ws2tcpip.h>	/* for IP_MULTICAIEC_TTL	*/
#include "iec_90_5_load_cfg.h"
#include "iec_90_5.h"

static SOCKET IEC_90_5_rx_socket;



static unsigned char goose_packet[] = {0x61,0x82,0x02,0xf5,0x80,0x0d,
0x6d,0x79,0x64,0x6f,0x6d,0x2f,0x6d,0x79,0x67,0x63,0x52,0x65,0x66,0x81,0x01,0x04,
0x82,0x0f,0x6d,0x79,0x64,0x6f,0x6d,0x2f,0x6d,0x79,0x64,0x61,0x74,0x61,0x73,0x65,
0x74,0x83,0x09,0x74,0x65,0x73,0x74,0x41,0x70,0x70,0x49,0x44,0x84,0x08,0x3f,0xfb,
0x32,0x4c,0x00,0x00,0x00,0x00,0x85,0x01,0x01,0x86,0x01,0x01,0x87,0x01,0x00,0x88,
0x01,0x20,0x89,0x01,0x00,0x8a,0x01,0x03,0xab,0x82,0x02,0xa7,0xa2,0x82,0x02,0x65,
0xa2,0x35,0x85,0x01,0x03,0x87,0x05,0x08,0x40,0x87,0x7c,0xee,0x85,0x01,0x05,0x87,
0x05,0x08,0x40,0xc7,0x7c,0xee,0x85,0x01,0x07,0x87,0x05,0x08,0x41,0x03,0xc2,0x8f,
0x85,0x01,0x09,0x87,0x05,0x08,0x41,0x23,0xc2,0x8f,0x84,0x03,0x00,0x55,0x55,0x8c,
0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x2b,0x85,0x01,0x0f,0x87,0x05,0x08,0x41,
0x81,0xe1,0x48,0x85,0x01,0x11,0x87,0x05,0x08,0x41,0x91,0xe1,0x48,0x85,0x01,0x13,
0x87,0x05,0x08,0x41,0xa1,0xe1,0x48,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,
0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x19,0x87,0x05,0x08,0x41,0xd1,0xe1,0x48,
0x85,0x01,0x1b,0x87,0x05,0x08,0x41,0xe1,0xe1,0x48,0x85,0x01,0x1d,0x87,0x05,0x08,
0x41,0xf1,0xe1,0x48,0x85,0x01,0x1f,0x87,0x05,0x08,0x42,0x00,0xef,0x9e,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x25,
0x87,0x05,0x08,0x42,0x18,0xef,0x9e,0x85,0x01,0x27,0x87,0x05,0x08,0x42,0x20,0xef,
0x9e,0x85,0x01,0x29,0x87,0x05,0x08,0x42,0x28,0xef,0x9e,0x85,0x01,0x2b,0x87,0x05,
0x08,0x42,0x30,0xef,0x9e,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,
0x17,0x86,0xa2,0x17,0x85,0x01,0x31,0x87,0x05,0x08,0x42,0x48,0xef,0x9e,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x37,
0x87,0x05,0x08,0x42,0x60,0xef,0x9e,0x85,0x01,0x39,0x87,0x05,0x08,0x42,0x68,0xef,
0x9e,0x85,0x01,0x3b,0x87,0x05,0x08,0x42,0x70,0xef,0x9e,0x85,0x01,0x3d,0x87,0x05,
0x08,0x42,0x78,0xef,0x9e,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,
0x17,0x86,0xa2,0x17,0x85,0x01,0x43,0x87,0x05,0x08,0x42,0x88,0x77,0xcf,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x49,
0x87,0x05,0x08,0x42,0x94,0x77,0xcf,0x85,0x01,0x4b,0x87,0x05,0x08,0x42,0x98,0x77,
0xcf,0x85,0x01,0x4d,0x87,0x05,0x08,0x42,0x9c,0x77,0xcf,0x85,0x01,0x4f,0x87,0x05,
0x08,0x42,0xa0,0x77,0xcf,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,
0x17,0x86,0xa2,0x17,0x85,0x01,0x55,0x87,0x05,0x08,0x42,0xac,0x77,0xcf,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x5b,
0x87,0x05,0x08,0x42,0xb8,0x77,0xcf,0x85,0x01,0x5d,0x87,0x05,0x08,0x42,0xbc,0x77,
0xcf,0x85,0x01,0x5f,0x87,0x05,0x08,0x42,0xc0,0x77,0xcf,0x85,0x01,0x61,0x87,0x05,
0x08,0x42,0xc4,0x77,0xcf,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,
0x17,0x86,0xa2,0x17,0x85,0x01,0x67,0x87,0x05,0x08,0x42,0xd0,0x78,0x52,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x6d,
0x87,0x05,0x08,0x42,0xdc,0x78,0x52,0x85,0x01,0x6f,0x87,0x05,0x08,0x42,0xe0,0x78,
0x52,0x85,0x01,0x71,0x87,0x05,0x08,0x42,0xe4,0x78,0x52,0x85,0x01,0x73,0x87,0x05,
0x08,0x42,0xe8,0x78,0x52,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,
0x17,0x86,0xa2,0x17,0x85,0x01,0x79,0x87,0x05,0x08,0x42,0xf4,0x78,0x52,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x38,0x85,0x01,0x7f,
0x87,0x05,0x08,0x43,0x00,0x3c,0x29,0x85,0x02,0x00,0x81,0x87,0x05,0x08,0x43,0x02,
0x3c,0x29,0x85,0x02,0x00,0x83,0x87,0x05,0x08,0x43,0x04,0x3c,0x29,0x85,0x02,0x00,
0x85,0x87,0x05,0x08,0x43,0x06,0x3c,0x29,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,
0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x03,0x87,0x05,0x08,0x40,0x87,0x7c,
0xee,0x85,0x01,0x05,0x87,0x05,0x08,0x40,0xc7,0x7c,0xee,0x85,0x01,0x07,0x87,0x05,
0x08,0x41,0x03,0xc2,0x8f,0x85,0x01,0x09,0x87,0x05,0x08,0x41,0x23,0xc2,0x8f,0x84,
0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0x87,0x05,0x08,0x40,
0x87,0x7c,0xee
};

static unsigned char goose_tunnel_packet[] = {
0x00,0x00,0x03,0x01,0x00,0x00,0x00,0x00,0x61,0x82,0x02,0xf5,0x80,0x0d,
0x6d,0x79,0x64,0x6f,0x6d,0x2f,0x6d,0x79,0x67,0x63,0x52,0x65,0x66,0x81,0x01,0x04,
0x82,0x0f,0x6d,0x79,0x64,0x6f,0x6d,0x2f,0x6d,0x79,0x64,0x61,0x74,0x61,0x73,0x65,
0x74,0x83,0x09,0x74,0x65,0x73,0x74,0x41,0x70,0x70,0x49,0x44,0x84,0x08,0x3f,0xfb,
0x32,0x4c,0x00,0x00,0x00,0x00,0x85,0x01,0x01,0x86,0x01,0x01,0x87,0x01,0x00,0x88,
0x01,0x20,0x89,0x01,0x00,0x8a,0x01,0x03,0xab,0x82,0x02,0xa7,0xa2,0x82,0x02,0x65,
0xa2,0x35,0x85,0x01,0x03,0x87,0x05,0x08,0x40,0x87,0x7c,0xee,0x85,0x01,0x05,0x87,
0x05,0x08,0x40,0xc7,0x7c,0xee,0x85,0x01,0x07,0x87,0x05,0x08,0x41,0x03,0xc2,0x8f,
0x85,0x01,0x09,0x87,0x05,0x08,0x41,0x23,0xc2,0x8f,0x84,0x03,0x00,0x55,0x55,0x8c,
0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x2b,0x85,0x01,0x0f,0x87,0x05,0x08,0x41,
0x81,0xe1,0x48,0x85,0x01,0x11,0x87,0x05,0x08,0x41,0x91,0xe1,0x48,0x85,0x01,0x13,
0x87,0x05,0x08,0x41,0xa1,0xe1,0x48,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,
0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x19,0x87,0x05,0x08,0x41,0xd1,0xe1,0x48,
0x85,0x01,0x1b,0x87,0x05,0x08,0x41,0xe1,0xe1,0x48,0x85,0x01,0x1d,0x87,0x05,0x08,
0x41,0xf1,0xe1,0x48,0x85,0x01,0x1f,0x87,0x05,0x08,0x42,0x00,0xef,0x9e,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x25,
0x87,0x05,0x08,0x42,0x18,0xef,0x9e,0x85,0x01,0x27,0x87,0x05,0x08,0x42,0x20,0xef,
0x9e,0x85,0x01,0x29,0x87,0x05,0x08,0x42,0x28,0xef,0x9e,0x85,0x01,0x2b,0x87,0x05,
0x08,0x42,0x30,0xef,0x9e,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,
0x17,0x86,0xa2,0x17,0x85,0x01,0x31,0x87,0x05,0x08,0x42,0x48,0xef,0x9e,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x37,
0x87,0x05,0x08,0x42,0x60,0xef,0x9e,0x85,0x01,0x39,0x87,0x05,0x08,0x42,0x68,0xef,
0x9e,0x85,0x01,0x3b,0x87,0x05,0x08,0x42,0x70,0xef,0x9e,0x85,0x01,0x3d,0x87,0x05,
0x08,0x42,0x78,0xef,0x9e,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,
0x17,0x86,0xa2,0x17,0x85,0x01,0x43,0x87,0x05,0x08,0x42,0x88,0x77,0xcf,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x49,
0x87,0x05,0x08,0x42,0x94,0x77,0xcf,0x85,0x01,0x4b,0x87,0x05,0x08,0x42,0x98,0x77,
0xcf,0x85,0x01,0x4d,0x87,0x05,0x08,0x42,0x9c,0x77,0xcf,0x85,0x01,0x4f,0x87,0x05,
0x08,0x42,0xa0,0x77,0xcf,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,
0x17,0x86,0xa2,0x17,0x85,0x01,0x55,0x87,0x05,0x08,0x42,0xac,0x77,0xcf,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x5b,
0x87,0x05,0x08,0x42,0xb8,0x77,0xcf,0x85,0x01,0x5d,0x87,0x05,0x08,0x42,0xbc,0x77,
0xcf,0x85,0x01,0x5f,0x87,0x05,0x08,0x42,0xc0,0x77,0xcf,0x85,0x01,0x61,0x87,0x05,
0x08,0x42,0xc4,0x77,0xcf,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,
0x17,0x86,0xa2,0x17,0x85,0x01,0x67,0x87,0x05,0x08,0x42,0xd0,0x78,0x52,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x6d,
0x87,0x05,0x08,0x42,0xdc,0x78,0x52,0x85,0x01,0x6f,0x87,0x05,0x08,0x42,0xe0,0x78,
0x52,0x85,0x01,0x71,0x87,0x05,0x08,0x42,0xe4,0x78,0x52,0x85,0x01,0x73,0x87,0x05,
0x08,0x42,0xe8,0x78,0x52,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,
0x17,0x86,0xa2,0x17,0x85,0x01,0x79,0x87,0x05,0x08,0x42,0xf4,0x78,0x52,0x84,0x03,
0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0xa2,0x38,0x85,0x01,0x7f,
0x87,0x05,0x08,0x43,0x00,0x3c,0x29,0x85,0x02,0x00,0x81,0x87,0x05,0x08,0x43,0x02,
0x3c,0x29,0x85,0x02,0x00,0x83,0x87,0x05,0x08,0x43,0x04,0x3c,0x29,0x85,0x02,0x00,
0x85,0x87,0x05,0x08,0x43,0x06,0x3c,0x29,0x84,0x03,0x00,0x55,0x55,0x8c,0x06,0x29,
0x32,0x2e,0x00,0x17,0x86,0xa2,0x35,0x85,0x01,0x03,0x87,0x05,0x08,0x40,0x87,0x7c,
0xee,0x85,0x01,0x05,0x87,0x05,0x08,0x40,0xc7,0x7c,0xee,0x85,0x01,0x07,0x87,0x05,
0x08,0x41,0x03,0xc2,0x8f,0x85,0x01,0x09,0x87,0x05,0x08,0x41,0x23,0xc2,0x8f,0x84,
0x03,0x00,0x55,0x55,0x8c,0x06,0x29,0x32,0x2e,0x00,0x17,0x86,0x87,0x05,0x08,0x40,
0x87,0x7c,0xee
};



//for unit testing only
extern void repeat_unit_teIEC_key_payloads(int num_repeats);
extern void repeat_unit_teIEC_igmp(int num_repeats, SOCKET rxSocket);



struct sv_thread_info{			  //just used to pass paramters to the demo SV transmit Thread
  KDC_REF *pMyKDCRef;
  char * pInterfaceID;
  IEC_COMM_ADDRESS *pSrcAddress;
  IEC_COMM_ADDRESS *pDestAddress;
  ETHERTYPE_8021Q Val8021Q;
  USHORT IP_TOS_DSCP;
  DWORD threadDelay;
  int *pCancelThreads;
};

struct parse_thread_info{
  IEC_90_5_RX *pMyRxCntrl;
  IEC_COMM_ADDRESS  *pDestAddr;
};

static int DoIt = TRUE;

static STARTUP_CFG cfg;
static FILE *StatisticFile=NULL;

/************************************************************************/
/* NOTE: tunnel_pdu_ptr points to GOOSE/SMPVAL PDU starting right AFTER	*/
/*   Ethertype ID. This would be easier if it pointed to the		*/
/*   Ethertype ID, but clnp_snet_read doesn't give us that.		*/
/************************************************************************/
unsigned char *example_tunnel_enc (IEC905_MSG_CNTRL *pMsgCntrl,
	unsigned char *dIEC_mac,		/* DST MAC (must be 6 bytes)	*/
	ST_UINT16 tci,			/* TCI from original message*/
	ST_UINT16 etype_id,	/* Ethertype ID from original message*/
	ST_UCHAR *etype_pdu_ptr,	/* pointer after Ethertype ID	*/
	ST_UINT16 etype_pdu_len,	/* len of PDU after Ethertype ID*/
	ST_UINT *enc_len_ptr)
  {

IEC905_SESS_PDU_HDR_INFO hdr_info;
IEC905_SESS_PAYLOAD_DEF payload_info;
unsigned char *temp_ptr;
unsigned char key_id[4];

iec905_manage_keys(pMsgCntrl);				//check about the keys
/* example of filling in the hearder  */
hdr_info.SessionIdentifer = IEC_90_5_SI_TYPE_TUNNEL;
hdr_info.hmacAlg = HMAC_ALG_SHA_256_128;;
hdr_info.secAlgType = SEC_ALG_NONE	;
hdr_info.pKey = pMsgCntrl->pCurrentKey->pKey;;
hdr_info.keyLen = pMsgCntrl->pCurrentKey->keyLen;
hdr_info.timeToNextKey= (ST_UINT16)(pMsgCntrl->pCurrentKey-> timeOfexpiration -time(NULL))/60;;
hdr_info.timeOfCurrentKey=(ST_UINT32)pMsgCntrl->pNextKey->timeOfInitialUse;;
memcpy(key_id,pMsgCntrl->pCurrentKey->key_id,4);
hdr_info.pKeyID = key_id;

/* fill in the payload_information  */
payload_info.payload_tag = TUNNEL_PAYLOAD_TYPE_TAG;
payload_info.appID = 0;
payload_info.pduLen = etype_pdu_len;
payload_info.pPDU = etype_pdu_ptr;
payload_info.etype_id = etype_id;
payload_info.simulationBit = 0x00;
payload_info.etype_id = etype_id;
payload_info.tci = tci;
payload_info.tpid = 0x00;
memcpy(&payload_info.dst_mac,dIEC_mac,6);
payload_info.next = NULL;

  temp_ptr = iec905_sess_enc(&hdr_info,&payload_info,enc_len_ptr,&pMsgCntrl->spdu_num_tx);
  return (temp_ptr);
  }


/************************************************************************/
/* NOTE: tunnel_pdu_ptr points to GOOSE/SMPVAL PDU starting right AFTER	*/
/*   Ethertype ID. This would be easier if it pointed to the		*/
/*   Ethertype ID, but clnp_snet_read doesn't give us that.		*/
/************************************************************************/
unsigned char *example_GOOSE_enc (IEC905_MSG_CNTRL *pMsgCntrl,
	ST_UINT16 enc_pdu_len,	
	ST_UCHAR *enc_ptr,
	ST_UINT *enc_len_ptr)
  {

IEC905_SESS_PDU_HDR_INFO hdr_info;
IEC905_SESS_PAYLOAD_DEF payload_info;
IEC905_SESS_PAYLOAD_DEF payload_info1;
unsigned char *temp_ptr;
unsigned char key_id[4];

iec905_manage_keys(pMsgCntrl);				//check about the keys
/* example of filling in the hearder  */
hdr_info.SessionIdentifer = IEC_90_5_SI_TYPE_GOOSE;
hdr_info.hmacAlg = HMAC_ALG_SHA_256_128;;
hdr_info.secAlgType = SEC_ALG_NONE	;
hdr_info.pKey = pMsgCntrl->pCurrentKey->pKey;
hdr_info.keyLen = pMsgCntrl->pCurrentKey->keyLen;
hdr_info.timeToNextKey= (ST_UINT16)(pMsgCntrl->pCurrentKey-> timeOfexpiration -time(NULL))/60;
hdr_info.timeOfCurrentKey=(ST_UINT32)pMsgCntrl->pNextKey->timeOfInitialUse;
memcpy(key_id,pMsgCntrl->pCurrentKey->key_id,4);
hdr_info.pKeyID = key_id;

/* fill in the payload_information  */
payload_info.payload_tag = GOOSE_PAYLOAD_TYPE_TAG;
payload_info.appID = 0;
payload_info.pduLen = enc_pdu_len;
payload_info.pPDU = enc_ptr;
payload_info.simulationBit = 0x00;
payload_info.next = NULL;


payload_info1.payload_tag = GOOSE_PAYLOAD_TYPE_TAG;	    //example of how to send multiple payloads
payload_info1.appID = 0;
payload_info1.pduLen = enc_pdu_len;
payload_info1.pPDU = enc_ptr;
payload_info1.simulationBit = 0x00;
payload_info1.next = NULL;
payload_info.next = &payload_info1;






  temp_ptr = iec905_sess_enc(&hdr_info,&payload_info,enc_len_ptr,&pMsgCntrl->spdu_num_tx);
  return (temp_ptr);
  }


/************************************************************************/
/* NOTE: tunnel_pdu_ptr points to GOOSE/SMPVAL PDU starting right AFTER	*/
/*   Ethertype ID. This would be easier if it pointed to the		*/
/*   Ethertype ID, but clnp_snet_read doesn't give us that.		*/
/************************************************************************/
unsigned char *example_SV_enc (IEC905_MSG_CNTRL *pMsgCntrl,
	ST_UINT16 enc_pdu_len,	
	ST_UCHAR *enc_ptr,
	ST_UINT *enc_len_ptr)
  {

IEC905_SESS_PDU_HDR_INFO hdr_info;
IEC905_SESS_PAYLOAD_DEF payload_info;
unsigned char *temp_ptr;
unsigned char key_id[4];

iec905_manage_keys(pMsgCntrl);				//check about the keys
/* example of filling in the hearder  */
hdr_info.SessionIdentifer = IEC_90_5_SI_TYPE_SV;
hdr_info.hmacAlg = HMAC_ALG_SHA_256_128;;
hdr_info.secAlgType = SEC_ALG_NONE	;
hdr_info.pKey = pMsgCntrl->pCurrentKey->pKey;
hdr_info.keyLen = pMsgCntrl->pCurrentKey->keyLen;
hdr_info.timeToNextKey=  (ST_UINT16)(pMsgCntrl->pCurrentKey-> timeOfexpiration -time(NULL))/60;
hdr_info.timeOfCurrentKey= (ST_UINT32)pMsgCntrl->pNextKey->timeOfInitialUse;

memcpy(key_id,pMsgCntrl->pCurrentKey->key_id,4);
hdr_info.pKeyID = key_id;

/* fill in the payload_information  */
payload_info.payload_tag = SV_PAYLOAD_TYPE_TAG ;
payload_info.appID = 0;
payload_info.pduLen = enc_pdu_len;
payload_info.pPDU = enc_ptr;
payload_info.simulationBit = 0x00;
payload_info.next = NULL;

  temp_ptr = iec905_sess_enc(&hdr_info,&payload_info,enc_len_ptr, &pMsgCntrl->spdu_num_tx);
  return (temp_ptr);
  }





static int SoftwareInitalizationSequence()
{
int retVal;
KDC_CREDENTIALS *pMyLocalCredentials;

    iec905_init_key_storage();				//no error is returned

  //initialize ip socket interface
  if((retVal = iec905_ip_init())!=SUCCESS_IEC905)				
    {
    IEC905_ErrorHandler (retVal, __FILE__, __LINE__);
    return(retVal);
    }
  
  //create a local credential structure
  if((pMyLocalCredentials = iec905_create_kdc_credential())==NULL)
    {
    IEC905_ErrorHandler (FAILURE_IEC905, __FILE__, __LINE__);
    return(FAILURE_IEC905);
    }
		
  //place holder for filling in the actual credentials when that is all worked out

  //now initialize the interface to the KDC with the local credentials

  if((retVal = iec905_init_kdc_interface(pMyLocalCredentials,10))!=SUCCESS_IEC905)
    {
    IEC905_ErrorHandler (retVal, __FILE__, __LINE__);
    return(retVal);
    }
 
  //the local credentials can no be freed since the init function creates an
  //internal copy of the information

   if((retVal=iec905_tx_pdu_init())!=SUCCESS_IEC905)
    {
    IEC905_ErrorHandler (retVal, __FILE__, __LINE__);
    return(retVal);
    }


  return(SUCCESS_IEC905);
}



//sample transmit sequence for Sampled Values in a thread
static void transmit_sequence_sv(void *arg)
{


struct sv_thread_info *pMyThreadInfo = (struct sv_thread_info *)arg;
KDC_REF *pMyKDCRef;
IEC905_MSG_CNTRL *pMyCntrl;
IECSOCKET_TX_INFO *mysockInfo=NULL;
SV_ENC_STRUCT *mySVInfo;
int retVal;
int offset_of_data,i,j;
unsigned char * pRetBuf;
int total_size;
int ret_len;
unsigned char *asdu_data; 
unsigned int y,z=0;



  
  pMyKDCRef = pMyThreadInfo->pMyKDCRef;
//transmit sequence 

  //if NULL is returned that there is a matching control structure that is already in use.  
  //Duplicate Control structures are not allowed
   if((pMyCntrl=iec905_create_msg_cntrl_tx( IEC_KEY_USAGE_TYPE_SV, pMyThreadInfo->pSrcAddress, pMyThreadInfo->pDestAddress, "SISCOExample/MYSVDataSet"))==NULL)
	 return;

  //if success is not returned, indicates that the KDC Reference is bad
  if((retVal=iec905_kdc_bind(pMyCntrl,pMyKDCRef))!=SUCCESS_IEC905)	  
    {
    //things that are created, must be destroyed.
    iec905_destroy_msg_cntrl(pMyCntrl);
    return;
    }

  //if success is not returned, indicates that keys could not be obtained.
  if((retVal=iec905_get_kdc_tx_keys(pMyCntrl))!=SUCCESS_IEC905)
    {
    //things that are created, must be destroyed.
    iec905_destroy_msg_cntrl(pMyCntrl);
    return;
    }


  //   open_tx_udp_socket_with_priorities(InterfaceID ,&Val8021Q, &IP_TOS_DSCP, &keyAddress);
  if((retVal=iec905_tx_pdu_open_socket_with_priorities(&mysockInfo,pMyThreadInfo->pInterfaceID ,&pMyThreadInfo->Val8021Q, &pMyThreadInfo->IP_TOS_DSCP, pMyThreadInfo->pDestAddress))!=SUCCESS_IEC905)
      {
    IEC905_ErrorHandler (retVal, __FILE__, __LINE__);
	//things that are created, must be destroyed.
    iec905_destroy_msg_cntrl(pMyCntrl);

    return;
    }
 

  
  //the sending methodolgy is to encode a packet and then call the transmit function

  //calculate the size of the samples in the ASDU (example shows 10 SV_INT32 values
  total_size = 0;

#define NUMBER_OF_SV_ULONGS 256
  for(i = 0;i<NUMBER_OF_SV_ULONGS;++i)
  {
    offset_of_data= sv_data_helper_calculate_samples_size_prim (SV_INT32, 0, &ret_len, &total_size);
  }

#define NUM_ASDUS_TO_DEFINE_MAX 3

  mySVInfo= initialize_sv_encode_struct ("ExampleSVID", "SISCOExample/MYSVDataSet",NUM_ASDUS_TO_DEFINE_MAX ,1,0x1,16,1,total_size);

  asdu_data = mySVInfo->asdus[0].pASDU_data;
  offset_of_data= 0;
  j=0;

  while(*(pMyThreadInfo->pCancelThreads) == FALSE)
  {
    offset_of_data = 0;
    for(i=0;i<NUMBER_OF_SV_ULONGS;++i)
    { 
      *(ST_UINT32 *)(asdu_data+offset_of_data) = htonl(j);//need to put things in the buffer in network order
      ++j;
      offset_of_data= sv_data_helper_calculate_samples_size_prim (SV_INT32, offset_of_data, &ret_len, &total_size);
    }

#if 0
    y = z %2;//change the number of ASDUs
    sv_enc_update_lengths( mySVInfo,++y);		  
    ++z;
#endif

    //transmit 2 ASDUs for the UDP test
    sv_enc_update_lengths( mySVInfo,2);

    pRetBuf = example_SV_enc (pMyCntrl, mySVInfo->length_to_send,mySVInfo->pSVbuffer, &ret_len);
  
     if(pRetBuf!=NULL)
       {
       iec905_tx_pdu (mysockInfo,pMyThreadInfo->pDestAddress, 102,pRetBuf,ret_len,pMyCntrl);
       free(pRetBuf);
	   }
	 else
	   {
	   //things that are created, must be destroyed.
       iec905_destroy_msg_cntrl(pMyCntrl);
       iec905_tx_pdu_close_socket(mysockInfo);
       return;
	   }
//    printf("@");
     Sleep((cfg.transmissionIntervalMsec+pMyThreadInfo->threadDelay));
  }

  iec905_tx_pdu_close_socket(mysockInfo);
}


//sample transmit sequence for a GOOSE in a thread
static void transmit_sequence_goose(void *arg)
{
struct sv_thread_info *pMyThreadInfo = (struct sv_thread_info *)arg;
KDC_REF *pMyKDCRef;
IEC905_MSG_CNTRL *pMyCntrl;
IECSOCKET_TX_INFO *mysockInfo=NULL;
int retVal;
unsigned char * pRetBuf;
int ret_len;

  
  pMyKDCRef = pMyThreadInfo->pMyKDCRef;
//transmit sequence 

  //if NULL is returned that there is a matching control structure that is already in use.  
  //Duplicate Control structures are not allowed
  if((pMyCntrl=iec905_create_msg_cntrl_tx( IEC_KEY_USAGE_TYPE_GOOSE, pMyThreadInfo->pSrcAddress, pMyThreadInfo->pDestAddress, "SISCOExample/MYGOOSEDataSet"))==NULL)
    return;

  //if success is not returned, indicates that the KDC Reference is bad
  if((retVal=iec905_kdc_bind(pMyCntrl,pMyKDCRef))!=SUCCESS_IEC905)
    {
    //things that are created, must be destroyed.
    iec905_destroy_msg_cntrl(pMyCntrl);
    return;
    }
    

  //if success is not returned, indicates that keys could not be obtained.
  if((retVal=iec905_get_kdc_tx_keys(pMyCntrl))!=SUCCESS_IEC905)
    {
    //things that are created, must be destroyed.
    iec905_destroy_msg_cntrl(pMyCntrl);
    return;
    }


  //   open_tx_udp_socket_with_priorities(InterfaceID ,&Val8021Q, &IP_TOS_DSCP, &keyAddress);
  if((retVal=iec905_tx_pdu_open_socket_with_priorities(&mysockInfo,pMyThreadInfo->pInterfaceID ,&pMyThreadInfo->Val8021Q, &pMyThreadInfo->IP_TOS_DSCP, pMyThreadInfo->pDestAddress))!=SUCCESS_IEC905)
      {
    IEC905_ErrorHandler (retVal, __FILE__, __LINE__);
    //things that are created, must be destroyed.
    iec905_destroy_msg_cntrl(pMyCntrl);
    return;
    }
 

  while(*(pMyThreadInfo->pCancelThreads) == FALSE)
  {

    pRetBuf = example_GOOSE_enc (pMyCntrl, sizeof(goose_packet),goose_packet, &ret_len);

    if(pRetBuf!=NULL)
      {
      iec905_tx_pdu (mysockInfo,pMyThreadInfo->pDestAddress, 102,pRetBuf,ret_len,pMyCntrl);
      free(pRetBuf);
      }
    else
      {
      //things that are created, must be destroyed.
       iec905_destroy_msg_cntrl(pMyCntrl);
       iec905_tx_pdu_close_socket(mysockInfo);
       return;
      }

    Sleep((cfg.transmissionIntervalMsec+pMyThreadInfo->threadDelay));
  }

  iec905_tx_pdu_close_socket(mysockInfo);
}

//sample transmit sequence for a Tunnelled GOOSE in a thread
static void transmit_sequence_tunnelled_goose(void *arg)
{
struct sv_thread_info *pMyThreadInfo = (struct sv_thread_info *)arg;
KDC_REF *pMyKDCRef;
IEC905_MSG_CNTRL *pMyCntrl;
IECSOCKET_TX_INFO *mysockInfo=NULL;
int retVal;
unsigned char * pRetBuf;
int ret_len;
ST_UCHAR teIEC_mac[6] = {0x01,0x33,0x34,0x35,0x36,0x37};




  pMyKDCRef = pMyThreadInfo->pMyKDCRef;
//transmit sequence 

  //if NULL is returned that there is a matching control structure that is already in use.  
  //Duplicate Control structures are not allowed
  if((pMyCntrl=iec905_create_msg_cntrl_tx(IEC_KEY_USAGE_TYPE_TUNNEL, pMyThreadInfo->pSrcAddress, pMyThreadInfo->pDestAddress, "SISCOExample/MYGOOSEDataSet"))==NULL)
	return;

  //if success is not returned, indicates that the KDC Reference is bad
  if((retVal=iec905_kdc_bind(pMyCntrl,pMyKDCRef))!=SUCCESS_IEC905)
    {
    //things that are created, must be destroyed.
    iec905_destroy_msg_cntrl(pMyCntrl);
    return;
    }

  //if success is not returned, indicates that keys could not be obtained.
  if((retVal=iec905_get_kdc_tx_keys(pMyCntrl))!=SUCCESS_IEC905)
    {
    //things that are created, must be destroyed.
    iec905_destroy_msg_cntrl(pMyCntrl);
    return;
    }


  //   open_tx_udp_socket_with_priorities(InterfaceID ,&Val8021Q, &IP_TOS_DSCP, &keyAddress);
  if((retVal=iec905_tx_pdu_open_socket_with_priorities(&mysockInfo,pMyThreadInfo->pInterfaceID ,&pMyThreadInfo->Val8021Q, &pMyThreadInfo->IP_TOS_DSCP, pMyThreadInfo->pDestAddress))!=SUCCESS_IEC905)
      {
    IEC905_ErrorHandler (retVal, __FILE__, __LINE__);  
    //things that are created, must be destroyed.
    iec905_destroy_msg_cntrl(pMyCntrl);
    return;
    }
 

  
  //the sending methodolgy is to encode a packet and then call the transmit function

  //calculate the size of the samples in the ASDU (example shows 10 SV_INT32 values


  while(*(pMyThreadInfo->pCancelThreads) == FALSE)
  {

    pRetBuf= example_tunnel_enc(pMyCntrl,teIEC_mac,0x3333,0x88b8,goose_tunnel_packet,sizeof(goose_tunnel_packet),&ret_len);

    if(pRetBuf!=NULL)
      {
      iec905_tx_pdu (mysockInfo,pMyThreadInfo->pDestAddress, 102,pRetBuf,ret_len,pMyCntrl);
      free(pRetBuf);
	  }
	else
	  {
      //things that are created, must be destroyed.
       iec905_destroy_msg_cntrl(pMyCntrl);
       iec905_tx_pdu_close_socket(mysockInfo);
       return;
      }
 //   printf("+");
    Sleep((cfg.transmissionIntervalMsec+pMyThreadInfo->threadDelay));
  }

  iec905_tx_pdu_close_socket(mysockInfo);
}

static void sample_usr_sv_decode(ST_UINT16 len, ST_UCHAR *pPdu, IEC_90_5_RX *pMyRxInfo)
{
    //inputs the length of the data and the start of the data pointer.
    //note, the pPdu, in the payload is not allocated, it is just a pointer
    //into the decoded buffer. The RXInfo pointer is provided so that the
    //user can destroy strcuture, allocated payloads, and the allocated buffer that held the 90-5 packet
    //when the use of the information is complete. 

SV_DEC_STRUCT *pSVinfo;		    //the call to the decode function allocates this, the user may free it
				    //when done.  It may not be used (due to pointers into the pMyRxInfo buffer)
				    //after the pMyRxInfo is destroyed
ASDU_DEC_INFO *pASDUinfo;	    //there may be multple ASDUs in the sv packet, will only deal with the 1st one.
				    //the actual number of ASDUs is found in pSVInfo->num_ASDUs.
unsigned char *pNullTerminate;
   

  if((pSVinfo=sv_decode(pPdu,len))!=NULL)   //decode was successful and can use the information
    {					    //for demonstration purposes, will just print out the dataSet information
	pASDUinfo = &pSVinfo->asdus[0];
        pNullTerminate = pASDUinfo->datSetRef.pValue + pASDUinfo->datSetRef.len;
	*pNullTerminate = 0;		    //know that the next byte in the buffer is a tag, so just overwrite it with a zero
//	printf("%s",pASDUinfo->datSetRef.pValue);

	//remember other values are in the buffer in network order so must be converted appropriately

	//the user can hold on to the Ctnrl and decoded SV information for as long as they want
	//when done free the pSVinfo and destroy the Ctnrl
	free(pSVinfo);
     }


}

/*****************************************************************************/
/*		usr_iec905_rx	  					     */
/* Sample user function to receive the IEC90-5 information that has been     */
/* received and then performs the APDU decode (only SV support currently)    */
/*****************************************************************************/
static void sample_usr_iec905_rx(IEC_90_5_RX *pMyRxInfo)
{
IEC905_SESS_PAYLOAD_DEF *payload;		
		
		

  payload = pMyRxInfo->pPayload;
  while(payload)		  //need to loop through the entire chaing of payloads and decode
  {
    switch(payload->payload_tag)
      {
      case SV_PAYLOAD_TYPE_TAG:	  //actually have a SV decoder provided
	sample_usr_sv_decode(payload->pduLen, payload->pPDU, pMyRxInfo);
        break;

      case GOOSE_PAYLOAD_TYPE_TAG:  //currently don't have a decoder
      case TUNNEL_PAYLOAD_TYPE_TAG:  //currently don't have a decoder
      case MNGT_PAYLOAD_TYPE_TAG:   //currently don't have a decoder
      default:
	break;
      }
    payload=payload->next;
  }

  //destroys the strcuture, allocated payloads, and the allocated buffer that held the 90-5 packet.
  iec905_destroy_dec_info( pMyRxInfo);	
}


static void parse_function(IEC_90_5_RX *pMyRxCntrl,IEC_COMM_ADDRESS  *pDestAddr)
{
int retVal;

          if((retVal =iec905_udp_dec(pMyRxCntrl ,pDestAddr))==SUCCESS_IEC905)	 		    /*points to the rxd information	*/
            sample_usr_iec905_rx(pMyRxCntrl);
	  else 
            iec905_destroy_dec_info(  pMyRxCntrl);
}

static void parse_thread(void *args)
{
struct parse_thread_info *pInfo = (struct parse_thread_info *)args; 

    if(pInfo!=NULL)
      {
      parse_function(pInfo->pMyRxCntrl,pInfo->pDestAddr);
      free(pInfo);
      }
}

/*****************************************************************************/
/*		sample_rxd_thread					      */
/*****************************************************************************/
static void sample_rxd_thread(void *args)
{

int fromlen, byte_count;
struct sockaddr addr;
struct sockaddr_in *pIPv4Addr;
unsigned char *pDecodeBuffer;
IEC_90_5_RX *pMyRxCntrl;
int nfds,retVal;
fd_set readfds;
IEC_COMM_ADDRESS destAddr;
IEC_COMM_ADDRESS *pDestAddr;
#define MAX_RXD_WAIT_BEFORE_WAKEUP 5000 //usec
struct timeval timeout;
struct parse_thread_info *parseInfo;

#define MAX_UDP_BUF_SIZE 64000
static unsigned char buf[MAX_UDP_BUF_SIZE];

timeout.tv_sec = 0;
timeout.tv_usec = 5000;

  //initalize the UDP receive socket (102) for 90-5 and store the results in the passed Socket Pointer

  if((retVal = iec905_rx_init(&IEC_90_5_rx_socket, cfg.updScktBufSize))!=SUCCESS_IEC905)		
    {
    IEC905_ErrorHandler (retVal, __FILE__, __LINE__);
    return;
    }


     FD_ZERO (&readfds);
     FD_SET (IEC_90_5_rx_socket, &readfds);
     /* Wait forever for incoming UDP packet.    */
#if defined(_WIN32)
     nfds = 1;    /* On _WIN32, ignored, but compiler wants it 
initialized.*/
#else
     nfds = ReceivingSocket+1;    /* ReceivingSocket is only fds to check, so set "nfds=ReceivingSocket+1".*/
#endif
   while (1)
     {
     nfds = select (nfds, &readfds, NULL, NULL, NULL);    /* wait forever    */
     if (nfds > 0)
       {    /* UDP packet available on the socket. Receive it.    */
       //get the length of the packet waiting
       fromlen = sizeof(addr);
#if 0
      //line removed due to performance issues
       byte_count = recvfrom(IEC_90_5_rx_socket,buf, MAX_UDP_BUF_SIZE,MSG_PEEK, &addr, &fromlen);
#else
      //instead of a peek and then read, for performance reasons read, then allocate a buffer and then memory copy
      byte_count = recvfrom(IEC_90_5_rx_socket,buf, MAX_UDP_BUF_SIZE,0, &addr, &fromlen);
#endif
       if(byte_count>0)
	 {
         pDecodeBuffer = malloc(byte_count);
#if 0
	 //line removed due to performance issues
         byte_count = recvfrom(IEC_90_5_rx_socket,pDecodeBuffer, byte_count,0, &addr, &fromlen);
#else
	 memcpy(pDecodeBuffer,buf,byte_count);
#endif

         pMyRxCntrl = calloc(1, sizeof(IEC_90_5_RX));
         pMyRxCntrl->lenRXDBuffer = byte_count;
         pMyRxCntrl->pRXDbuffer = pDecodeBuffer;

#if 1	//use if want to have blocking parses
        //will need to update to support IPv6 eventually     
        if(addr.sa_family==AF_INET)		//then this is IPv4
	  {
          pIPv4Addr = (struct sockaddr_in *)&addr;
          destAddr.typeOfAddress= IP_V4_ADDRESS_TYPE;
	  destAddr.lenOfAddress =4;
          destAddr.pAddress = (unsigned char *)&pIPv4Addr->sin_addr.S_un.S_addr;
	  }

	  parse_function(pMyRxCntrl,&destAddr);
#else
	if(addr.sa_family==AF_INET)
	  {
	  pDestAddr = calloc(1,sizeof(IEC_COMM_ADDRESS));
	  if(pDestAddr!=NULL)
	    {
            pIPv4Addr = (struct sockaddr_in *)&addr;
            pDestAddr->typeOfAddress= IP_V4_ADDRESS_TYPE;
	    pDestAddr->lenOfAddress =4;
            pDestAddr->pAddress = (unsigned char *)&pIPv4Addr->sin_addr.S_un.S_addr;
	    parseInfo = malloc(sizeof(struct parse_thread_info));
	    parseInfo->pMyRxCntrl = pMyRxCntrl;
	    parseInfo->pDestAddr = pDestAddr;
	    _beginthread(parse_thread,0, (void *) parseInfo);
	    }
	  else
	    {
	    free(pDecodeBuffer);
	    free(pMyRxCntrl);
	    }
	  }
        else
	  {
	  free(pDecodeBuffer);
	  free(pMyRxCntrl);
	  }
#endif

#if  0
          if((retVal =iec905_udp_dec(pMyRxCntrl ,&destAddr))==SUCCESS_IEC905)	 		    /*points to the rxd information	*/
            sample_usr_iec905_rx(pMyRxCntrl);
	  else 
            iec905_destroy_dec_info(  pMyRxCntrl);
#endif

	}
//       printf(".");
      }
    }
}


static void get_stats(void *args)
{
IEC905_STATISTICS stats;
time_t reset;
#define DEFAULT_TIME_DELAY_TO_RESET 3600 //1 hour
int elapsedTimeToReset=DEFAULT_TIME_DELAY_TO_RESET; 
time_t timeToLog;
unsigned int logInterval=0;
unsigned int logCnt=0;


 if((StatisticFile!=NULL) && (cfg.logIntervalSeconds>0))
    {
    logInterval = cfg.logIntervalSeconds;
    fprintf(StatisticFile,"TotalTxPktCnt,TotalRxPktCnt,TotalRxMissingPktCnt,TotalRxPktWithBadHMAC\n");
    timeToLog = time(NULL) + logInterval; 
    }


if(cfg.statResetTime!=0)
  elapsedTimeToReset = cfg.statResetTime;

reset = time(NULL) + elapsedTimeToReset;

 while(1)
  {

   if(time(NULL)>=reset)
    {
    iec905_getStats(&stats, TRUE);
    reset = time(NULL)+elapsedTimeToReset;
    }
   else
        iec905_getStats(&stats, FALSE);

  printf("\n\n*************** STATS  **************************\n");
  printf("Total Transmitted Packets: %lu\n",stats.TotalTxPktCnt);
  printf("Total Packets Received: %lu\n",stats.TotalRxPktCnt);
  printf("Total Missing Pakcets: %lu\n",stats.TotalRxMissingPktCnt);
  printf("Total Bad HMAC Packets: %lu\n",stats.TotalRxPktWithBadHMAC);
  printf("Number of records exported: %lu\n",logCnt);
  printf("*************************************************\n");

  if((time(NULL) > timeToLog) && (logInterval!=0))
    {
    fprintf(StatisticFile,"%lu,%lu,%lu,%lu\n",stats.TotalTxPktCnt,stats.TotalRxPktCnt,stats.TotalRxMissingPktCnt,stats.TotalRxPktWithBadHMAC);
    timeToLog = time(NULL) + logInterval;
    ++logCnt;
    }
  Sleep(5000);
  }
}

//Function to catch Cntrl-C
ST_VOID ctrlCfun (int i)
   {
   DoIt = FALSE;
   }

/************************************************************************/
/*                       main						*/
/************************************************************************/
int main (int argc, char *argv[])
  {




ST_UCHAR ipAddr1[] = {0x10,0x32,0x33,0x01};
ST_UCHAR ipAddr2[] = {0x10,0x32,0x33,0x02};
char *DataSetRef1  = "SISCO_IED/DataSet1";
char InterfaceID[100];
ETHERTYPE_8021Q Val8021Q;
USHORT IP_TOS_DSCP=46;
IECSOCKET_TX_INFO *mysockInfo=NULL;
int retVal,i;
KDC_CREDENTIALS *pKDCCredential1;
KDC_CREDENTIALS *pKDCCredential2;
KDC_REF *pRetKDCRef;
IEC_COMM_ADDRESS kdcAddress;	      //doesn't have anything in it, but is used to show the process
struct sv_thread_info mySVThreadInfo;
struct sv_thread_info myGOOSEThreadInfo;
struct sv_thread_info myTunnelledGOOSEThreadInfo;
int cancelThreads=FALSE;
IEC905_MSG_CNTRL *pMyRxCntrl;
IEC_COMM_ADDRESS *pKeyAddress;
DWORD totalDelay=0;
UINT    wTimerRes, wMaxTimerRes;  // timer resolturion
UINT    wTimerID;  // timer ID
TIMECAPS  timecaps;  // needed by timeGetDevCaps
uintptr_t threadHandle;
DWORD windowsLastError;

  setbuf (stdout, NULL);    /* do not buffer the output to stdout   */
  setbuf (stderr, NULL);    /* do not buffer the output to stderr   */

//do this near top of "main". I don't remember if you need both, but it shouldn't hurt.
/* Set the ^c catcher */
   signal (SIGINT, ctrlCfun);
//   signal (SIGABRT, ctrlCfun);

#define DEFAULT_TRANS_TIME 30

  cfg.transmissionIntervalMsec = DEFAULT_TRANS_TIME;
  startup_cfg_read ("startup.cfg",&cfg);

  if(cfg.logIntervalSeconds!=0)
    StatisticFile = fopen("statistics.log","w+");
 
  strcpy( InterfaceID,cfg.interaceID);


 
  Val8021Q.priority =4;
  Val8021Q.vlanID =4; 

//set the precision of the sleep timer
   if ( timeGetDevCaps( &timecaps, sizeof( TIMECAPS ) ) == TIMERR_NOERROR )
    {
    // get optimal resolution
    wTimerRes = max( timecaps.wPeriodMin, 1);

    // set minimal res for our timer
    if( timeBeginPeriod( 1 ) != TIMERR_NOERROR )
     printf("timeBeginPeriod Error\n");
    else
     printf("Min Timer Resolution: %ld\n", wTimerRes);
     }
   else
     printf("timeGetDevCaps Error\n");



//the following section is the normal set of initialization call

  //initialize key storage before the receive socket so that everything is
  //OK should a packet come in once the receive Socket is set up
  if((retVal = SoftwareInitalizationSequence())!=SUCCESS_IEC905)
    {
      printf("Could not initialize software\n");
      printf(" Error: %s\n",IEC905_XlatErrorCode(retVal));
    }

  //before transmitting, need to ope a transmit socket.  In this case, we need to do
  //something a little special in order to allow priorities to be set in Windows and Linux
  //thus a intervening function has been provided



//KDC registration process *************************************************************

  pKDCCredential1 = iec905_create_kdc_credential();
  pKDCCredential2 = iec905_create_kdc_credential();

  //this would typically be the spot to fill in the credentials, but have none currently

  //we will use the dummy addressing for the KDC pair
  pRetKDCRef = iec905_create_KDC_pair(NULL, &kdcAddress,pKDCCredential1 ,&kdcAddress,pKDCCredential2);
  

  //spawn the receive thread - no arguments are needed
  threadHandle=_beginthread(sample_rxd_thread,0, (void *) NULL);


// can compile in the following section if a higher thread priority is needed.
  if(threadHandle!=-1L)
    {
     retVal= SetThreadPriority((HANDLE)threadHandle,THREAD_PRIORITY_TIME_CRITICAL);
//     retVal= SetThreadPriority((HANDLE)threadHandle,THREAD_PRIORITY_ABOVE_NORMAL);
    if(!retVal)
      windowsLastError = GetLastError();
    }



//now can add the subscriptions
    while((IEC_90_5_rx_socket==0) && (DoIt==TRUE))    //need to wait a bit to let the rxd thread initial the receive socket
      Sleep(5000);

    if(IEC_90_5_rx_socket!=0)
      {
      pMyRxCntrl = iec905_igmpv3_group_enroll(IEC_KEY_USAGE_TYPE_SV,  IP_V4_ADDRESS_TYPE, cfg.subscriptions[0].subAddress, cfg.subscriptions[0].srcAddress, 
			    cfg.subscriptions[0].dataSetRef, IEC_90_5_rx_socket);
     if(pMyRxCntrl)
      {
      retVal=iec905_kdc_bind(pMyRxCntrl,pRetKDCRef );
      if(retVal==SUCCESS_IEC905)
         retVal = iec905_get_kdc_tx_keys(pMyRxCntrl);
      }
    }


  //spawn a thread to continously transmit a SV packet
  if(strlen(cfg.destIPAddressSMV)!=0)
    {
    if((pKeyAddress= create_address_structure(IP_V4_ADDRESS_TYPE, cfg.destIPAddressSMV))!=NULL)
      {
      mySVThreadInfo.pMyKDCRef = pRetKDCRef;
      mySVThreadInfo.pSrcAddress = pKeyAddress;
      mySVThreadInfo.pDestAddress = pKeyAddress;
      mySVThreadInfo.pCancelThreads = &cancelThreads;
      mySVThreadInfo.pInterfaceID = InterfaceID;
      mySVThreadInfo.Val8021Q= Val8021Q;
      mySVThreadInfo.IP_TOS_DSCP=IP_TOS_DSCP;
      mySVThreadInfo.threadDelay = 0;
      totalDelay = cfg.threadStrtDelay;

     _beginthread(transmit_sequence_sv,0, (void *) &mySVThreadInfo);
     }
    else
      printf("SMV Publication Address Could not be converted\n");
    }
  else
     printf("SMV Publication Address not configured\n");   


  if(strlen(cfg.destIPAddressGOOSE)!=0)
    {
    if((pKeyAddress= create_address_structure(IP_V4_ADDRESS_TYPE, cfg.destIPAddressGOOSE))!=NULL)
      {
      //spawn a thread to continously transmit a GOOSE packet
      myGOOSEThreadInfo.pMyKDCRef = pRetKDCRef;
      myGOOSEThreadInfo.pSrcAddress = pKeyAddress;
      myGOOSEThreadInfo.pDestAddress = pKeyAddress;
      myGOOSEThreadInfo.pCancelThreads = &cancelThreads;
      myGOOSEThreadInfo.pInterfaceID = InterfaceID;
      myGOOSEThreadInfo.Val8021Q= Val8021Q;
      myGOOSEThreadInfo.IP_TOS_DSCP=IP_TOS_DSCP;
      myGOOSEThreadInfo.threadDelay = totalDelay;
      totalDelay += cfg.threadStrtDelay;


      _beginthread(transmit_sequence_goose,0, (void *) &myGOOSEThreadInfo);
      }
    else
      printf("GOOSE Publication Address Could not be converted\n");
    }
  else
     printf("GOOSE Publication Address not configured\n");



  if(totalDelay)
    Sleep(10);

  if(strlen(cfg.destIPAddressTunnell)!=0)
    {
    if((pKeyAddress= create_address_structure(IP_V4_ADDRESS_TYPE, cfg.destIPAddressTunnell))!=NULL)
      {
     //spawn a thread to continously transmit a Tunnelled GOOSE packet
      myTunnelledGOOSEThreadInfo.pMyKDCRef = pRetKDCRef;
      myTunnelledGOOSEThreadInfo.pSrcAddress = pKeyAddress;
      myTunnelledGOOSEThreadInfo.pDestAddress = pKeyAddress;
      myTunnelledGOOSEThreadInfo.pCancelThreads = &cancelThreads;
      myTunnelledGOOSEThreadInfo.pInterfaceID = InterfaceID;
      myTunnelledGOOSEThreadInfo.Val8021Q= Val8021Q;
      myTunnelledGOOSEThreadInfo.IP_TOS_DSCP=IP_TOS_DSCP;
      myTunnelledGOOSEThreadInfo.threadDelay= totalDelay;

      _beginthread(transmit_sequence_tunnelled_goose,0, (void *) &myTunnelledGOOSEThreadInfo);
      }
    else
      printf("Tunnel Publication Address Could not be converted\n");
    }
  else
     printf("Tunnel Publication Address not configured\n");


    while((IEC_90_5_rx_socket==0) && (DoIt==TRUE))    //need to wait a bit to let the rxd thread initial the receive socket
      Sleep(5000);

    if(IEC_90_5_rx_socket!=0)
      {
      for(i=0;i<cfg.numCfgSubscriptions;++i)
	{
        pMyRxCntrl = iec905_igmpv3_group_enroll(cfg.subscriptions[i].usage,  IP_V4_ADDRESS_TYPE, cfg.subscriptions[i].subAddress, cfg.subscriptions[i].srcAddress, 
			    cfg.subscriptions[i].dataSetRef, IEC_90_5_rx_socket);
       if(pMyRxCntrl)
        {
        retVal=iec905_kdc_bind(pMyRxCntrl,pRetKDCRef );
        retVal = iec905_get_kdc_tx_keys(pMyRxCntrl);
        cfg.subscriptions[0].pRxdCntrl= (void *)pMyRxCntrl;
        }
      }
    }

  _beginthread(get_stats,0, (void *) &mySVThreadInfo);


  while(DoIt==TRUE)
    Sleep(5000);

  cancelThreads = TRUE;
  Sleep(1000);

  //termination sequence - unsubscribe for multicast


  for(i=0;i<cfg.numCfgSubscriptions;++i)
    iec905_igmpv3_group_destroy((IEC905_MSG_CNTRL *)cfg.subscriptions[i].pRxdCntrl,IEC_90_5_rx_socket);

  TerminateThread((HANDLE)threadHandle,0);

  iec905_close_socket(IEC_90_5_rx_socket);

  if(StatisticFile!=NULL)
    fclose(StatisticFile);



  return(0);
  }

/**************************************************************/
/*	    usr_notify_of_key_updated_needed		      */
/*							      */
/*  Function is called to notify the user that the keys	      */
/*  found int the CTNRL_STRUCTURE need to be updated.	      */
/*  The user may need to spawn a thread since COMMS to a      */
/*  KDC may be needed					      */
/*							      */
/*  Input: pCntrl - pointer to the control structure	      */
/*							      */
void usr_notify_of_key_updated_needed(IEC905_MSG_CNTRL *pCntrl)
{
  //may desire to spawn a thread to complete this to renew keys

  iec905_get_kdc_tx_keys(pCntrl);
}
  
void usr_notify_of_error(errorCode,fileName,lineNumber)
{
  printf("\nError Generated (%s , %lu): %s\n",fileName,lineNumber,IEC905_XlatErrorCode(errorCode));
}