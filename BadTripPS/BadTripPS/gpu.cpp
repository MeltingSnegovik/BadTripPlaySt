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

void pscx_gpu::_gpu::Gp0(uint32_t val) {
	uint32_t c_len;
	if (Gp0CommandRemaining == 0) {
		uint32_t opcode = (val >> 24) & 0xff;
		switch (opcode) {
		case 0x00:
			c_len = 1;
			Gp0Nop();
			break;
		case 0x28:
			c_len = 5;
			Gp0QuadMonoOpaque();
		case 0xe1:
			Gp0DrawMode(val);
			break;
		default:
			std::cout << "unhandled GP0 command " << val << std::endl;
			break;
		};
	};
};

void pscx_gpu::_gpu::Gp0DrawMode(uint32_t val) {
	d_page_base_x = (uint8_t)(val & 0xf);
	d_page_base_y = (uint8_t)((val >> 4) & 1);
	d_semi_transparency = (uint8_t)((val >> 5) & 3);
	switch ((val >> 7) & 3) {
	case 0:
		d_texture_depth = _gpu::_textureDepth::e_T4BIT;
		break;
	case 1:
		d_texture_depth = _gpu::_textureDepth::e_T8BIT;
		break;
	case 2:
		d_texture_depth = _gpu::_textureDepth::e_T15BIT;
		break;
	default:
		std::cout << "unhandled texture depth " << val << std::endl;
		break;
	};
	d_dithering = (((val >> 9) & 1) != 0);
	d_draw_to_display = ((val >> 10) & 1) != 0;
	d_texture_disable = ((val >> 11) & 1)!= 0;
	d_rectangle_texture_x_flip = ((val >> 12) & 1) != 0;
	d_rectangle_texture_y_flip = ((val >> 13) & 1) != 0;
};

void pscx_gpu::_gpu::Gp1(uint32_t val) {
	uint32_t opcode = (val >> 24) & 0xff;
	switch (opcode) {
	case 0x00:
		_gpu::Gp1Reset(val);
		break;
	};
};

void pscx_gpu::_gpu::Gp1Reset(uint32_t val) {
	d_interrupt = false;
	d_page_base_x = 0;
	d_page_base_y = 0;
	d_semi_transparency = 0;
	d_texture_depth = _textureDepth::e_T4BIT;
	d_texture_window_x_mask = 0;
	d_texture_window_y_mask = 0;
	d_texture_window_x_offset = 0;
	d_texture_window_y_offset = 0;
	d_dithering = false;
	d_draw_to_display = false;
	d_texture_disable = false;
	d_rectangle_texture_x_flip = false;
	d_rectangle_texture_y_flip = false;
	d_drawing_area_left = 0;
	d_drawing_area_top = 0;
	d_drawing_area_right = 0;
	d_drawing_area_bottom = 0;
	d_drawing_x_offset = 0;
	d_drawing_y_offset = 0;
	d_force_set_mask_bit = false;
	d_preserve_masked_pixels = false;
	d_dma_direction = _dmaDirection::e_OFF;//DmaDirection::Off;
	d_display_disabled = true;
	d_display_vram_x_start = 0;
	d_display_vram_y_start = 0;
	d_hres = _horizontalRes(0, 0);//HorizontalRes::from_fields(0, 0);
	d_vres = _verticalRes::e_Y240LINES;
	d_vmode = _vmode::e_NTSC;
	d_interlaced = true;
	d_display_horiz_start = 0x200;
	d_display_horiz_end = 0xc00;
	d_display_line_start = 0x10;
	d_display_line_end = 0x100;
	d_display_depth = _displayDepth::e_D15BITS;//DisplayDepth::D15Bits;
};


uint32_t pscx_gpu::_gpu::Read() {
	return 0;
};

