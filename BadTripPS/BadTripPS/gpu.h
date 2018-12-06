#pragma once
#include "map.h"
#include "render.h"
#include <iostream>
/*
#include <SDL.h>
*/
#include <SDL_opengl.h>
#include <SDL_video.h>

#include <cstdio>


namespace pscx_gpu
{

	struct _horizontalRes {
		uint8_t d_data;

		_horizontalRes(uint8_t hr1, uint8_t hr2) :
			d_data(0)
		{
			d_data = ((hr2 & 1) | ((hr1 & 3) << 1));
		};
		uint32_t IntoStat();
	};

	struct _position {
		GLshort p_Ox;
		GLshort p_Oy;

		_position(uint32_t val):
			p_Ox((int16_t)val),
			p_Oy((int16_t)(val>>16))
		{};
	};

	struct _color {
		GLubyte p_Red;
		GLubyte p_Blue;
		GLubyte p_Green;

		_color(uint32_t val) :
			p_Red((uint8_t)val),
			p_Blue((uint8_t)(val>>8)),
			p_Green((uint8_t)(val>>16))
		{};
	};

	
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

		enum _commandMeth {
			e_Gp0Nop,
			e_Gp0QuadMonoOpaque,
			e_Gp0DrawMode,
			e_Gp0TextureWindow,
			e_Gp0DrawingAreaTopLeft,
			e_Gp0DrawingAreaBottomRight,
			e_Gp0DrawingOffset,
			e_Gp0MaskBitSetting,
			e_Gp0ImageLoad,
			e_Gp0QuadShadedOpaque,
			e_Gp0TriangleShadedOpaque,
			e_Gp0QuadTextureBlandOpaque
		};

		enum _gp0Mode {
			e_Command,
			e_ImageLoad
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
		pscx_gpu::_horizontalRes d_hres;
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
		// Mirror textured rectangle  along x axis
		bool d_rectangle_texture_x_flip;
		// Mirror textured rectangle  along y axis
		bool d_rectangle_texture_y_flip;

		/// Texture window x mask (8 pixel steps)
		uint8_t d_texture_window_x_mask;
		/// Texture window y mask (8 pixel steps)
		uint8_t d_texture_window_y_mask;
		/// Texture window x offset (8 pixel steps)
		uint8_t d_texture_window_x_offset;
		/// Texture window y offset (8 pixel steps)
		uint8_t d_texture_window_y_offset;
		/// Left-most column of drawing area
		uint16_t d_drawing_area_left;
		/// Top-most line of drawing area
		uint16_t d_drawing_area_top;
		/// Right-most column of drawing area
		uint16_t d_drawing_area_right;
		/// Bottom-most line of drawing area
		uint16_t d_drawing_area_bottom;
		/// Horizontal drawing offset applied to all vertex
		int16_t d_drawing_x_offset;
		/// Vertical drawing offset applied to all vertex
		int16_t d_drawing_y_offset;
		/// First column of the display area in VRAM
		uint16_t d_display_vram_x_start;
		/// First line of the display area in VRAM
		uint16_t d_display_vram_y_start;
		/// Display output horizontal start relative to HSYNC
		uint16_t d_display_horiz_start;
		/// Display output horizontal end relative to HSYNC
		uint16_t d_display_horiz_end;
		/// Display output first line relative to VSYNC
		uint16_t d_display_line_start;
		/// Display output last line relative to VSYNC
		uint16_t d_display_line_end;

		pscx_memory::_buffer d_Gp0Command;

		uint32_t d_Gp0WordsRemaining;

		_commandMeth d_Gp0CommandMethod;

		//current mode of the GP0 register
		_gp0Mode d_Gp0Mode;

		_render d_render;

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
			//d_render()
		{
			d_hres = _horizontalRes(0, 0);
		};

		uint32_t Status();
		void Gp0(uint32_t val);
		void Gp0DrawMode(uint32_t val);
		void Gp1(uint32_t val);
		void Gp1Reset(uint32_t val);
		uint32_t Read();
		void Gp1DisplayMode(uint32_t val);
		void Gp1DmaDirection(uint32_t val);
//		void DoDmaLinkedList(_port port);
		void Gp0DrawingAreaTopLeft(uint32_t val);
		void Gp0DrawingAreaBottomRight(uint32_t val);
		void Gp0DrawingOffset(uint32_t val);
		void Gp0TextureWindow(uint32_t val);
		void Gp0MaskBitSetting(uint32_t val);
		void Gp0Nop(uint32_t val);
		void Gp0QuadMonoOpaque(uint32_t val);
		void Gp0CommandMethodRun(_commandMeth meth, uint32_t val);
		void Gp0ClearCache();
		void Gp0ImageLoad();
		void Gp0ImageStore();
		void Gp0QuadShadedOpaque();
		void Gp0TriangleShadedOpaque();
		void Gp0QuadTextureBlandOpaque();
		void Gp1DisplayVramStart(uint32_t val);
		void Gp1DisplayHorizontalRange(uint32_t val);
		void Gp1DisplayVerticalRange(uint32_t val);
		void Gp1DisplayEnable(uint32_t val);
		void Gp1Acknowledge();
		void Gp1ResetCommandClear();
};


};

