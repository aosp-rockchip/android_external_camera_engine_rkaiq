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

#ifndef _RK_AIQ_VERSION_H_
/*!
 * ==================== AIQ VERSION HISTORY ====================
 *
 * v0.0.9
 *  - initial version, support HDR
 *  - optimized tnr,nr in low light condition
 *  - optimized imx347 iq
 *  - FEC,ORB,LDCH not enabled
 *  - set compile optimized level to Os
 * v0.1.0
 *  - optimize nr quality under low light
 *  - optimize hdr image quality
 *  - fix circular flicker of image luma
 *  - add fec and ldch, default disabled
 * v0.1.1
 *  - fix some bugs of bayer nr, ynr, uvnr
 *  - optimize imx347 hdr mode image quality
 *  - workaround for imx347 ae flicker when the
 *    luma changed dramatically
 * v0.1.2
 *  - hdr: fix clipRatio error,and redefine tmo params
 *  - fix some bugs in ae smoot
 *  - fix high iso blc problem and uvnr / 3dnr parameters
 *  - fix mmap memory leak
 *  - fix gic bugs
 *  - add color saturation level and color inhibition level setting and getting in accm module
 *  - update imx347 and os04a10 xml
 *  - add gc4c33 xml
 * v0.1.3
 *  - IMX347: split iqfile to hdr and normal, enable fec default
 *  - add dcg setting in aiq
 *  - ablc: iq xml support diff iso diff blc value
 *  - use different iq file for mode hdr and normal
 *  - implement uapi and test
 *  - add Antiflicker-Mode
 *  - add the switch whether to enable HDR module
 *  - using mipi read back mode for normal mode
 *  - enable adebayer module
 *  - update dpcc setting in GC4C33 xml
 * v0.1.4
 * - implement module control api
 * - calibdb fast loaded
 * - afec dynamic control
 * - NR: support max 4096x gain for local gain mode
 * - add HLROIExpandEn in IQ/aiq
 * - NR,Sharp,BLC,DPCC: support 13 levels iso value
 * - ORB: bring up
 * - make sure the media link correctly when streaming on
 * - UVNR: sigmaR params change to 256/old_sigmaR
 * - gc4c33: update iqfiles v1.0.3
 * v0.1.5
 * - imx347 IQ xml v1.0.9
 * - update ahdr algo
 * - modify awb & aec runinterval para & mfnr para for gc4c33
 * - use VS as unified timestamp
 * v0.1.6
 * - gc4c33 IQ v1.0.7
 * - imx347 IQ v1.0.a
 * - NR & sharp: support free iso level on machine
 * - move paras of wbgain clip and wbgain adjustment to xml
 * - add awb chromatic adatptation gain adjust funciton
 * - add rk_aiq_uapi_sysctl_get3AStats interface
 * v0.1.7
 * - gc4c33 IQ v1.0.8
 * - sharp: fix bug of select iso level
 * - rkisp_parser_demo: parse xml and generate bin
 * - support IR-CUT&Infrared-light control
 * - add synchronization mechanism for capturing RAW and YUV images
 * - NR & sharp: fix bug for free iso level
 * - fix wrong expression in caga part
 * - modify calibdb load logic
 * v0.1.8
 * - gc4c33 iq v1.0.b
 * - demo support full/limit range
 * - fix rkisp_parse_demo can't generate bin error
 * - Add sensor dpcc setting to IQ
 * - change sensor_dpcc.enable from bool to int
 * - update DPCC setting in GC4C33 IQ
 * - format the source codes
 * - add hsnr & lsnr control from AE pre results
 * v0.1.9
 * - tnr disable/enable controlled by iq xml
 * - GC4C33 iq v1.0.c
 * - ae v0.1.3
 * - add env variable normal_no_read_back
 * - fix ahdr bug in v0.1.8
 * v1.0.0
 * - product API implement, include:
 *   - brightness/contrast/sataration/sharpeness
 *   - exposure time&gain range
 *   - white balance scene and R/G gain
 *   - noise reduction
 *   - dehaze&fec
 * - suppport cpsl(compensation light) functionality
 *   - suppport LED or IR light source
 *   - support gray mode
 *   - support auto light compensation control
 * - rkisp_parser_demo: fixup iqfile path are too long to be complete
 * - change calib parser version to v1.0.1
 * - change the name of '/tmp/capture_cnt' to '/tmp/.capture_cnt'
 * - determine isp read back times according to lumaDetect module
 * - fix sensor dpcc bug
 * v1.0.1
 * - iq parser support tag check
 *   - Calib v1.1.1 magic code 996625
 * - add following uApi
 *   - rk_aiq_uapi_getGrayMode
 *   - rk_aiq_uapi_setGrayMode
 *   - rk_aiq_uapi_setFrameRate
 *   - rk_aiq_uapi_getFrameRate
 *   - rk_aiq_uapi_sysctl_enumStaticMetas
 *   - rk_aiq_uapi_get_version_info
 * - cpsl support sensitivity and strength control
 * - add iq for OV02K10
 * - add iq for imx334
 * - fix accm-saturation bug
 * v1.0.2
 * - Calib : v1.1.3  magic code: 1003001
 * - iq_parser: fix make error for host
 * - add imx307/ov2718 xml
 * - fix Saturation_adjust_API bug
 * - support re-start and re-prepare procedure
 * - support sharp fbc rotation
 * - support VICAP MIPI + ISP, LVDS + ISP
 *   - tested on imx307, imx415, os04a10
 * - nr,sharp: add IQ para set & get interface
 * - ae: v0.1.4-20200722
 * - Fix the bug of getStaticCamHwInfo function
 * - xcore: add mutex for dq/que buffer, fix can_not_get_buffer error
 */

#define RK_AIQ_VERSION_REAL_V "v1.0.2"
#define RK_AIQ_RELEASE_DATE "2020-07-24"

/******* DO NOT EDIT THE FOLLOWINGS ***********/

#define RK_AIQ_VERSION_HEAD "AIQ "
#define RK_AIQ_VERSION \
    RK_AIQ_VERSION_HEAD\
    RK_AIQ_VERSION_REAL_V

#endif
