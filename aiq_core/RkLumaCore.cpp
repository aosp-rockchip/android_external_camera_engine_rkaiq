/*
 * rkisp_aiq_core.h
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

#include "RkLumaCore.h"
#include "v4l2_buffer_proxy.h"
#ifdef RK_SIMULATOR_HW
#include "simulator/isp20_hw_simulator.h"
#endif
#include "common/rk_aiq_comm.h"
#include "isp20/rkisp2-config.h"

namespace RkCam {
#define ISP2X_MIPI_RAW_NORMAL 0
#define ISP2X_MIPI_RAW_LONG   1
#define ISP2X_MIPI_RAW_SHORT  2

bool
RkLumaCoreThread::loop()
{
    ENTER_ANALYZER_FUNCTION();

    const static int32_t timeout = -1;
    SmartPtr<VideoBuffer> luma = mStatsQueue.pop (timeout);

    if (!luma.ptr()) {
        LOGW_ANALYZER("RkLumaCoreThread got empty stats, stop thread");
        return false;
    }

    XCamReturn ret = mRkLumaCore->analyze (luma);
    if (ret == XCAM_RETURN_NO_ERROR || ret == XCAM_RETURN_BYPASS)
        return true;

    LOGE_ANALYZER("RkAiqCoreThread failed to analyze 3a stats");

    EXIT_ANALYZER_FUNCTION();

    return false;
}

uint16_t RkLumaCore::DEFAULT_POOL_SIZE = 20;

RkLumaCore::RkLumaCore()
    : mRkLumaCoreTh(new RkLumaCoreThread(this))
    , mWorkingMode(RK_AIQ_WORKING_MODE_NORMAL)
    , mCb(NULL)
    , mState(RK_AIQ_CORE_STATE_INVALID)
{
    ENTER_ANALYZER_FUNCTION();
    EXIT_ANALYZER_FUNCTION();
}

RkLumaCore::~RkLumaCore()
{
    ENTER_ANALYZER_FUNCTION();
    EXIT_ANALYZER_FUNCTION();
}

XCamReturn
RkLumaCore::init()
{
    ENTER_ANALYZER_FUNCTION();

    if (mState != RK_AIQ_CORE_STATE_INVALID) {
        LOGE_ANALYZER("wrong state %d\n", mState);
        return XCAM_RETURN_ERROR_ANALYZER;
    }

    mState = RK_AIQ_CORE_STATE_INITED;
    return XCAM_RETURN_NO_ERROR;

    EXIT_ANALYZER_FUNCTION();
}

XCamReturn
RkLumaCore::deInit()
{
    ENTER_ANALYZER_FUNCTION();

    if (mState == RK_AIQ_CORE_STATE_STARTED) {
        LOGE_ANALYZER("wrong state %d\n", mState);
        return XCAM_RETURN_ERROR_ANALYZER;
    }

    mState = RK_AIQ_CORE_STATE_INVALID;

    EXIT_ANALYZER_FUNCTION();

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
RkLumaCore::start()
{
    ENTER_ANALYZER_FUNCTION();

    if ((mState != RK_AIQ_CORE_STATE_INITED) &&
            (mState != RK_AIQ_CORE_STATE_PREPARED) &&
            (mState != RK_AIQ_CORE_STATE_STOPED)) {
        LOGE_ANALYZER("wrong state %d\n", mState);
        return XCAM_RETURN_ERROR_ANALYZER;
    }

    mRkLumaCoreTh->start();
    mState = RK_AIQ_CORE_STATE_STARTED;

    EXIT_ANALYZER_FUNCTION();

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
RkLumaCore::stop()
{
    ENTER_ANALYZER_FUNCTION();

    if (mState != RK_AIQ_CORE_STATE_STARTED) {
        LOGW_ANALYZER("in state %d\n", mState);
        return XCAM_RETURN_NO_ERROR;
    }

    mRkLumaCoreTh->triger_stop();
    mRkLumaCoreTh->stop();
    mState = RK_AIQ_CORE_STATE_STOPED;

    EXIT_ANALYZER_FUNCTION();

    return XCAM_RETURN_NO_ERROR;
}


XCamReturn
RkLumaCore::pushStats(SmartPtr<VideoBuffer> &buffer)
{
    ENTER_ANALYZER_FUNCTION();

    XCAM_ASSERT(buffer.ptr());
    mRkLumaCoreTh->push_stats(buffer);

    EXIT_ANALYZER_FUNCTION();

    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
RkLumaCore::prepare(int mode)
{
    mWorkingMode = mode;
    return XCAM_RETURN_NO_ERROR;
}

XCamReturn
RkLumaCore::analyze(const SmartPtr<VideoBuffer> &buffer)
{
    uint16_t hdrProcessCnt = 1;
    const SmartPtr<V4l2BufferProxy> buf =
        buffer.dynamic_cast_ptr<V4l2BufferProxy>();
#ifdef RK_SIMULATOR_HW
    rk_sim_isp_v200_luma_t* luma =
        (rk_sim_isp_v200_luma_t*)(buf->get_v4l2_userptr());
    LOGD_ANALYZER("lumatest rkaiq get luma(%d): %d\n",
                luma->valid_luma,
                luma->image_luma_result.mean_luma[0]);

    if (!mLumaQueueFIFO.is_empty() && mLumaQueueFIFO.size() == LUMA_FIFO_CNT - 1) {
        SmartPtr<isp_luma_stat_t> luma_frame_pre2 = mLumaQueueFIFO.pop();
        SmartPtr<isp_luma_stat_t> luma_frame_pre1 = mLumaQueueFIFO.pop();
        LOGD_ANALYZER("lumatest now pre1 pre2: %d-%d-%d, datasize: %d\n",
            luma->image_luma_result.mean_luma[0],
            luma_frame_pre1->mean_luma[0],
            luma_frame_pre2->mean_luma[0],
            mLumaQueueFIFO.size());
        float dluma1 = float(luma_frame_pre1->mean_luma[0] - luma->image_luma_result.mean_luma[0]) /
                        (luma_frame_pre1->mean_luma[0] + luma->image_luma_result.mean_luma[0]);
        float dluma2 = float(luma_frame_pre1->mean_luma[0] - luma_frame_pre2->mean_luma[0]) /
                        (luma_frame_pre1->mean_luma[0] + luma_frame_pre2->mean_luma[0]);

        if ((dluma1 > 0.2 && dluma2 > 0.2) ||
                (dluma1 < -0.2 && dluma2 < -0.2)) {
            hdrProcessCnt = 2;
        }

        //write back pre1 result to fifo
        mLumaQueueFIFO.push(luma_frame_pre1);
    }

    SmartPtr<isp_luma_stat_t> luma_result = new isp_luma_stat_t();
    luma_result->mean_luma[0] = luma->image_luma_result.mean_luma[0];

    mLumaQueueFIFO.push(luma_result);


    if (mCb)
        mCb->rkLumaCalcDone(0, hdrProcessCnt);
#else
    rkisp_isp2x_luma_buffer* lumaStat =
            (rkisp_isp2x_luma_buffer*)(buf->get_v4l2_userptr());

    int raw_channal = 1;
    switch (mWorkingMode)
    {
        case RK_AIQ_WORKING_MODE_NORMAL:
            raw_channal = 1;
            break;
        case RK_AIQ_ISP_HDR_MODE_2_FRAME_HDR:
        case RK_AIQ_ISP_HDR_MODE_2_LINE_HDR:
            raw_channal = 0;
            break;
        case RK_AIQ_ISP_HDR_MODE_3_FRAME_HDR:
        case RK_AIQ_ISP_HDR_MODE_3_LINE_HDR:
            raw_channal = 1;
            break;
        default:
            raw_channal = 1;
            break;
    }

    unsigned int mean_luma = 0;
    for (int i = 0; i < ISP2X_MIPI_LUMA_MEAN_MAX; i++) {
        mean_luma += lumaStat->luma[raw_channal].exp_mean[i];
    }
    mean_luma = mean_luma / ISP2X_MIPI_LUMA_MEAN_MAX;

    if (!mLumaQueueFIFO.is_empty() && mLumaQueueFIFO.size() == LUMA_FIFO_CNT - 1) {
        SmartPtr<isp_luma_stat_t> luma_frame_pre2 = mLumaQueueFIFO.pop();
        SmartPtr<isp_luma_stat_t> luma_frame_pre1 = mLumaQueueFIFO.pop();

        float dluma1 = int(luma_frame_pre1->mean_luma[raw_channal] - mean_luma) /
                        float(luma_frame_pre1->mean_luma[raw_channal] + mean_luma);
        float dluma2 = int(luma_frame_pre1->mean_luma[raw_channal] - luma_frame_pre2->mean_luma[raw_channal]) /
                        float(luma_frame_pre1->mean_luma[raw_channal] + luma_frame_pre2->mean_luma[raw_channal]);
        /*
        LOGE_ANALYZER("lumatest(%d) now pre1 pre2: %d-%d-%d, datasize: %d\n",
            mWorkingMode, mean_luma,
            luma_frame_pre1->mean_luma[raw_channal],
            luma_frame_pre2->mean_luma[raw_channal],
            mLumaQueueFIFO.size());
        */
        if ((dluma1 > 0.2 && dluma2 > 0.2) ||
                (dluma1 < -0.2 && dluma2 < -0.2)) {
            hdrProcessCnt = 2;
        }

        //write back pre1 result to fifo
        mLumaQueueFIFO.push(luma_frame_pre1);
    }

    SmartPtr<isp_luma_stat_t> luma_result = new isp_luma_stat_t();
    luma_result->mean_luma[raw_channal] = mean_luma;

    mLumaQueueFIFO.push(luma_result);

    if (mCb)
        mCb->rkLumaCalcDone(lumaStat->frame_id, hdrProcessCnt);
#endif


    return XCAM_RETURN_NO_ERROR;
}

} //namespace RkCam
