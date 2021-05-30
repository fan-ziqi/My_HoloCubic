#ifndef LGFX_PANEL_ST7789_HPP_
#define LGFX_PANEL_ST7789_HPP_

#include "PanelIlitekCommon.hpp"

namespace lgfx
{
 inline namespace v0
 {
//----------------------------------------------------------------------------

  struct Panel_ST7789  : public PanelIlitekCommon
  {
    Panel_ST7789(void)
    {
      panel_height = memory_height = 320;

      freq_write = 40000000;
      freq_read  = 20000000;
      freq_fill  = 40000000;

      spi_mode = 3;
      spi_mode_read = 1;

      len_dummy_read_pixel = 16;
    }

  protected:

    struct CMD : public CommandCommon
    {
      static constexpr std::uint8_t PORCTRL  = 0xB2;      // Porch control
      static constexpr std::uint8_t GCTRL    = 0xB7;      // Gate control
      static constexpr std::uint8_t VCOMS    = 0xBB;      // VCOMS setting
      static constexpr std::uint8_t LCMCTRL  = 0xC0;      // LCM control
      static constexpr std::uint8_t VDVVRHEN = 0xC2;      // VDV and VRH command enable
      static constexpr std::uint8_t VRHS     = 0xC3;      // VRH set
      static constexpr std::uint8_t VDVSET   = 0xC4;      // VDV setting
      static constexpr std::uint8_t FRCTR2   = 0xC6;      // FR Control 2
      static constexpr std::uint8_t PWCTRL1  = 0xD0;      // Power control 1
      static constexpr std::uint8_t PVGAMCTRL= 0xE0;      // Positive voltage gamma control
      static constexpr std::uint8_t NVGAMCTRL= 0xE1;      // Negative voltage gamma control
      static constexpr std::uint8_t COLMOD= 0x3A;
    };
    const std::uint8_t* getInitCommands(std::uint8_t listno) const override {
      static constexpr std::uint8_t list0[] = {
          CMD::SLPOUT , CMD_INIT_DELAY, 120,
          CMD::NORON  , CMD_INIT_DELAY, 0,
          0xB6        , 2, 0x0A,0x82,
          CMD::COLMOD , 1, 0x55, 
          0x30        , 2, 0x00,0xE0,
          CMD::PORCTRL, 5, 0x0c, 0x0c, 0x00, 0x33, 0x33,
          CMD::GCTRL  , 1, 0x45,

          CMD::VCOMS  , 1, 0x3F,  // JLX240 display datasheet
          CMD::LCMCTRL, 1, 0x0C,
          CMD::VDVVRHEN,2, 0x01, 0xFF,
          CMD::VRHS   , 1, 0x10,      // voltage VRHS
          CMD::VDVSET , 1, 0x2A,
          CMD::FRCTR2 , 1, 0x0f,
          CMD::PWCTRL1, 2, 0xa4, 0xa1,
          //--------------------------------ST7789V gamma setting---------------------------------------//
          CMD::PVGAMCTRL,14, 0xd0,0x08,0x11,0x08,
                             0x0C,0x15,0x39,0x33,
                             0x50,0x36,0x13,0x14,
                             0x29,0x2D,
          CMD::NVGAMCTRL,14, 0xd0,0x08,0x10,0x08,
                             0x06,0x06,0x39,0x44,
                             0x51,0x0B,0x16,0x14,
                             0x2F,0x31,
          CMD::DISPON , CMD_INIT_DELAY, 120,
          0xFF,0xFF, // end
      };
      static constexpr std::uint8_t list1[] = {
          CMD::DISPON , CMD_INIT_DELAY, 120,
          0xFF,0xFF, // end
      };
      switch (listno) {
      case 0: return list0;
      case 1: return list1;
      default: return nullptr;
      }
    }
  };

//----------------------------------------------------------------------------
 }
}

#endif
