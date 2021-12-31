#include <fel.h>

static int chip_detect(struct xfel_ctx_t * ctx, uint32_t id)
{
	if(id == 0x00185900)
	{
		/*
		 * Dual-Core ARM Cortex-A7
		 */
		if(R32(0x00000000) == 0xea000019)
			return 1;
	}
	return 0;
}

static int chip_reset(struct xfel_ctx_t * ctx)
{
	W32(0x020500a0 + 0x08, (0x16aa << 16) | (0x1 << 0));
	return 1;
}

static int chip_sid(struct xfel_ctx_t * ctx, char * sid)
{
	uint32_t id[4];

	id[0] = R32(0x03006200 + 0x0);
	id[1] = R32(0x03006200 + 0x4);
	id[2] = R32(0x03006200 + 0x8);
	id[3] = R32(0x03006200 + 0xc);
	sprintf(sid, "%08x%08x%08x%08x", id[0], id[1], id[2], id[3]);
	return 1;
}

static int chip_jtag(struct xfel_ctx_t * ctx)
{
	static const uint8_t payload[] = {
		0xff, 0xff, 0xff, 0xea, 0x58, 0x00, 0x9f, 0xe5, 0x00, 0xd0, 0x80, 0xe5,
		0x04, 0xe0, 0x80, 0xe5, 0x00, 0xe0, 0x0f, 0xe1, 0x08, 0xe0, 0x80, 0xe5,
		0x10, 0xef, 0x11, 0xee, 0x0c, 0xe0, 0x80, 0xe5, 0x10, 0xef, 0x1c, 0xee,
		0x10, 0xe0, 0x80, 0xe5, 0x10, 0xef, 0x11, 0xee, 0x14, 0xe0, 0x80, 0xe5,
		0x0c, 0x00, 0x00, 0xeb, 0x28, 0x00, 0x9f, 0xe5, 0x00, 0xd0, 0x90, 0xe5,
		0x04, 0xe0, 0x90, 0xe5, 0x14, 0x10, 0x90, 0xe5, 0x10, 0x1f, 0x01, 0xee,
		0x10, 0x10, 0x90, 0xe5, 0x10, 0x1f, 0x0c, 0xee, 0x0c, 0x10, 0x90, 0xe5,
		0x10, 0x1f, 0x01, 0xee, 0x08, 0x10, 0x90, 0xe5, 0x01, 0xf0, 0x29, 0xe1,
		0x1e, 0xff, 0x2f, 0xe1, 0xf8, 0x7f, 0x03, 0x00, 0x02, 0x34, 0xa0, 0xe3,
		0xf0, 0x20, 0x93, 0xe5, 0x0f, 0x20, 0xc2, 0xe3, 0x03, 0x20, 0x82, 0xe3,
		0xf0, 0x20, 0x83, 0xe5, 0xf0, 0x20, 0x93, 0xe5, 0xf0, 0x20, 0xc2, 0xe3,
		0x30, 0x20, 0x82, 0xe3, 0xf0, 0x20, 0x83, 0xe5, 0xf0, 0x20, 0x93, 0xe5,
		0x0f, 0x2a, 0xc2, 0xe3, 0x03, 0x2a, 0x82, 0xe3, 0xf0, 0x20, 0x83, 0xe5,
		0xf0, 0x20, 0x93, 0xe5, 0x0f, 0x26, 0xc2, 0xe3, 0x03, 0x26, 0x82, 0xe3,
		0xf0, 0x20, 0x83, 0xe5, 0x1e, 0xff, 0x2f, 0xe1, 0x04, 0x00, 0x00, 0x00,
		0x14, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x47, 0x4e, 0x55, 0x00,
		0xaf, 0x41, 0x21, 0xd8, 0x0d, 0x8a, 0x1a, 0x24, 0x15, 0x06, 0x35, 0xff,
		0x86, 0xc4, 0xf9, 0xca, 0xeb, 0xfe, 0xaa, 0x17, 0x2f, 0x6c, 0x69, 0x62,
		0x2f, 0x6c, 0x64, 0x2d, 0x6c, 0x69, 0x6e, 0x75, 0x78, 0x2d, 0x61, 0x72,
		0x6d, 0x68, 0x66, 0x2e, 0x73, 0x6f, 0x2e, 0x33, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xf5, 0xfe, 0xff, 0x6f, 0x04, 0x01, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00,
		0x00, 0x01, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x02, 0x00,
		0x0a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
		0x10, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x1e, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xfb, 0xff, 0xff, 0x6f,
		0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x20, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	};
	fel_write(ctx, 0x00020000, (void *)&payload[0], sizeof(payload));
	fel_exec(ctx, 0x00020000);
	return 1;
}

static int chip_ddr(struct xfel_ctx_t * ctx, const char * type)
{
	return 0;
}

static int chip_spi_init(struct xfel_ctx_t * ctx, uint32_t * swapbuf, uint32_t * swaplen, uint32_t * cmdlen)
{
	return 0;
}

static int chip_spi_run(struct xfel_ctx_t * ctx, uint8_t * cbuf, uint32_t clen)
{
	return 0;
}

struct chip_t r528 = {
	.name = "R528",
	.detect = chip_detect,
	.reset = chip_reset,
	.sid = chip_sid,
	.jtag = chip_jtag,
	.ddr = chip_ddr,
	.spi_init = chip_spi_init,
	.spi_run = chip_spi_run,
};
