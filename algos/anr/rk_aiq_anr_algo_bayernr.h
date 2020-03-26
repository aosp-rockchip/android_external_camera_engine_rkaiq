/*

 */

#ifndef _RKAIQ_BAYERNR_H_
#define _RKAIQ_BAYERNR_H_

//在此处包含其它头文件
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "base/xcam_log.h"
#include "rk_aiq_comm.h"
#include "RkAiqCalibDbTypes.h"
#include "anr/rk_aiq_types_anr_algo_prvt.h"

RKAIQ_BEGIN_DECLARE

ANRresult_t init_bayernr_params(RKAnr_Bayernr_Params_t *pParams, CalibDb_BayerNr_t *pCalibdb);

ANRresult_t select_bayernr_params_by_ISO(RKAnr_Bayernr_Params_t *stBayerNrParams, RKAnr_Bayernr_Params_Select_t *stBayerNrParamsSelected,int iso);

unsigned short bayernr_get_trans(int tmpfix);

ANRresult_t bayernr_fix_tranfer(RKAnr_Bayernr_Params_Select_t* rawnr, RKAnr_Bayernr_Fix_t *pRawnrCfg);

ANRresult_t bayernr_fix_printf(RKAnr_Bayernr_Fix_t * pRawnrCfg);

RKAIQ_END_DECLARE

#endif  // BAYERNR_READ_PARM_H_

