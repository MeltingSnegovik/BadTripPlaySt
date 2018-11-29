#include "gpu.h"


/*
void  pscx_gpu::_horizontalRes::FromFields(uint8_t hr1, uint8_t hr2) {
	d_data = (hr2 & 1) | ((hr1 & 3) << 1);
};
*/

uint32_t pscx_gpu::_horizontalRes::IntoStat() {
	d_data = (uint32_t)d_data << 16;
};

uint32_t pscx_gpu::_gpu::Status() {
	uint32_t r = 0;


	r |= (d_page_base_x) << 0;
	r |= ((uint32_t)d_page_base_y) << 4;
	r |= ((uint32_t)d_semi_transparency) << 5;
	r |= ((uint32_t)d_texture_depth) << 7;
	r |= ((uint32_t)d_dithering) << 9;
	r |= ((uint32_t)d_draw_to_display) << 10;
	r |= ((uint32_t)d_force_set_mask_bit) << 11;
	r |= ((uint32_t)d_preserve_masked_pixels) << 12;
	r |= ((uint32_t)d_field) << 13;
	r |= ((uint32_t)d_texture_disable) << 15;
	r |= d_hres.IntoStat();
	r |= ((uint32_t)d_vres) << 19;
	r |= ((uint32_t)d_vmode) << 20;
	r |= ((uint32_t)d_display_depth) << 21;
	r |= ((uint32_t)d_interlaced) << 22;
	r |= ((uint32_t)d_display_disabled) << 23;
	r |= ((uint32_t)d_interrupt) << 24;


	r |= 1 << 26;
	r |= 1 << 27;
	r |= 1 << 28;
	r |= ((uint32_t)d_dma_direction) << 29;
	r |= 0 << 31;
	uint32_t dma_request;
	switch(d_dma_direction) {
	case _dmaDirection::e_OFF:
		dma_request = 0;
		break;
	case _dmaDirection::e_FIFO:
		dma_request = 1;
		break;
	case _dmaDirection::e_GPUTOGP0:
		dma_request = (r >> 28) & 1;
		break;
	case _dmaDirection::e_VRAMTOCPU:
		dma_request = (r >> 27) & 1;
		break;
	};
	r |= dma_request << 25;

	return r;
};