/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef VPP_S7D_SR_REGS_H
#define VPP_VSR_TOP_MISC                           0x5000
#define VPP_VSR_TOP_GCLK_CTRL                      0x5001
#define VPP_VSR_TOP_IN_SIZE                        0x5002
#define VPP_VSR_TOP_OUT_SIZE                       0x5004
#define VPP_VSR_TOP_C42C44_MODE                    0x5008
#define VPP_VSR_PPS_DUMMY_DATA                     0x5009
#define VPP_VSR_DEBUG_MODE                         0x500a
#define VPP_VSR_DITHER_MODE                        0x500b
#define VPP_VSR_DITHER_LUT_0                       0x500c
#define VPP_VSR_DITHER_LUT_1                       0x500d
#define VPP_VSR_DITHER_LUT_2                       0x500e
#define VPP_VSR_DITHER_LUT_3                       0x500f
#define VPP_PI_MISC                                0x5040
#define VPP_PI_GCLK_CTRL                           0x5041
#define VPP_PI_EN_MODE                             0x5048
#define VPP_PI_DICT_NUM                            0x5049
#define VPP_PI_HF_COEF                             0x504a
#define VPP_PI_HF_COEF_F                           0x504b
#define VPP_PI_HPF_NORM_CORING                     0x504c
#define VPP_PI_WIN_OFST                            0x504d
#define VPP_PI_HF_SCL_COEF_0                       0x504e
#define VPP_PI_HF_SCL_COEF_1                       0x504f
#define VPP_PI_HF_SCL_COEF_2                       0x5050
#define VPP_PI_HF_SCL_COEF_3                       0x5051
#define VPP_PI_HF_SCL_COEF_4                       0x5052
#define VPP_PI_HF_SCL_COEF_5                       0x5053
#define VPP_PI_HF_SCL_COEF_6                       0x5054
#define VPP_PI_HF_SCL_COEF_7                       0x5055
#define VPP_PI_HF_SCL_COEF_F                       0x5056
#define VPP_PI_HF_HSC_PART                         0x5057
#define VPP_PI_HF_HSC_INI                          0x5058
#define VPP_PI_HF_VSC_PART                         0x5059
#define VPP_PI_HF_VSC_INI                          0x505a
#define VPP_PI_IN_HSC_PART                         0x505b
#define VPP_PI_IN_HSC_INI                          0x505c
#define VPP_PI_IN_VSC_PART                         0x505d
#define VPP_PI_IN_VSC_INI                          0x505e
#define VPP_PI_PPS_NOR_RS_BITS                     0x505f
#define VPP_PI_MM_WIN_INTERP_EN                    0x5060
#define VPP_PI_MAXSAD_GAMMA_LUT2D_0_0_0            0x5061
#define VPP_PI_MAXSAD_GAMMA_LUT2D_1_0_0            0x5062
#define VPP_PI_MAXSAD_GAMMA_LUT2D_2_0_0            0x5063
#define VPP_PI_MAXSAD_GAMMA_LUT2D_0_1_0            0x5064
#define VPP_PI_MAXSAD_GAMMA_LUT2D_1_1_0            0x5065
#define VPP_PI_MAXSAD_GAMMA_LUT2D_2_1_0            0x5066
#define VPP_PI_MAXSAD_GAMMA_LUT2D_0_2_0            0x5067
#define VPP_PI_MAXSAD_GAMMA_LUT2D_1_2_0            0x5068
#define VPP_PI_MAXSAD_GAMMA_LUT2D_2_2_0            0x5069
#define VPP_PI_MAXSAD_GAMMA_LUT2D_0_3_0            0x506a
#define VPP_PI_MAXSAD_GAMMA_LUT2D_1_3_0            0x506b
#define VPP_PI_MAXSAD_GAMMA_LUT2D_2_3_0            0x506c
#define VPP_PI_MAXSAD_GAMMA_LUT2D_0_4_0            0x506d
#define VPP_PI_MAXSAD_GAMMA_LUT2D_1_4_0            0x506e
#define VPP_PI_MAXSAD_GAMMA_LUT2D_2_4_0            0x506f
#define VPP_PI_MAXSAD_GAMMA_LUT2D_0_5_0            0x5070
#define VPP_PI_MAXSAD_GAMMA_LUT2D_1_5_0            0x5071
#define VPP_PI_MAXSAD_GAMMA_LUT2D_2_5_0            0x5072
#define VPP_PI_MAXSAD_GAMMA_LUT2D_0_6_0            0x5073
#define VPP_PI_MAXSAD_GAMMA_LUT2D_1_6_0            0x5074
#define VPP_PI_MAXSAD_GAMMA_LUT2D_2_6_0            0x5075
#define VPP_PI_MAXSAD_GAMMA_LUT2D_0_7_0            0x5076
#define VPP_PI_MAXSAD_GAMMA_LUT2D_1_7_0            0x5077
#define VPP_PI_MAXSAD_GAMMA_LUT2D_2_7_0            0x5078
#define VPP_PI_MAXSAD_GAMMA_LUT2D_0_8_0            0x5079
#define VPP_PI_MAXSAD_GAMMA_LUT2D_1_8_0            0x507a
#define VPP_PI_MAXSAD_GAMMA_LUT2D_2_8_0            0x507b
#define VPP_PI_CLP_BLD_ALP_LUT_0                   0x507c
#define VPP_PI_CLP_BLD_ALP_LUT_1                   0x507d
#define VPP_PI_CLP_BLD_ALP_LUT_F                   0x507e
#define VPP_PI_LUMA_GAIN_0                         0x507f
#define VPP_PI_LUMA_GAIN_1                         0x5080
#define VPP_PI_LUMA_GAIN_F                         0x5081
#define VPP_PI_HF_STR_GAIN_0                       0x5082
#define VPP_PI_HF_STR_GAIN_1                       0x5083
#define VPP_PI_HF_STR_GAIN_F                       0x5084
#define VPP_PI_OS_ADP_LUT_0                        0x5085
#define VPP_PI_OS_ADP_LUT_1                        0x5086
#define VPP_PI_OS_ADP_LUT_F                        0x5087
#define VPP_PI_GLB_GAIN                            0x5088
#define VPP_PI_OS_UP_DN_GAIN                       0x5089
#define VPP_PI_DEBUG_DEMO_WND_EN                   0x508a
#define VPP_PI_DEBUG_DEMO_WND_COEF_1               0x508b
#define VPP_PI_DEBUG_DEMO_WND_COEF_0               0x508c
#define SAFA_PPS_SR_422_EN                         0x5100
#define SAFA_PPS_VSC_START_PHASE_STEP              0x5101
#define SAFA_PPS_HSC_START_PHASE_STEP              0x5102
#define SAFA_PPS_SC_MISC                           0x5103
#define SAFA_PPS_HSC_INI_PAT_CTRL                  0x5104
#define SAFA_PPS_PRE_HSCALE_COEF_Y1                0x5105
#define SAFA_PPS_PRE_HSCALE_COEF_Y0                0x5106
#define SAFA_PPS_PRE_HSCALE_COEF_C1                0x5107
#define SAFA_PPS_PRE_HSCALE_COEF_C0                0x5108
#define SAFA_PPS_PRE_SCALE                         0x5109
#define SAFA_PPS_PRE_VSCALE_COEF                   0x510a
#define SAFA_PPS_VSC_INIT                          0x510b
#define SAFA_PPS_HSC_INIT                          0x510c
#define SAFA_PPS_INTERP_EN_MODE                    0x510d
#define SAFA_PPS_DEBUG_DEMO_EN                     0x510e
#define SAFA_PPS_INTERP_OUT_FORCE_VALUE            0x510f
#define SAFA_PPS_DIR_INTERP_THD                    0x5110
#define SAFA_PPS_BETA_HF_GAIN                      0x5111
#define SAFA_PPS_CHB_DET_DIFF_THD                  0x5112
#define SAFA_PPS_CHB_DET_CNT_THD                   0x5113
#define SAFA_PPS_DEBUG_DEMO_WND_COEF_1             0x5114
#define SAFA_PPS_DEBUG_DEMO_WND_COEF_0             0x5115
#define SAFA_PPS_YUV_SHARPEN_EN                    0x5116
#define SAFA_PPS_YUV_SHARPEN_GAIN_LUT_0            0x5117
#define SAFA_PPS_YUV_SHARPEN_GAIN_LUT_1            0x5118
#define SAFA_PPS_YUV_SHARPEN_GAIN_LUT_F            0x5119
#define SAFA_PPS_YUV_SHARPEN_OS_ADP_LUT            0x511a
#define SAFA_PPS_YUV_SHARPEN_OS_ADP_LUT_F          0x511b
#define SAFA_PPS_YUV_SHARPEN_FINAL_GAIN            0x511c
#define SAFA_PPS_YUV_SHARPEN_OS_ADP_UP_DN_GAIN     0x511d
#define SAFA_PPS_DIR_EN_MODE                       0x511e
#define SAFA_PPS_DIR_MIN_IDX_VALID                 0x511f
#define SAFA_PPS_DIR_UP_EN                         0x5120
#define SAFA_PPS_DIR_XERR_GAIN                     0x5121
#define SAFA_PPS_DIR_SAD_GAIN                      0x5122
#define SAFA_PPS_DIR_RATE                          0x5123
#define SAFA_PPS_DIR_SAD_PENALTY                   0x5124
#define SAFA_PPS_DIR_XERR_THD                      0x5125
#define SAFA_PPS_MIN2IDX_DIF_GAIN                  0x5126
#define SAFA_PPS_MIN2SAD_DIF_THD                   0x5127
#define SAFA_PPS_MIMAXERR_LUT2D_0_0                0x5128
#define SAFA_PPS_MIMAXERR_LUT2D_0_1                0x5129
#define SAFA_PPS_MIMAXERR_LUT2D_1_0                0x512a
#define SAFA_PPS_MIMAXERR_LUT2D_1_1                0x512b
#define SAFA_PPS_MIMAXERR_LUT2D_2_0                0x512c
#define SAFA_PPS_MIMAXERR_LUT2D_2_1                0x512d
#define SAFA_PPS_MIMAXERR_LUT2D_3_0                0x512e
#define SAFA_PPS_MIMAXERR_LUT2D_3_1                0x512f
#define SAFA_PPS_MIMAXERR_LUT2D_4_0                0x5130
#define SAFA_PPS_MIMAXERR_LUT2D_4_1                0x5131
#define SAFA_PPS_MIMAXERR_LUT2D_5_0                0x5132
#define SAFA_PPS_MIMAXERR_LUT2D_5_1                0x5133
#define SAFA_PPS_MIMAXERR_LUT2D_6_0                0x5134
#define SAFA_PPS_MIMAXERR_LUT2D_6_1                0x5135
#define SAFA_PPS_THETA_GLB_GAIN                    0x5136
#define SAFA_PPS_DIR_UP_SCL_THD                    0x5137
#define SAFA_PPS_DIR_HIST_WIN                      0x5138
#define SAFA_PPS_SAD_FLAT_THD                      0x5139
#define SAFA_PPS_DIR_HIST_DIF_THD                  0x513a
#define SAFA_PPS_DIR0_SWAP_RATE_THD                0x513b
#define SAFA_PPS_DIR1_SWAP_RATE_THD                0x513c
#define SAFA_PPS_DIR_SWAP_THD_H                    0x513d
#define SAFA_PPS_DIR_SWAP_THD_V                    0x513e
#define SAFA_PPS_DIR_SWAP_SAD_RATIO_THD            0x513f
#define SAFA_PPS_DIR_BLEND_ALPHA_DIS_HV            0x5140
#define SAFA_PPS_EDGE_STR_MODE_GAIN                0x5141
#define SAFA_PPS_EDGE_AVGSTD_LUT2D_0_0             0x5142
#define SAFA_PPS_EDGE_AVGSTD_LUT2D_F_0_0           0x5143
#define SAFA_PPS_EDGE_AVGSTD_LUT2D_1_0             0x5144
#define SAFA_PPS_EDGE_AVGSTD_LUT2D_F_1_0           0x5145
#define SAFA_PPS_EDGE_AVGSTD_LUT2D_2_0             0x5146
#define SAFA_PPS_EDGE_AVGSTD_LUT2D_F_2_0           0x5147
#define SAFA_PPS_EDGE_AVGSTD_LUT2D_3_0             0x5148
#define SAFA_PPS_EDGE_AVGSTD_LUT2D_F_3_0           0x5149
#define SAFA_PPS_EDGE_AVGSTD_LUT2D_4_0             0x514a
#define SAFA_PPS_EDGE_AVGSTD_LUT2D_F_4_0           0x514b
#define SAFA_PPS_DIR_DIF_WIN                       0x514c
#define SAFA_PPS_DIR_DIF_GAIN                      0x514d
#define SAFA_PPS_DIR_BLEND_ALPHA_LUT               0x514e
#define SAFA_PPS_HV_ADP_TAP_WIND                   0x514f
#define SAFA_PPS_ADP_TAP_ALP_LUT2D_0_0             0x5150
#define SAFA_PPS_ADP_TAP_ALP_LUT2D_1_0             0x5151
#define SAFA_PPS_ADP_TAP_ALP_LUT2D_2_0             0x5152
#define SAFA_PPS_ADP_TAP_ALP_LUT2D_3_0             0x5153
#define SAFA_PPS_ADP_TAP_ALP_LUT2D_4_0             0x5154
#define SAFA_PPS_ADP_TAP_ALP_LUT2D_5_0             0x5155
#define SAFA_PPS_ADP_TAP_ALP_LUT2D_6_0             0x5156
#define SAFA_PPS_ADP_TAP_ALP_LUT2D_7_0             0x5157
#define SAFA_PPS_ADP_TAP_SAD_THD                   0x5158
#define SAFA_PPS_ADP_TAP_EDGE_STR_THD              0x5159
#define SAFA_PPS_ADP_TAP_ALP_THD                   0x515a
#define SAFA_PPS_SR_ALP_INFO                       0x515b
#define SAFA_PPS_PI_INFO                           0x515c
#define SAFA_PPS_RMETER_MODE                       0x515d
#define SAFA_PPS_SR_GRPH_COR_THD                   0x515e
#define SAFA_PPS_SR_GRPH_GAIN                      0x515f
#define SAFA_PPS_RO_SR_GRPH_FLT_CNT                0x5160
#define SAFA_PPS_RO_SR_GRPH_DTL_CNT                0x5161
#define SAFA_PPS_RMETER_WINDOW1                    0x5162
#define SAFA_PPS_RMETER_WINDOW0                    0x5163
#define SAFA_PPS_RMETER_CORING                     0x5164
#define SAFA_PPS_RMETER_H_LOW_THD                  0x5165
#define SAFA_PPS_RMETER_H_HIG_THD                  0x5166
#define SAFA_PPS_RMETER_H_RATIO                    0x5167
#define SAFA_PPS_RMETER_V_LOW_THD                  0x5168
#define SAFA_PPS_RMETER_V_HIG_THD                  0x5169
#define SAFA_PPS_RMETER_V_RATIO                    0x516a
#define SAFA_PPS_RMETER_D_LOW_THD                  0x516b
#define SAFA_PPS_RMETER_D_HIG_THD                  0x516c
#define SAFA_PPS_RMETER_D_RATIO                    0x516d
#define SAFA_PPS_RO_RMETER_HCNT_0                  0x516e
#define SAFA_PPS_RO_RMETER_HCNT_1                  0x516f
#define SAFA_PPS_RO_RMETER_HCNT_2                  0x5170
#define SAFA_PPS_RO_RMETER_HCNT_3                  0x5171
#define SAFA_PPS_RO_RMETER_HCNT_4                  0x5172
#define SAFA_PPS_RO_RMETER_VCNT_0                  0x5173
#define SAFA_PPS_RO_RMETER_VCNT_1                  0x5174
#define SAFA_PPS_RO_RMETER_VCNT_2                  0x5175
#define SAFA_PPS_RO_RMETER_VCNT_3                  0x5176
#define SAFA_PPS_RO_RMETER_VCNT_4                  0x5177
#define SAFA_PPS_RO_RMETER_PDCNT_0                 0x5178
#define SAFA_PPS_RO_RMETER_PDCNT_1                 0x5179
#define SAFA_PPS_RO_RMETER_PDCNT_2                 0x517a
#define SAFA_PPS_RO_RMETER_PDCNT_3                 0x517b
#define SAFA_PPS_RO_RMETER_PDCNT_4                 0x517c
#define SAFA_PPS_RO_RMETER_NDCNT_0                 0x517d
#define SAFA_PPS_RO_RMETER_NDCNT_1                 0x517e
#define SAFA_PPS_RO_RMETER_NDCNT_2                 0x517f
#define SAFA_PPS_RO_RMETER_NDCNT_3                 0x5180
#define SAFA_PPS_RO_RMETER_NDCNT_4                 0x5181
#define SAFA_PPS_HW_CTRL                           0x5190
#define SAFA_PPS_GATE_CTRL                         0x5191
#define SAFA_PPS_UPSAP_BLANK_NUM                   0x5192
#define SAFA_PPS_BLANK_NUM                         0x5193
#define SAFA_PPS_ANALY_PRE_BLANK_NUM               0x5194
#define SAFA_PPS_ANALY_POST_BLANK_NUM              0x5195
#define SAFA_PPS_HVALP_POST_BLANK_NUM              0x5196
#define SAFA_PPS_CNTL_SCALE_COEF_IDX_LUMA          0x5197
#define SAFA_PPS_CNTL_SCALE_COEF_LUMA              0x5198
#define SAFA_PPS_CNTL_SCALE_COEF_IDX_CHRO          0x5199
#define SAFA_PPS_CNTL_SCALE_COEF_CHRO              0x519a
#define VPP_SR_MISC                                0x5200
#define VPP_SR_GCLK_CTRL0                          0x5201
#define VPP_SR_GCLK_CTRL1                          0x5202
#define VPP_SR_GCLK_CTRL2                          0x5203
#define VPP_SR_EN                                  0x5204
#define VPP_SR_DIR_EN                              0x5208
#define VPP_SR_DIR_LPF_ALPHA_EN                    0x5209
#define VPP_SR_DIR_SAD_GAIN                        0x520a
#define VPP_SR_DIR_SAD_CORE_RATE                   0x520b
#define VPP_SR_DIR_BLEND_ALPHA_LUT                 0x520c
#define VPP_SR_DIR_SAD_THD                         0x520d
#define VPP_SR_DIR_LPF_BETA                        0x520e
#define VPP_SR_DIR_LPF_THETA_0                     0x520f
#define VPP_SR_DIR_LPF_THETA_1                     0x5210
#define VPP_SR_DIR_LPF_ALPHA_OFST_0                0x5211
#define VPP_SR_DIR_LPF_ALPHA_OFST_1                0x5212
#define VPP_SR_DIR_LPF_ALPHA_0                     0x5213
#define VPP_SR_DIR_LPF_ALPHA_1                     0x5214
#define VPP_SR_DIR_LPF_ALPHA_2                     0x5215
#define VPP_SR_DIR_LPF_EDGE_0                      0x5216
#define VPP_SR_DIR_LPF_EDGE_1                      0x5217
#define VPP_SR_DIR_LPF_EDGE_F                      0x5218
#define VPP_SR_DIR_PK_GAMMA                        0x5219
#define VPP_SR_DIR_DIF_PK_GAIN                     0x521a
#define VPP_SR_DIR_MIMAXERR2_LUT_0_0               0x521b
#define VPP_SR_DIR_MIMAXERR2_LUT_0_1               0x521c
#define VPP_SR_DIR_MIMAXERR2_LUT_1_0               0x521d
#define VPP_SR_DIR_MIMAXERR2_LUT_1_1               0x521e
#define VPP_SR_DIR_MIMAXERR2_LUT_2_0               0x521f
#define VPP_SR_DIR_MIMAXERR2_LUT_2_1               0x5220
#define VPP_SR_DIR_MIMAXERR2_LUT_3_0               0x5221
#define VPP_SR_DIR_MIMAXERR2_LUT_3_1               0x5222
#define VPP_SR_DIR_MIMAXERR2_LUT_4_0               0x5223
#define VPP_SR_DIR_MIMAXERR2_LUT_4_1               0x5224
#define VPP_SR_DIR_MIMAXERR2_LUT_5_0               0x5225
#define VPP_SR_DIR_MIMAXERR2_LUT_5_1               0x5226
#define VPP_SR_DIR_MIMAXERR2_LUT_6_0               0x5227
#define VPP_SR_DIR_MIMAXERR2_LUT_6_1               0x5228
#define VPP_SR_DIR_MIN2SAD_GAMMA_THD               0x5229
#define VPP_SR_DIR_PK_LA_ERR_DIS_RATE              0x522a
#define VPP_SR_DIR_SAD_PENALTY                     0x522b
#define VPP_SR_DIR_HF_GAIN_ADJ                     0x522c
#define VPP_DERING_EN                              0x522d
#define VPP_DERING_MED_LUT_0                       0x522e
#define VPP_DERING_MED_LUT_1                       0x522f
#define VPP_DERING_MED_LUT_F                       0x5230
#define VPP_DERING_EDGE_CONF_GAIN                  0x5231
#define VPP_DERING_RAMP_CNT_THD                    0x5232
#define VPP_NR_LPF_EN                              0x5233
#define VPP_NR_DIR_LPF_GAIN                        0x5234
#define VPP_NR_DIR_DIFF_THD_MIN                    0x5235
#define VPP_NR_DIR_DIFF_THD_MAX                    0x5236
#define VPP_NR_DIR_DIFF_THD_RATIO_0_0              0x5237
#define VPP_NR_DIR_DIFF_THD_RATIO_0_1              0x5238
#define VPP_NR_DIR_DIFF_THD_RATIO_1_0              0x5239
#define VPP_NR_DIR_DIFF_THD_RATIO_1_1              0x523a
#define VPP_NR_DIR_DIFF_THD_RATIO_F_0              0x523b
#define VPP_GAU_LPF_ALP_VSSAD_LUT_0                0x523c
#define VPP_GAU_LPF_ALP_VSSAD_LUT_1                0x523d
#define VPP_GAU_LPF_ALP_VSSAD_LUT_F                0x523e
#define VPP_NR_DIR_GAU_BLD_GAIN                    0x523f
#define VPP_HTI_EN_MODE                            0x5240
#define VPP_HTI_INVT_EN                            0x5241
#define VPP_HLTI_BP0_COEF                          0x5242
#define VPP_HLTI_BP1_COEF                          0x5243
#define VPP_HLTI_BP1_COEF_F                        0x5244
#define VPP_HLTI_BP2_COEF_0                        0x5245
#define VPP_HLTI_BP2_COEF_1                        0x5246
#define VPP_HLTI_BP3_COEF_0                        0x5247
#define VPP_HLTI_BP3_COEF_1                        0x5248
#define VPP_HLTI_BP3_COEF_F                        0x5249
#define VPP_HLTI_BST_GAIN                          0x524a
#define VPP_HLTI_BST_CORE                          0x524b
#define VPP_HLTI_WIN                               0x524c
#define VPP_HLTI_ADP_THD                           0x524d
#define VPP_HLTI_INVT_THD                          0x524e
#define VPP_HLTI_BST_GAIN_LUT_0                    0x524f
#define VPP_HLTI_BST_GAIN_LUT_1                    0x5250
#define VPP_HLTI_BST_GAIN_LUT_F                    0x5251
#define VPP_HLTI_OS_ADP_LUT_0                      0x5252
#define VPP_HLTI_OS_ADP_LUT_1                      0x5253
#define VPP_HLTI_OS_ADP_LUT_F                      0x5254
#define VPP_HTI_OS_UP_DN_GAIN                      0x5255
#define VPP_HCTI_BP_3TAP_COEF                      0x5256
#define VPP_HCTI_BP_5TAP_COEF                      0x5257
#define VPP_HCTI_BP_7TAP_COEF                      0x5258
#define VPP_HCTI_BP_9TAP_COEF                      0x5259
#define VPP_HCTI_BP_9TAP_COEF_F                    0x525a
#define VPP_HCTI_BP_11TAP_COEF                     0x525b
#define VPP_HCTI_BP_11TAP_COEF_F                   0x525c
#define VPP_HCTI_BP_15TAP_COEF_0                   0x525d
#define VPP_HCTI_BP_15TAP_COEF_1                   0x525e
#define VPP_HCTI_BP_19TAP_COEF_0                   0x525f
#define VPP_HCTI_BP_19TAP_COEF_1                   0x5260
#define VPP_HCTI_BP_19TAP_COEF_F                   0x5261
#define VPP_HCTI_BST_GAIN                          0x5262
#define VPP_HCTI_BST_CORE                          0x5263
#define VPP_HCTI_WIN                               0x5264
#define VPP_HCTI_ADP_THD                           0x5265
#define VPP_HCTI_INVT_THD                          0x5266
#define VPP_HCTI_BST_GAIN_LUT_0                    0x5267
#define VPP_HCTI_BST_GAIN_LUT_1                    0x5268
#define VPP_HCTI_BST_GAIN_LUT_F                    0x5269
#define VPP_HCTI_OS_ADP_LUT_0                      0x526a
#define VPP_HCTI_OS_ADP_LUT_1                      0x526b
#define VPP_HCTI_OS_ADP_LUT_F                      0x526c
#define VPP_HTI_FINAL_GAIN                         0x526d
#define VPP_VTI_EN                                 0x526e
#define VPP_VTI_MAX_SAD_GAIN                       0x526f
#define VPP_VLTI_BP0_COEF                          0x5270
#define VPP_VLTI_BP1_COEF                          0x5271
#define VPP_VLTI_BST_GAIN_LUT_0                    0x5272
#define VPP_VLTI_BST_GAIN_LUT_1                    0x5273
#define VPP_VLTI_BST_GAIN_LUT_F                    0x5274
#define VPP_VLTI_OS_ADP_LUT_0                      0x5275
#define VPP_VLTI_OS_ADP_LUT_1                      0x5276
#define VPP_VLTI_OS_ADP_LUT_F                      0x5277
#define VPP_VTI_OS_UP_DN_GAIN                      0x5278
#define VPP_VLTI_BST_GAIN_CORE                     0x5279
#define VPP_VCTI_BP0_COEF                          0x527a
#define VPP_VCTI_BP1_COEF                          0x527b
#define VPP_VCTI_BST_GAIN_LUT_0                    0x527c
#define VPP_VCTI_BST_GAIN_LUT_1                    0x527d
#define VPP_VCTI_BST_GAIN_LUT_F                    0x527e
#define VPP_VCTI_OS_ADP_LUT_0                      0x527f
#define VPP_VCTI_OS_ADP_LUT_1                      0x5280
#define VPP_VCTI_OS_ADP_LUT_F                      0x5281
#define VPP_VCTI_BST_GAIN_CORE                     0x5282
#define VPP_HVTI_BLEND_ALPHA_0                     0x5283
#define VPP_HVTI_BLEND_ALPHA_1                     0x5284
#define VPP_VTI_FINAL_GAIN                         0x5285
#define VPP_PK_EN                                  0x5286
#define VPP_PK_DIR0_HP_COEF                        0x5287
#define VPP_PK_DIR1_HP_COEF                        0x5288
#define VPP_PK_DIR2_HP_COEF                        0x5289
#define VPP_PK_DIR3_HP_COEF                        0x528a
#define VPP_PK_DIR4_HP_COEF                        0x528b
#define VPP_PK_DIR4_HP_COEF_F                      0x528c
#define VPP_PK_DIR5_HP_COEF                        0x528d
#define VPP_PK_DIR6_HP_COEF                        0x528e
#define VPP_PK_DIR7_HP_COEF                        0x528f
#define VPP_PK_DIR0_BP_COEF                        0x5290
#define VPP_PK_DIR1_BP_COEF                        0x5291
#define VPP_PK_DIR2_BP_COEF                        0x5292
#define VPP_PK_DIR3_BP_COEF                        0x5293
#define VPP_PK_DIR4_BP_COEF                        0x5294
#define VPP_PK_DIR4_BP_COEF_F                      0x5295
#define VPP_PK_DIR5_BP_COEF                        0x5296
#define VPP_PK_DIR6_BP_COEF                        0x5297
#define VPP_PK_DIR7_BP_COEF                        0x5298
#define VPP_PK_DIR_HP_GAIN                         0x5299
#define VPP_PK_DIR_HP_CORE                         0x529a
#define VPP_PK_DIR_BP_GAIN                         0x529b
#define VPP_PK_DIR_BP_CORE                         0x529c
#define VPP_PK_MODE                                0x529d
#define VPP_PK_CIR_HP_GAIN                         0x529e
#define VPP_PK_CIR_BP_GAIN                         0x529f
#define VPP_PK_DIR_HBP_BLD_ALP_LUT_0               0x52a0
#define VPP_PK_DIR_HBP_BLD_ALP_LUT_1               0x52a1
#define VPP_PK_DIR_HBP_BLD_ALP_LUT_F               0x52a2
#define VPP_PK_CIR_HBP_BLD_ALP_LUT_0               0x52a3
#define VPP_PK_CIR_HBP_BLD_ALP_LUT_1               0x52a4
#define VPP_PK_CIR_HBP_BLD_ALP_LUT_F               0x52a5
#define VPP_PK_DIR_GAIN_LUT_0                      0x52a6
#define VPP_PK_DIR_GAIN_LUT_1                      0x52a7
#define VPP_PK_DIR_GAIN_LUT_F                      0x52a8
#define VPP_PK_CIR_GAIN_LUT_0                      0x52a9
#define VPP_PK_CIR_GAIN_LUT_1                      0x52aa
#define VPP_PK_CIR_GAIN_LUT_F                      0x52ab
#define VPP_PK_FINAL_GAIN                          0x52ac
#define VPP_PK_DIR_CIR_BLD_ALP_GAIN                0x52ad
#define VPP_PK_GAIN_VSLUMA_LUT_0                   0x52ae
#define VPP_PK_GAIN_VSLUMA_LUT_1                   0x52af
#define VPP_PK_GAIN_VSLUMA_LUT_2                   0x52b0
#define VPP_PK_GAIN_VSLUMA_LUT_3                   0x52b1
#define VPP_PK_GAIN_VSLUMA_LUT_F                   0x52b2
#define VPP_PK_COLOR_PRCT_LUT_0                    0x52b3
#define VPP_PK_COLOR_PRCT_LUT_1                    0x52b4
#define VPP_PK_COLOR_PRCT_LUT_2                    0x52b5
#define VPP_PK_COLOR_PRCT_LUT_3                    0x52b6
#define VPP_PK_COLOR_PRCT_LUT_4                    0x52b7
#define VPP_PK_COLOR_PRCT_LUT_5                    0x52b8
#define VPP_PK_COLOR_PRCT_LUT_6                    0x52b9
#define VPP_PK_COLOR_PRCT_LUT_7                    0x52ba
#define VPP_PK_COLOR_PRCT_LUT_8                    0x52bb
#define VPP_PK_COLOR_PRCT_LUT_9                    0x52bc
#define VPP_PK_COLOR_PRCT_LUT_10                   0x52bd
#define VPP_PK_COLOR_PRCT_LUT_11                   0x52be
#define VPP_PK_COLOR_PRCT_LUT_12                   0x52bf
#define VPP_PK_COLOR_PRCT_LUT_13                   0x52c0
#define VPP_PK_COLOR_PRCT_LUT_14                   0x52c1
#define VPP_PK_COLOR_PRCT_LUT_15                   0x52c2
#define VPP_PK_COLOR_PRCT_LUT_16                   0x52c3
#define VPP_PK_COLOR_PRCT_LUT_17                   0x52c4
#define VPP_PK_COLOR_PRCT_LUT_18                   0x52c5
#define VPP_PK_COLOR_PRCT_LUT_19                   0x52c6
#define VPP_PK_COLOR_PRCT_LUT_20                   0x52c7
#define VPP_PK_COLOR_PRCT_LUT_21                   0x52c8
#define VPP_PK_COLOR_PRCT_LUT_22                   0x52c9
#define VPP_PK_COLOR_PRCT_LUT_23                   0x52ca
#define VPP_PK_COLOR_PRCT_LUT_24                   0x52cb
#define VPP_PK_COLOR_PRCT_LUT_25                   0x52cc
#define VPP_PK_COLOR_PRCT_LUT_26                   0x52cd
#define VPP_PK_COLOR_PRCT_LUT_27                   0x52ce
#define VPP_PK_COLOR_PRCT_LUT_28                   0x52cf
#define VPP_PK_COLOR_PRCT_LUT_29                   0x52d0
#define VPP_PK_COLOR_PRCT_LUT_30                   0x52d1
#define VPP_PK_COLOR_PRCT_LUT_31                   0x52d2
#define VPP_PK_COLOR_PRCT_GAIN                     0x52d3
#define VPP_PK_OS_EN_SEL_MODE                      0x52d4
#define VPP_PK_OS_ADP_LUT_0                        0x52d5
#define VPP_PK_OS_ADP_LUT_1                        0x52d6
#define VPP_PK_OS_ADP_LUT_F                        0x52d7
#define VPP_PK_OS_EDGE_GAIN_THD                    0x52d8
#define VPP_PK_OS_GAIN_VSLUMA_LUT_0                0x52d9
#define VPP_PK_OS_GAIN_VSLUMA_LUT_1                0x52da
#define VPP_PK_OS_GAIN_VSLUMA_LUT_2                0x52db
#define VPP_PK_OS_GAIN_VSLUMA_LUT_3                0x52dc
#define VPP_PK_OS_GAIN_VSLUMA_LUT_F                0x52dd
#define VPP_PK_OS_UP_DN                            0x52de
#define VPP_PKTI_EDGE_STR_GAIN_0                   0x52df
#define VPP_PKTI_EDGE_STR_GAIN_1                   0x52e0
#define VPP_PKTI_EDGE_STR_GAIN_F                   0x52e1
#define VPP_SR_CC_EN                               0x52e2
#define VPP_SR_CC_YUV2RGB_OFSET                    0x52e3
#define VPP_SR_CC_YUV2RGB_MAT_0                    0x52e4
#define VPP_SR_CC_YUV2RGB_MAT_1                    0x52e5
#define VPP_SR_CC_YUV2RGB_MAT_2                    0x52e6
#define VPP_SR_CC_YUV2RGB_MAT_3                    0x52e7
#define VPP_SR_CC_YUV2RGB_MAT_F                    0x52e8
#define VPP_SR_CC_YUV2RGB_CLIP_RS                  0x52e9
#define VPP_SR_CC_C_GAIN                           0x52ea
#define VPP_SR_CC_ADP_YWT_THD_GAIN                 0x52eb
#define VPP_SR_CC_ADP_CWT_THD_GAIN                 0x52ec
#define VPP_SR_DEBUG_DEMO_WND_EN                   0x52ed
#define VPP_SR_DEBUG_DEMO_WND_COEF_1               0x52ee
#define VPP_SR_DEBUG_DEMO_WND_COEF_0               0x52ef
#define VPP_CONTRAST_DEBUG_DEMO_WND_EN             0x5300
#define VPP_CONTRAST_DEBUG_DEMO_WND_COEF_1         0x5301
#define VPP_CONTRAST_DEBUG_DEMO_WND_COEF_0         0x5302
#define VPP_DNLP_EN_MODE                           0x5303
#define VPP_DNLP_YGRID_0                           0x5304
#define VPP_DNLP_YGRID_1                           0x5305
#define VPP_DNLP_YGRID_2                           0x5306
#define VPP_DNLP_YGRID_3                           0x5307
#define VPP_DNLP_YGRID_4                           0x5308
#define VPP_DNLP_YGRID_5                           0x5309
#define VPP_DNLP_YGRID_6                           0x530a
#define VPP_DNLP_YGRID_7                           0x530b
#define VPP_DNLP_YGRID_8                           0x530c
#define VPP_DNLP_YGRID_9                           0x530d
#define VPP_DNLP_YGRID_10                          0x530e
#define VPP_DNLP_YGRID_11                          0x530f
#define VPP_DNLP_YGRID_12                          0x5310
#define VPP_DNLP_YGRID_13                          0x5311
#define VPP_DNLP_YGRID_14                          0x5312
#define VPP_DNLP_YGRID_15                          0x5313
#define VPP_DNLP_YGRID_16                          0x5314
#define VPP_DNLP_YGRID_17                          0x5315
#define VPP_DNLP_YGRID_18                          0x5316
#define VPP_DNLP_YGRID_19                          0x5317
#define VPP_DNLP_YGRID_20                          0x5318
#define VPP_DNLP_YGRID_21                          0x5319
#define VPP_DNLP_YGRID_22                          0x531a
#define VPP_DNLP_YGRID_23                          0x531b
#define VPP_DNLP_YGRID_24                          0x531c
#define VPP_DNLP_YGRID_25                          0x531d
#define VPP_DNLP_YGRID_26                          0x531e
#define VPP_DNLP_YGRID_27                          0x531f
#define VPP_DNLP_YGRID_28                          0x5320
#define VPP_DNLP_YGRID_29                          0x5321
#define VPP_DNLP_YGRID_30                          0x5322
#define VPP_DNLP_YGRID_31                          0x5323
#define VPP_DNLP_SATPRT_DIV_M                      0x5324
#define VPP_DNLP_SATPRT_LMT_RGB                    0x5325
#define VPP_DNLP_SATPRT_LMT_RGB_F                  0x5326
#define VPP_DNLP_SATPRT_MODE                       0x5327
#define VPP_DNLP_SATPRT_SAT_RATE_CORE              0x5328
#define VPP_DNLP_COLOR_PRT_GAIN                    0x5329

#endif
