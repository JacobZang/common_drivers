/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2021 Amlogic, Inc. All rights reserved.
 */
#if defined AML_DEMOD_SUPPORT_ISDBT || defined AML_DEMOD_SUPPORT_DVBT
void program_acf(int acf1[20], int acf2[33])
{
	int i;

	for (i = 0; i < 20; i++)
		dvbt_isdbt_wr_reg((0x2c + i) * 4, acf1[i]);
	for (i = 0; i < 33; i++) {
		dvbt_isdbt_wr_reg(0xfe * 4, i);
		dvbt_isdbt_wr_reg(0xff * 4, acf2[i]);
	}
}

void ini_acf_iireq_src_45m_8m(void)
{
	int acf1[] = { 0x294, 0x085, 0x076, 0x01e,
		0x27c, 0x0af, 0x2bf, 0x06d,
		0x265, 0x0d8, 0x270, 0x05e,
		0x257, 0x0ef, 0x25b, 0x04b,
		0x24f, 0x0fc, 0x254, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3da7cd, 0x3c0f9b, 0x3a7768, 0x38df35,
		0x373f01,
		0x3596cd, 0x33ee98, 0x323e62, 0x307e2b, 0x2eb5f3,
		0x2ce5b9,
		0x2b057e, 0x290d41, 0x26fd00, 0x24dcbd, 0x229477,
		0x202c2c,
		0x1d93dc, 0x1ac386, 0x17b328, 0x144ac1, 0x106a4d,
		0x0be1c8,
		0x07e129, 0x04d0cc, 0x015064, 0x3d47ec, 0x38675e,
		0x326eb1,
		0x326e4d, 0x326e4d, 0x00064d
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_45m_7m(void)
{
	int acf1[] = { 0x283, 0x091, 0x02f, 0x01e,
		0x26a, 0x0b8, 0x296, 0x06d,
		0x253, 0x0dc, 0x257, 0x05e,
		0x245, 0x0f1, 0x246, 0x04b,
		0x23d, 0x0fc, 0x241, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3dafce, 0x3c1f9c, 0x3a8769, 0x38ef37,
		0x374f03,
		0x35aecf, 0x34069b, 0x325665, 0x30962e, 0x2ecdf6,
		0x2cfdbc,
		0x2b1581, 0x291d43, 0x271503, 0x24e4bf, 0x229c78,
		0x202c2d,
		0x1d8bdc, 0x1ab384, 0x179325, 0x141abc, 0x102a46,
		0x0b81be,
		0x07711c, 0x0448bd, 0x00b052, 0x3c7fd6, 0x374740,
		0x308684,
		0x308610, 0x308610, 0x000610
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_45m_6m(void)
{
	int acf1[] = { 0x272, 0x09e, 0x3dc, 0x01e,
		0x259, 0x0c0, 0x272, 0x06d,
		0x242, 0x0e1, 0x240, 0x05e,
		0x235, 0x0f3, 0x234, 0x04b,
		0x22e, 0x0fd, 0x230, 0x04d
	};
	int acf2[] = { 0x3f47ff, 0x3dbfcf, 0x3c379e, 0x3aa76d, 0x391f3c,
		0x378709,
		0x35e6d6, 0x343ea2, 0x328e6d, 0x30d636, 0x2f0dfe,
		0x2d35c4,
		0x2b4d88, 0x294d49, 0x273d08, 0x2504c4, 0x22b47c,
		0x203c2f,
		0x1d9bde, 0x1ac386, 0x17a327, 0x1432bf, 0x104249,
		0x0ba9c2,
		0x07a922, 0x0490c5, 0x01185c, 0x3d0fe5, 0x383f58,
		0x3286af,
		0x328650, 0x328650, 0x000650
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_45m_5m(void)
{
	int acf1[] = { 0x260, 0x0ab, 0x37e, 0x02e,
		0x249, 0x0ca, 0x251, 0x06d,
		0x233, 0x0e6, 0x22d, 0x05e,
		0x227, 0x0f5, 0x224, 0x04b,
		0x220, 0x0fd, 0x221, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3db7cf, 0x3c279d, 0x3a9f6c, 0x39073a,
		0x377707,
		0x35d6d4, 0x3436a0, 0x328e6b, 0x30d636, 0x2f15ff,
		0x2d4dc6,
		0x2b758c, 0x29854f, 0x278511, 0x256ccf, 0x232c89,
		0x20cc3f,
		0x1e33f0, 0x1b6b9b, 0x185b3d, 0x14e2d5, 0x10f260,
		0x0c51d7,
		0x082934, 0x04f8d4, 0x014066, 0x3ccfe4, 0x372f46,
		0x2f5673,
		0x2f55ea, 0x2f55ea, 0x0005ea
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_2857m_8m(void)
{
	int acf1[] = { 0x2df, 0x059, 0x144, 0x00e,
		0x2d3, 0x08f, 0x38d, 0x06f,
		0x2c6, 0x0c5, 0x302, 0x05e,
		0x2be, 0x0e7, 0x2d6, 0x04b,
		0x2b7, 0x0f9, 0x2c8, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3dbfcf, 0x3c379e, 0x3aaf6d, 0x391f3c,
		0x37870a,
		0x35eed7, 0x344ea3, 0x32a66f, 0x30f639, 0x2f3602,
		0x2d65c9,
		0x2b858e, 0x299552, 0x278d12, 0x2564cf, 0x231c88,
		0x20b43d,
		0x1e13ec, 0x1b3395, 0x181336, 0x1492cc, 0x109254,
		0x0be1cb,
		0x07c127, 0x0498c7, 0x00f85b, 0x3cbfde, 0x377747,
		0x309e88,
		0x309e13, 0x309e13, 0x000613
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_2857m_7m(void)
{
	int acf1[] = { 0x2c6, 0x067, 0x10f, 0x01e,
		0x2b4, 0x099, 0x344, 0x06f,
		0x2a2, 0x0cb, 0x2cb, 0x05e,
		0x297, 0x0ea, 0x2a7, 0x04b,
		0x28f, 0x0fa, 0x29c, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3dbfcf, 0x3c379e, 0x3aa76d, 0x39173b,
		0x378709,
		0x35e6d6, 0x3446a2, 0x329e6d, 0x30e637, 0x2f2600,
		0x2d4dc7,
		0x2b6d8c, 0x297d4e, 0x276d0e, 0x2544cb, 0x22fc84,
		0x208438,
		0x1de3e7, 0x1b0b90, 0x17eb30, 0x146ac7, 0x107250,
		0x0bc9c6,
		0x07b124, 0x0490c5, 0x00f85b, 0x3cc7df, 0x37974a,
		0x30ce8d,
		0x30ce19, 0x30ce19, 0x000619
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_2857m_6m(void)
{
	int acf1[] = { 0x2ac, 0x076, 0x0c9, 0x01e,
		0x297, 0x0a4, 0x2fd, 0x06d,
		0x281, 0x0d2, 0x299, 0x05e,
		0x274, 0x0ed, 0x27d, 0x04b,
		0x26c, 0x0fb, 0x274, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3db7cf, 0x3c279d, 0x3a976b, 0x390739,
		0x376f07,
		0x35ced3, 0x342e9f, 0x327e6a, 0x30c634, 0x2f05fc,
		0x2d35c4,
		0x2b5d89, 0x29654c, 0x275d0c, 0x253cca, 0x22fc83,
		0x209439,
		0x1dfbe9, 0x1b2b93, 0x181b35, 0x14b2ce, 0x10ca5a,
		0x0c41d4,
		0x084935, 0x0538d9, 0x01c071, 0x3db7fa, 0x38bf6b,
		0x327eb9,
		0x327e4f, 0x327e4f, 0x00064f
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_2857m_5m(void)
{
	int acf1[] = { 0x292, 0x087, 0x06e, 0x01e,
		0x27a, 0x0b0, 0x2b9, 0x06d,
		0x262, 0x0d8, 0x26d, 0x05e,
		0x254, 0x0f0, 0x258, 0x04b,
		0x24c, 0x0fc, 0x252, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3db7ce, 0x3c279d, 0x3a976b, 0x38ff38,
		0x376706,
		0x35c6d2, 0x341e9d, 0x326e68, 0x30ae31, 0x2eedf9,
		0x2d15c0,
		0x2b2d84, 0x293546, 0x272506, 0x24fcc2, 0x22ac7b,
		0x203c2f,
		0x1d9bde, 0x1ac386, 0x17a327, 0x1422be, 0x103247,
		0x0b91bf,
		0x07891e, 0x0470c1, 0x00e858, 0x3ccfde, 0x37bf4d,
		0x313e96,
		0x313e27, 0x313e27, 0x000627
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_24m_8m(void)
{
	int acf1[] = { 0x303, 0x048, 0x17e, 0x00e,
		0x302, 0x081, 0x3f8, 0x00a,
		0x300, 0x0bd, 0x35b, 0x05e,
		0x2fe, 0x0e3, 0x325, 0x04b,
		0x2fb, 0x0f8, 0x313, 0x04d
	};
	int acf2[] = { 0x3f47ff, 0x3dc7d0, 0x3c3fa0, 0x3abf6f, 0x392f3e,
		0x37a70d,
		0x360eda, 0x346ea7, 0x32c673, 0x31163d, 0x2f5606,
		0x2d8dce,
		0x2bad93, 0x29bd56, 0x27b517, 0x258cd4, 0x23448d,
		0x20cc41,
		0x1e2bf0, 0x1b4b98, 0x182338, 0x149ace, 0x109255,
		0x0bd1ca,
		0x07a123, 0x0468c2, 0x00b054, 0x3c5fd4, 0x37073a,
		0x302e79,
		0x302e05, 0x302e05, 0x000605
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_24m_7m(void)
{
	int acf1[] = { 0x2e7, 0x055, 0x153, 0x00e,
		0x2dd, 0x08b, 0x3a5, 0x06f,
		0x2d2, 0x0c4, 0x315, 0x05e,
		0x2cb, 0x0e6, 0x2e7, 0x04b,
		0x2c5, 0x0f9, 0x2d8, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3dbfcf, 0x3c379e, 0x3aaf6d, 0x391f3c,
		0x37870a,
		0x35eed7, 0x344ea3, 0x32a66f, 0x30ee39, 0x2f2e02,
		0x2d65c9,
		0x2b858e, 0x298d51, 0x278511, 0x255cce, 0x231487,
		0x20a43c,
		0x1e0beb, 0x1b3394, 0x181335, 0x1492cb, 0x109254,
		0x0be1ca,
		0x07b925, 0x0480c5, 0x00d858, 0x3c87d8, 0x373740,
		0x305e80,
		0x305e0b, 0x305e0b, 0x00060b
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_24m_6m(void)
{
	int acf1[] = { 0x2c9, 0x065, 0x118, 0x01e,
		0x2b9, 0x097, 0x34f, 0x06f,
		0x2a7, 0x0ca, 0x2d3, 0x05e,
		0x29c, 0x0e9, 0x2ae, 0x04b,
		0x295, 0x0fa, 0x2a2, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3db7cf, 0x3c2f9d, 0x3a9f6c, 0x390f3a,
		0x377707,
		0x35d6d4, 0x342ea0, 0x32866b, 0x30ce34, 0x2f05fd,
		0x2d35c3,
		0x2b5588, 0x295d4b, 0x27550b, 0x252cc8, 0x22dc80,
		0x206c35,
		0x1dcbe4, 0x1af38c, 0x17cb2d, 0x144ac3, 0x104a4b,
		0x0b99c1,
		0x07791d, 0x0448be, 0x00b052, 0x3c6fd4, 0x37473f,
		0x30c686,
		0x30c618, 0x30c618, 0x000618
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_24m_5m(void)
{
	int acf1[] = { 0x2ab, 0x077, 0x0c6, 0x01e,
		0x295, 0x0a5, 0x2fa, 0x06d,
		0x27f, 0x0d2, 0x297, 0x05e,
		0x272, 0x0ed, 0x27b, 0x04b,
		0x26a, 0x0fb, 0x272, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3db7cf, 0x3c2f9e, 0x3aa76c, 0x39173b,
		0x377f08,
		0x35ded5, 0x343ea1, 0x328e6c, 0x30de36, 0x2f15ff,
		0x2d45c6,
		0x2b658a, 0x29754d, 0x27650d, 0x253cca, 0x22f483,
		0x208438,
		0x1de3e7, 0x1b0b90, 0x17eb30, 0x1472c7, 0x107a51,
		0x0bd9c8,
		0x07c927, 0x04a8c9, 0x01205f, 0x3cf7e4, 0x37e752,
		0x31669a,
		0x31662c, 0x31662c, 0x00062c
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_207m_8m(void)
{
	int acf1[] = { 0x327, 0x039, 0x1a5, 0x07b,
		0x332, 0x076, 0x05c, 0x06e,
		0x33e, 0x0b6, 0x3b8, 0x05e,
		0x344, 0x0e0, 0x37a, 0x04b,
		0x345, 0x0f7, 0x365, 0x04d
	};
	int acf2[] = { 0x3f47ff, 0x3dcfd1, 0x3c57a1, 0x3ad772, 0x394f42,
		0x37c711,
		0x3636df, 0x34a6ad, 0x32fe7a, 0x315645, 0x2f9e0f,
		0x2dd5d7,
		0x2bfd9d, 0x2a0d61, 0x280d21, 0x25e4df, 0x239c98,
		0x212c4d,
		0x1e8bfc, 0x1baba4, 0x188344, 0x14fad9, 0x10ea61,
		0x0c29d4,
		0x07e92d, 0x04a8cb, 0x00f05c, 0x3c87da, 0x371f3e,
		0x30267a,
		0x302604, 0x302604, 0x000604
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_207m_7m(void)
{
	int acf1[] = { 0x307, 0x046, 0x182, 0x00e,
		0x306, 0x080, 0x002, 0x00a,
		0x306, 0x0bd, 0x364, 0x05e,
		0x304, 0x0e3, 0x32d, 0x04b,
		0x301, 0x0f8, 0x31b, 0x04d
	};
	int acf2[] = { 0x3f47ff, 0x3dc7d0, 0x3c47a0, 0x3abf6f, 0x39373f,
		0x37a70d,
		0x3616db, 0x3476a8, 0x32d674, 0x31263f, 0x2f6608,
		0x2d9dd0,
		0x2bbd96, 0x29d559, 0x27cd19, 0x25a4d7, 0x235c90,
		0x20ec45,
		0x1e53f4, 0x1b739d, 0x18533d, 0x14d2d4, 0x10d25c,
		0x0c19d1,
		0x07e12c, 0x04a8ca, 0x00f05c, 0x3c8fdb, 0x372740,
		0x302e7c,
		0x302e05, 0x302e05, 0x000605
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_207m_6m(void)
{
	int acf1[] = { 0x2e6, 0x056, 0x151, 0x00e,
		0x2db, 0x08c, 0x3a1, 0x06f,
		0x2d0, 0x0c4, 0x312, 0x05e,
		0x2c9, 0x0e6, 0x2e4, 0x04b,
		0x2c3, 0x0f9, 0x2d6, 0x04d
	};
	int acf2[] = { 0x3f47ff, 0x3dbfd0, 0x3c3f9f, 0x3ab76e, 0x39273d,
		0x37970b,
		0x35fed9, 0x345ea5, 0x32b671, 0x31063b, 0x2f4604,
		0x2d75cb,
		0x2b9590, 0x29a553, 0x279513, 0x256cd0, 0x232489,
		0x20b43d,
		0x1e0bec, 0x1b3395, 0x180b35, 0x148acb, 0x108253,
		0x0bd1c8,
		0x07a123, 0x0470c2, 0x00c055, 0x3c77d6, 0x372f3e,
		0x306e80,
		0x306e0d, 0x306e0d, 0x00060d
	};

	program_acf(acf1, acf2);
}

void ini_acf_iireq_src_207m_5m(void)
{
	int acf1[] = { 0x2c3, 0x068, 0x109, 0x01e,
		0x2b1, 0x09a, 0x33d, 0x06f,
		0x29f, 0x0cc, 0x2c6, 0x05e,
		0x293, 0x0ea, 0x2a3, 0x04b,
		0x28c, 0x0fa, 0x298, 0x04d
	};
	int acf2[] = { 0x3f3fff, 0x3db7ce, 0x3c279d, 0x3a976b, 0x38ff38,
		0x376706,
		0x35c6d2, 0x341e9e, 0x327669, 0x30be32, 0x2ef5fb,
		0x2d25c1,
		0x2b4586, 0x295549, 0x274509, 0x251cc6, 0x22dc80,
		0x206c34,
		0x1dcbe4, 0x1afb8d, 0x17db2e, 0x1462c5, 0x106a4f,
		0x0bc9c6,
		0x07b124, 0x0488c5, 0x00e859, 0x3cafdc, 0x377f47,
		0x30ee8c,
		0x30ee1d, 0x30ee1d, 0x00061d
	};

	program_acf(acf1, acf2);
}
#endif