void pscx_gpu::_gpu::Gp1DisplayMode(uint32_t val) {
	uint8_t hr1 = val & 3;
	uint8_t hr2 = (val >> 6) & 1;
	
	d_hres = _horizontalRes(hr1, hr2);
	switch (val & 0x4 != 0) {
	case false:
		d_vres = _verticalRes::e_Y240LINES;
		break;
	case true:
		d_vres = _verticalRes::e_Y480LINES;
		break;
	};

	switch (val & 0x8 != 0) {
	case false:
		d_vmode = _vmode::e_NTSC;
		break;
	case true:
		d_vmode = _vmode::e_PAL;
		break;
	};

	switch (val & 0x10 != 0) {
	case false:
		d_display_depth = _displayDepth::e_D24BITS;
		break;
	case true:
		d_display_depth = _displayDepth::e_D15BITS;
		break;
	};

	d_interlaced = (val & 0x20) != 0;
	if ((val & 0x80) != 0) {
		std::cout << "Unsupported display mode " << val << std::endl;
	};
};


void pscx_gpu::_gpu::Gp1DmaDirection(uint32_t val) {
	switch (val & 3) {
	case 0:
		d_dma_direction = _dmaDirection::e_OFF;
		break;
	case 1:
		d_dma_direction = _dmaDirection::e_FIFO;
		break;
	case 2:
		d_dma_direction = _dmaDirection::e_GPUTOGP0;
		break;
	case 3:
		d_dma_direction = _dmaDirection::e_VRAMTOCPU;
		break;
	default:
		std::cout << "Unreadable" << std::endl;
		break;
	};
	return;
};


void pscx_gpu::_gpu::Gp0DrawingAreaTopLeft(uint32_t val) {
	d_drawing_area_top = (uint16_t)((val >> 10) & 0x3ff);
	d_drawing_area_left = (uint16_t)(val & 0x3ff);
};

void pscx_gpu::_gpu::Gp0DrawingAreaBottomRight(uint32_t val) {
	d_drawing_area_bottom = (uint16_t)((val >> 10) & 0x3ff);
	d_drawing_area_right = (uint16_t)(val & 0x3ff);
};

void pscx_gpu::_gpu::Gp0DrawingOffset(uint32_t val) {
	uint16_t x = (val & 0x7ff);
	uint16_t y = ((val >> 11) & 0x7ff);

	d_drawing_x_offset = ((int16_t)(x << 5)) >> 5;
	d_drawing_x_offset = ((int16_t)(y << 5)) >> 5;
};

void pscx_gpu::_gpu::Gp0TextureWindow(uint32_t val) {
	d_texture_window_x_mask = (uint8_t)(val & 0x1f);
	d_texture_window_y_mask = (uint8_t)((val >> 5) & 0x1f);
	d_texture_window_x_offset = (uint8_t)((val >> 10) & 0x1f);
	d_texture_window_y_offset = (uint8_t)((val >> 15) & 0x1f);
};

void pscx_gpu::_gpu::Gp0MaskBitSetting(uint32_t val) {
	d_force_set_mask_bit = (val & 1) != 0;
	d_preserve_masked_pixels = (val & 2) != 0;
};

void pscx_gpu::_gpu::Gp1DisplayVramStart(uint32_t val) {
	d_display_vram_x_start = (uint16_t)(val & 0x3fe);
	d_display_vram_y_start = (uint16_t)((val >> 10) & 0xff);
};

void pscx_gpu::_gpu::Gp1DisplayHorizontalRange(uint32_t val) {
	d_display_horiz_start = (uint16_t)(val & 0xfff);
	d_display_horiz_end = (uint16_t)((val >> 12) & 0xfff);
};

void pscx_gpu::_gpu::Gp1DisplayVerticalRange(uint32_t val) {
	d_display_line_start = (uint16_t)(val & 0x3ff);
	d_display_line_end = (uint16_t)((val >> 10) & 0x3ff);
};

void pscx_gpu::_gpu::Gp0CommandMethodRun(uint32_t par) {

	uint32_t len;
	_commandMeth meth;
	switch (Gp0CommandMethod) {
	case 1:
		meth = _commandMeth::e_Gp0Nop;
		break;
	case 2:

	};
};