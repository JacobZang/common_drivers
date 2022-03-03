/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __SOUND_INIT_H__
#define __SOUND_INIT_H__

int __init audio_controller_init(void);
void __exit audio_controller_exit(void);
int __init auge_snd_iomap_init(void);
void __exit auge_snd_iomap_exit(void);
int __init audio_clocks_init(void);
void __exit audio_clocks_exit(void);
int __init pdm_init(void);
void __exit pdm_exit(void);
int __init audio_ddr_init(void);
void __exit audio_ddr_exit(void);
int __init tdm_init(void);
void __exit tdm_exit(void);
int __init spdif_init(void);
void __exit spdif_exit(void);
int __init loopback_init(void);
void __exit loopback_exit(void);
int __init audio_locker_init(void);
void __exit audio_locker_exit(void);
int __init resample_drv_init(void);
void __exit resample_drv_exit(void);
int __init effect_init(void);
void __exit effect_exit(void);
int __init vad_drv_init(void);
void __exit vad_drv_exit(void);
int __init vad_dev_init(void);
void __exit vad_dev_exit(void);
int __init aud_sram_iomap_init(void);
void __exit aud_sram_iomap_exit(void);
int __init aml_wwe_init(void);
void __exit aml_wwe_exit(void);
int __init pwrdet_init(void);
void __exit pwrdet_exit(void);
int __init extn_init(void);
void __exit extn_exit(void);
int __init earc_init(void);
void __exit earc_exit(void);
int __init aml_card_init(void);
void __exit aml_card_exit(void);

#endif
