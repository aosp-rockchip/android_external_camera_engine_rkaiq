/*
 * rk_aiq_algo_adpcc_itf.c
 *
 *  Copyright (c) 2019 Rockchip Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "rk_aiq_algo_types_int.h"
#include "adpcc/rk_aiq_algo_adpcc_itf.h"
#include "adpcc/adpcc.h"

RKAIQ_BEGIN_DECLARE

typedef struct _RkAiqAlgoContext {
    void* place_holder[0];
} RkAiqAlgoContext;

static RkAiqAlgoContext ctx;

static XCamReturn
create_context(RkAiqAlgoContext **context, const AlgoCtxInstanceCfg* cfg)
{
	XCamReturn result = XCAM_RETURN_NO_ERROR;

	LOGI_ADPCC("%s: (enter)\n", __FUNCTION__ );
	 
#if 1
	AdpccContext_t* pAdpccCtx = NULL;
    AdpccResult_t ret = AdpccInit(&pAdpccCtx);
	if(ret != ADPCC_RET_SUCCESS){
		result = XCAM_RETURN_ERROR_FAILED;
		LOGE_ADPCC("%s: Initializaion Adpcc failed (%d)\n", __FUNCTION__, ret);
	}else{
		*context = (RkAiqAlgoContext *)(pAdpccCtx);
	}
#endif

	LOGI_ADPCC("%s: (exit)\n", __FUNCTION__ );
	return result;
}

static XCamReturn
destroy_context(RkAiqAlgoContext *context)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;
	
	LOGI_ADPCC("%s: (enter)\n", __FUNCTION__ );
	
#if 1
	AdpccContext_t* pAdpccCtx = (AdpccContext_t*)context;
    AdpccResult_t ret = AdpccRelease(pAdpccCtx);
	if(ret != ADPCC_RET_SUCCESS){
		result = XCAM_RETURN_ERROR_FAILED;
		LOGE_ADPCC("%s: release Adpcc failed (%d)\n", __FUNCTION__, ret);
	}
#endif

	LOGI_ADPCC("%s: (exit)\n", __FUNCTION__ );
    return result;
}

static XCamReturn
prepare(RkAiqAlgoCom* params)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;

	LOGI_ADPCC("%s: (enter)\n", __FUNCTION__ );
	
#if 1
    AdpccContext_t* pAdpccCtx = (AdpccContext_t *)params->ctx;
    RkAiqAlgoConfigAdpccInt* pCfgParam = (RkAiqAlgoConfigAdpccInt*)params;
	AdpccConfig_t* pAdpccConfig = &pCfgParam->stAdpccConfig;

	pAdpccConfig->stDpccCalib = pCfgParam->rk_com.u.prepare.calib->dpcc;
	
	AdpccResult_t ret = AdpccConfig(pAdpccCtx, pAdpccConfig);
	if(ret != ADPCC_RET_SUCCESS){
		result = XCAM_RETURN_ERROR_FAILED;
		LOGE_ADPCC("%s: config Adpcc failed (%d)\n", __FUNCTION__, ret);
	}
	
#endif

	LOGI_ADPCC("%s: (exit)\n", __FUNCTION__ );
    return result;
}

static XCamReturn
pre_process(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    return XCAM_RETURN_NO_ERROR;
}

static XCamReturn
processing(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    XCamReturn result = XCAM_RETURN_NO_ERROR;
	int iso;

	LOGI_ADPCC("%s: (enter)\n", __FUNCTION__ );
	
#if 1
	RkAiqAlgoProcAdpccInt* pAdpccProcParams = (RkAiqAlgoProcAdpccInt*)inparams;
	RkAiqAlgoProcResAdpccInt* pAdpccProcResParams = (RkAiqAlgoProcResAdpccInt*)outparams;
	AdpccContext_t* pAdpccCtx = (AdpccContext_t *)inparams->ctx;

	LOGD_ADPCC("%s:%d init:%d iso:%d \n", __FUNCTION__, __LINE__, inparams->u.proc.init, iso);

	if(inparams->u.proc.init)
		iso = 50;
	else
		iso = pAdpccProcParams->rk_com.u.proc.iso;
	
	AdpccResult_t ret = AdpccProcess(pAdpccCtx, iso);
	if(ret != ADPCC_RET_SUCCESS){
		result = XCAM_RETURN_ERROR_FAILED;
		LOGE_ADPCC("%s: processing Adpcc failed (%d)\n", __FUNCTION__, ret);
	}

	AdpccGetProcResult(pAdpccCtx, &pAdpccProcResParams->stAdpccProcResult);	
#endif

	LOGI_ADPCC("%s: (exit)\n", __FUNCTION__ );
    return XCAM_RETURN_NO_ERROR;
}

static XCamReturn
post_process(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams)
{
    return XCAM_RETURN_NO_ERROR;
}

RkAiqAlgoDescription g_RkIspAlgoDescAdpcc = {
    .common = {
        .version = RKISP_ALGO_ADPCC_VERSION,
        .vendor  = RKISP_ALGO_ADPCC_VENDOR,
        .description = RKISP_ALGO_ADPCC_DESCRIPTION,
        .type    = RK_AIQ_ALGO_TYPE_ADPCC,
        .id      = 0,
        .create_context  = create_context,
        .destroy_context = destroy_context,
    },
    .prepare = prepare,
    .pre_process = pre_process,
    .processing = processing,
    .post_process = post_process,
};

RKAIQ_END_DECLARE
