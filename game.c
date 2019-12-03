
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include <string.h>

// include NESLIB header
#include "neslib.h"

// include CC65 NES Header (PPU)
#include <nes.h>

// link the pattern table into CHR ROM
//#link "chr_generic.s"

// BCD arithmetic support
#include "bcd.h"
//#link "bcd.c"

// VRAM update buffer
#include "vrambuf.h"
//#link "vrambuf.c"

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
  0x01,			// screen color

  0x11,0x30,0x27,0x0,	// background palette 0
  0x1c,0x20,0x2c,0x0,	// background palette 1
  0x00,0x10,0x20,0x0,	// background palette 2
  0x06,0x16,0x26,0x0,   // background palette 3

  0x16,0x35,0x24,0x0,	// sprite palette 0
  0x00,0x37,0x25,0x0,	// sprite palette 1
  0x0d,0x2d,0x3a,0x0,	// sprite palette 2
  0x0d,0x27,0x2a	// sprite palette 3
};

int y = 160;

int yv = 0;

char pad;

byte s = 0;



bool start = false;

// setup PPU and tables
void setup_graphics() {
  // clear sprites
  oam_clear();
  // set palette colors
  pal_all(PALETTE);
} 

void die() {
  start = false;
  yv = 0;
  y = 160;
}

void main(void)
{
  setup_graphics();
  
  vram_adr(NTADR_A(1,1));
  
  vrambuf_clear();
  set_vram_update(updbuf);
  
  vram_fill(0x09, 30);
  
  vram_adr(NTADR_A(0,27));
  
  vram_fill(0x09, 31);
  
  // enable rendering
  ppu_on_all();
  // infinite loop
  while(1) {
    pad = pad_poll(0);
    
    if (start) {
    	if (yv < 40) yv += 2;
   	y += yv;
    	if (pad & PAD_A) yv = -30;
    	oam_spr(32, y/10, 0xb1, 1, 0);
      
      	if (y > 2040 || y < 160) die();
    } else if (pad & PAD_START) {
      start = true;
      s++;
    }
    
    vrambuf_clear();
    
    vrambuf_put(NTADR_A(2,3), "0", 1);
    
    
    
    ppu_wait_frame();
  }
}
