#pragma once
#include "map.h"



namespace pscx_gpu
{
	struct _gpu {

		enum _textureDepth {
			e_T4BIT,
			e_T8BIT,
			e_T15BIT
		};

		enum _field {
			e_BOTTOM,
			e_TOP
		};

		enum _verticalRes {
			e_Y240LINES,
			e_Y480LINES
		};

		enum _vmode {
			e_NTSC,
			e_PAL
		};

		enum _displayDepth {
			e_D15BITS,
			e_D24BITS
		};

		enum _dmaDirection {
			e_OFF,
			e_FIFO,
			e_GPUTOGP0,
			e_VRAMTOCPU
		};	

		/// Texture page base X coordinate (4 bits, 64 byte increment)
		uint8_t d_page_base_x;
		/// Texture page base Y coordinate (1bit, 256 line increment)
		uint8_t d_page_base_y;
		/// Semi-transparency. Not entirely sure how to handle that value
		/// yet, it seems to describe how to blend the source and
		/// destination colors.
		uint8_t d_semi_transparency;
		/// Texture page color depth
		_textureDepth d_texture_depth;
		/// Enable dithering from 24 to 15bits RGB
		bool d_dithering;
		/// Allow drawing to the display area
		bool d_draw_to_display;
		/// Force "mask" bit of the pixel to 1 when writing to VRAM
		/// (otherwise don't modify it)
		bool d_force_set_mask_bit;
		/// Don't draw to pixels which have the "mask" bit set
		bool d_preserve_masked_pixels;
		/// Currently displayed field. For progressive output this is
		/// always Top.
		_field d_field;
		/// When true all textures are disabled
		bool d_texture_disable;
		/// Video output horizontal resolution
		_horizontalRes d_hres;
		/// Video output vertical resolution
		_verticalRes d_vres;
		/// Video mode
		_vmode d_vmode;
		/// Display depth. The GPU itself always draws 15bit RGB, 24bit
		/// output must use external assets (pre-rendered textures, MDEC,
		/// etc...)
		_displayDepth d_display_depth;
		/// Output interlaced video signal instead of progressive
		bool d_interlaced;
		/// Disable the display
		bool d_display_disabled;
		/// True when the interrupt is active
		bool d_interrupt;
		/// DMA request direction
		_dmaDirection d_dma_direction;


		_gpu() :
			d_page_base_x(0),
			d_page_base_y(0),
			d_semi_transparency(0),
			d_texture_depth(e_T4BIT),
			d_dithering(false),
			d_draw_to_display(false),
			d_force_set_mask_bit(false),
			d_preserve_masked_pixels(false),
			d_field(e_TOP),
			d_texture_disable(false),
			d_hres(_horizontalRes(0,0)),
			d_vres(_verticalRes::e_Y240LINES),
			d_vmode(_vmode::e_NTSC),
			d_display_depth(_displayDepth::e_D15BITS),
			d_interlaced(false),
			d_display_disabled(true),
			d_interrupt(false),
			d_dma_direction(_dmaDirection::e_OFF)
		{};

		uint32_t Status();
	};

	struct _horizontalRes {
		uint8_t d_data;

		_horizontalRes(uint8_t hr1, uint8_t hr2):
			d_data(0)
		{
			d_data = ((hr2 & 1) | ((hr1 & 3) << 1));
		};
		uint32_t IntoStat();
	};

};