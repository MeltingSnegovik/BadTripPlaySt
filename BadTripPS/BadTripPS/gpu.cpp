#include "gpu.h"



/*
void  pscx_gpu::_horizontalRes::FromFields(uint8_t hr1, uint8_t hr2) {
	d_data = (hr2 & 1) | ((hr1 & 3) << 1);
};
*/

uint32_t pscx_gpu::_horizontalRes::IntoStat() {
	return (uint32_t)d_data << 16;
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
	r |= _gpu::d_hres.IntoStat();
	r |= ((uint32_t)d_vres) << 19;
	r |= ((uint32_t)_gpu::d_hres.IntoStat());
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
	if (d_Gp0WordsRemaining == 0) {
		uint32_t opcode = (val >> 24) & 0xff;
		uint32_t len;
		_commandMeth meth;
		switch (opcode) {
			case 0x00:
				len = 1;
				meth = _commandMeth::e_Gp0Nop;
				break;
			case 0xa0:
				len = 3;
				meth = _commandMeth::e_Gp0ImageLoad;
				break;
			case 0x28:
				len = 5;
				meth = _commandMeth::e_Gp0QuadMonoOpaque;
				break;
			case 0x2c:
				len = 9;		
				meth = _commandMeth::e_Gp0QuadTextureBlandOpaque;
				break;
			case 0x30:
				len = 6;
				meth = _commandMeth::e_Gp0TriangleShadedOpaque;
				break;
			case 0x38:
				len = 8;
				meth = _commandMeth::e_Gp0QuadShadedOpaque;
				break;
			case 0xe1:
				len = 1;
				meth = _commandMeth::e_Gp0DrawMode;
				break;
			case 0xe2:
				len = 1;
				meth = _commandMeth::e_Gp0TextureWindow;
				break;
			case 0xe3:
				len = 1;
				meth = _commandMeth::e_Gp0DrawingAreaTopLeft;
				break;
			case 0xe4:
				len = 1;
				meth = _commandMeth::e_Gp0DrawingAreaBottomRight;
				break;
			case 0xe5:
				len = 1;
				meth = _commandMeth::e_Gp0DrawingOffset;
				break;
			case 0xe6:
				len = 1;
				meth = _commandMeth::e_Gp0MaskBitSetting;
				break;
			default:
				std::cout << "Unhandled GP0 command" << val << std::endl;
				break;
			};
		d_Gp0WordsRemaining = len;
		d_Gp0CommandMethod = meth;
		d_Gp0Command.Clear();
		};

	d_Gp0WordsRemaining--;
	switch (d_Gp0Mode) {
	case _gp0Mode::e_Command:
		d_Gp0Command.PushWord(val);
		if (d_Gp0WordsRemaining == 0)
			Gp0CommandMethodRun(d_Gp0CommandMethod, val); //tbd ??
		break;
	case _gp0Mode::e_ImageLoad:
		if (d_Gp0WordsRemaining == 0)
			d_Gp0Mode = _gp0Mode::e_ImageLoad;
		break;
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
		std::cout << "Unreadable Gp1DmaDirection" << std::endl;
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

void pscx_gpu::_gpu::Gp0CommandMethodRun(_commandMeth meth,uint32_t val) {
	switch (meth) {
	case _commandMeth::e_Gp0DrawingAreaBottomRight:
		Gp0DrawingAreaBottomRight(val);
		break;
	case _commandMeth::e_Gp0DrawingAreaTopLeft :
		Gp0DrawingAreaTopLeft(val);
		break;
	case _commandMeth::e_Gp0DrawingOffset:
		Gp0DrawingOffset(val);
		break;
	case _commandMeth::e_Gp0DrawMode:
		Gp0DrawMode(val);
		break;
	case _commandMeth::e_Gp0MaskBitSetting:
		Gp0MaskBitSetting(val);
		break;
	case _commandMeth::e_Gp0Nop:
		Gp0Nop(val);
		break;
	case _commandMeth::e_Gp0QuadMonoOpaque:
		Gp0QuadMonoOpaque(val);
		break;
	case _commandMeth::e_Gp0TextureWindow:
		Gp0TextureWindow(val);
		break;
	};
};

void pscx_gpu::_gpu::Gp0Nop(uint32_t val) {
	//
};

void pscx_gpu::_gpu::Gp0QuadMonoOpaque(uint32_t val) {
	std::cout << "Draw mr Hanky " << val << std::endl;
};

void pscx_gpu::_gpu::Gp0ClearCache() {
	//
};

void pscx_gpu::_gpu::Gp0ImageLoad() {
	uint32_t res = d_Gp0Command.m_buffer[2];
	uint32_t width = res & 0xffff;
	uint32_t height = res >> 16;

	uint32_t imgsize = (width * height+1 &!1); //tbd pg 132

	d_Gp0WordsRemaining = imgsize / 2;
	d_Gp0Mode = _gp0Mode::e_ImageLoad;
};

void pscx_gpu::_gpu::Gp1DisplayEnable(uint32_t val) {
	d_display_disabled = (val & 1 != 0);
};

void pscx_gpu::_gpu::Gp0ImageStore() {
	uint32_t res = d_Gp0Command.m_buffer[2];
	uint32_t width = res & 0xffff;
	uint32_t height = res >> 16;
	std::cout << "Unhandled IMage Store " << width << " " << height << std::endl;
};

void pscx_gpu::_gpu::Gp0QuadShadedOpaque() {
	std::cout << "Gp0QuadShadedOpaque" << std::endl;
};

void pscx_gpu::_gpu::Gp0TriangleShadedOpaque() {
	std::cout << "pscx_gpu::_gpu::Gp0TriangleShadedOpaque" << std::endl;
};

void pscx_gpu::_gpu::Gp0QuadTextureBlandOpaque() {
	std::cout << "Gp0QuadTextureBlandOpaque" << std::endl;
};

void pscx_gpu::_gpu::Gp1Acknowledge() {
	d_interrupt = false;
};

void pscx_gpu::_gpu::Gp1ResetCommandClear() {
	d_Gp0Command.Clear();
	d_Gp0WordsRemaining = 0;
	d_Gp0Mode = _gp0Mode::e_Command;
};