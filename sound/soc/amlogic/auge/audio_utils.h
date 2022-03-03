/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __AML_AUDIO_UTILS_H__
#define __AML_AUDIO_UTILS_H__

#include <sound/soc.h>

int snd_card_add_kcontrols(struct snd_soc_card *card);

void audio_locker_set(int enable);

int audio_locker_get(void);

void fratv_enable(bool enable);
void fratv_src_select(int src);

void cec_arc_enable(int src, bool enable);
#endif
