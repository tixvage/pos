#include "vga.h"
#include "ports.h"

static unsigned char g_320x200x256[] = {
    /* MISC */
    0x63,
    /* SEQ */
    0x03, 0x01, 0x0F, 0x00, 0x0E,
    /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
    0xFF,
    /* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    0xFF,
    /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00, 0x00
};

static uint8_t* get_fb_segment() {
    port_byte_out(GC_INDEX_PORT, 0x06);
    uint8_t segment = port_byte_in(GC_DATA_PORT) & (3<<2);
    switch (segment) {
        case 0<<2: {
            return (uint8_t*)0x00000;
        } break;
        case 1<<2: {
            return (uint8_t*)0xA0000;
        } break;
        case 2<<2: {
            return (uint8_t*)0xB0000;
        } break;
        case 3<<2: {
            return (uint8_t*)0xB8000;
        } break;
    }
}

static void vga_write_regs(uint8_t* regs) {
    port_byte_out(VGA_MISC_PORT, *(regs++));
    for (uint8_t i = 0; i < 5; i++) {
        port_byte_out(SEQUENCER_INDEX_PORT, i);
        port_byte_out(SEQUENCER_DATA_PORT, *(regs++));
    }

    port_byte_out(CRTC_INDEX_PORT, 0x03);
    port_byte_out(CRTC_DATA_PORT, port_byte_in(CRTC_DATA_PORT) | 0x80);
    port_byte_out(CRTC_INDEX_PORT, 0x11);
    port_byte_out(CRTC_DATA_PORT, port_byte_in(CRTC_DATA_PORT) & ~0x80);

    regs[0x03] = regs[0x03] | 0x80;
    regs[0x11] = regs[0x11] & ~0x80;

    for (uint8_t i = 0; i < 25; i++) {
        port_byte_out(CRTC_INDEX_PORT, i);
        port_byte_out(CRTC_DATA_PORT, *(regs++));
    }
    
    for (uint8_t i = 0; i < 9; i++) {
        port_byte_out(GC_INDEX_PORT, i);
        port_byte_out(GC_DATA_PORT, *(regs++));
    }

    for (uint8_t i = 0; i < 21; i++) {
        port_byte_in(AC_RESET_PORT);
        port_byte_out(AC_INDEX_PORT, i);
        port_byte_out(AC_WRITE_PORT, *(regs++));
    }

    port_byte_in(AC_RESET_PORT);
    port_byte_out(AC_INDEX_PORT, 0x20);
}

void init_vga_graphics(void) {
    vga_write_regs(g_320x200x256);
}

void vga_put_pixel(uint32_t x, uint32_t y, uint8_t color_index) {
    uint8_t* pixelAddress = get_fb_segment() + 320*y + x;
    *pixelAddress = color_index;
}
