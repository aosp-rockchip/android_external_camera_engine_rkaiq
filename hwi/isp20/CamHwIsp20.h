
/*
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

#ifndef _CAM_HW_ISP20_H_
#define _CAM_HW_ISP20_H_

#include "CamHwBase.h"
#include "Isp20Params.h"
#include "SensorHw.h"

namespace RkCam {
#define MAX_MEDIA_INDEX               16
#define DEV_PATH_LEN                  64
#define SENSOR_ATTACHED_FLASH_MAX_NUM 2

typedef struct {
    char media_dev_path[DEV_PATH_LEN];
    char isp_dev_path[DEV_PATH_LEN];
    char csi_dev_path[DEV_PATH_LEN];
    char mpfbc_dev_path[DEV_PATH_LEN];
    char main_path[DEV_PATH_LEN];
    char self_path[DEV_PATH_LEN];
    char rawwr0_path[DEV_PATH_LEN];
    char rawwr1_path[DEV_PATH_LEN];
    char rawwr2_path[DEV_PATH_LEN];
    char rawwr3_path[DEV_PATH_LEN];
    char dma_path[DEV_PATH_LEN];
    char rawrd0_m_path[DEV_PATH_LEN];
    char rawrd1_l_path[DEV_PATH_LEN];
    char rawrd2_s_path[DEV_PATH_LEN];
    char stats_path[DEV_PATH_LEN];
    char input_params_path[DEV_PATH_LEN];
    char mipi_luma_path[DEV_PATH_LEN];
    char mipi_dphy_rx_path[DEV_PATH_LEN];
} rk_aiq_isp_t;

typedef struct {
    char media_dev_path[DEV_PATH_LEN];
    char pp_input_image_path[DEV_PATH_LEN];
    char pp_m_bypass_path[DEV_PATH_LEN];
    char pp_scale0_path[DEV_PATH_LEN];
    char pp_scale1_path[DEV_PATH_LEN];
    char pp_scale2_path[DEV_PATH_LEN];
    char pp_input_params_path[DEV_PATH_LEN];
    char pp_stats_path[DEV_PATH_LEN];
    char pp_dev_path[DEV_PATH_LEN];
} rk_aiq_ispp_t;

typedef struct {
    int isp_ver;
    int awb_ver;
    int aec_ver;
    int afc_ver;
    int ahdr_ver;
    int blc_ver;
    int dpcc_ver;
    int anr_ver;
    int debayer_ver;
    int lsc_ver;
    int ccm_ver;
    int gamma_ver;
    int gic_ver;
    int sharp_ver;
    int dehaze_ver;
} rk_aiq_hw_ver_t;

typedef struct {
    rk_aiq_isp_t isp_info[2];
    rk_aiq_ispp_t ispp_info;
    rk_aiq_hw_ver_t hw_ver_info;
} rk_aiq_isp_hw_info_t;

typedef struct {
    /* sensor entity name format:
     * m01_b_ov13850 1-0010, where 'm01' means module index number
     * 'b' meansback or front, 'ov13850' is real sensor name
     * '1-0010' means the i2c bus and sensor i2c slave address
     */
    std::string sensor_name;
    std::string device_name;
    std::string parent_media_dev;
    int csi_port;
    std::string module_lens_dev_name; // matched using mPhyModuleIndex
    int flash_num;
    std::string module_flash_dev_name[SENSOR_ATTACHED_FLASH_MAX_NUM]; // matched using mPhyModuleIndex
    std::string module_real_sensor_name; //parsed frome sensor entity name
    std::string module_index_str; // parsed from sensor entity name
    char phy_module_orient; // parsed from sensor entity name
    std::vector<rk_frame_fmt_t>  frame_size;
    rk_aiq_isp_t *isp_info;
    struct rkmodule_inf mod_info;
} rk_sensor_full_info_t;

class CamHwIsp20
    : public CamHwBase, public Isp20Params, public V4l2Device {
public:
    explicit CamHwIsp20();
    virtual ~CamHwIsp20();

    // from CamHwBase
    virtual XCamReturn init(const char* sns_ent_name);
    virtual XCamReturn deInit();
    virtual XCamReturn prepare(uint32_t width, uint32_t height, int mode);
    virtual XCamReturn start();
    virtual XCamReturn stop();
    virtual XCamReturn getSensorModeData(const char* sns_ent_name,
                                         rk_aiq_exposure_sensor_descriptor& sns_des);
    virtual XCamReturn setIspParams(SmartPtr<RkAiqIspParamsProxy>& ispParams);
    virtual XCamReturn setExposureParams(SmartPtr<RkAiqExpParamsProxy>& expPar);
    virtual XCamReturn setHdrProcessCount(int frame_id, int count);
    virtual XCamReturn setFocusParams(SmartPtr<RkAiqFocusParamsProxy>& focus_params);
    virtual XCamReturn setIsppParams(SmartPtr<RkAiqIsppParamsProxy>& isppParams);
    static rk_aiq_static_info_t* getStaticCamHwInfo(const char* sns_ent_name);
    static XCamReturn clearStaticCamHwInfo();
    static XCamReturn initCamHwInfos();
    XCamReturn setupHdrLink(int mode, int isp_index, bool enable);
    static XCamReturn selectIqFile(const char* sns_ent_name, char* iqfile_name);
private:
    XCAM_DEAD_COPY(CamHwIsp20);
    int _hdr_mode;
    Mutex _mutex;
    bool _first;
    volatile bool _is_exit;
    SmartPtr<RkAiqIspParamsProxy> _last_aiq_results;
    struct isp2x_isp_params_cfg _full_active_isp_params;
    std::list<SmartPtr<RkAiqIspParamsProxy>> _pending_ispparams_queue;
    std::map<int, SmartPtr<RkAiqIspParamsPool>> _effecting_ispparm_map;
    static std::map<std::string, SmartPtr<rk_aiq_static_info_t>> mCamHwInfos;
    static rk_aiq_isp_hw_info_t mIspHwInfos;
    static std::map<std::string, SmartPtr<rk_sensor_full_info_t>> mSensorHwInfos;
    void gen_full_isp_params(const struct isp2x_isp_params_cfg* update_params,
                             struct isp2x_isp_params_cfg* full_params);
    void dump_isp_config(struct isp2x_isp_params_cfg* isp_params,
                         SmartPtr<RkAiqIspParamsProxy> aiq_results);
    XCamReturn setIspParamsSync();
    void dumpRawnrFixValue(struct isp2x_rawnr_cfg * pRawnrCfg );
    void dumpTnrFixValue(struct rkispp_tnr_config  * pTnrCfg);
    void dumpUvnrFixValue(struct rkispp_nr_config  * pNrCfg);
    void dumpYnrFixValue(struct rkispp_nr_config  * pNrCfg);
    void dumpSharpFixValue(struct rkispp_sharp_config  * pSharpCfg);
};

};
#endif
