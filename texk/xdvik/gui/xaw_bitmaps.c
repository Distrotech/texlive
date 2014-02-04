#include "xaw_bitmaps.h"

/*
 * Bitmaps for indicating checkbuttons and radiobuttons in Xaw popup windows.
 * The size of these is 16x16.
 */

/* checkbuttons: rectangular box containing a tick mark if on, empty box else */
unsigned char button_check_on_bits[] = {
    0x00, 0x00, 0x00, 0x80, 0xfc, 0xff, 0x04, 0x60, 0x04, 0x30, 0x04, 0x38,
    0x64, 0x2c, 0xe4, 0x26, 0xc4, 0x23, 0x84, 0x21, 0x04, 0x20, 0x04, 0x20,
    0x04, 0x20, 0xfc, 0x3f, 0x00, 0x00, 0x00, 0x00
};
unsigned char button_check_off_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
    0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20, 0x04, 0x20,
    0x04, 0x20, 0xfc, 0x3f, 0x00, 0x00, 0x00, 0x00
};

/* radiobuttons: round and filled with dot if on, empty else */
unsigned char button_radio_on_bits[] = {
    /*     0x70, 0x00, 0x8c, 0x01, 0x02, 0x02, 0x72, 0x02, 0xf9, 0x04, 0xf9, 0x04, */
    /*     0xf9, 0x04, 0x72, 0x02, 0x02, 0x02, 0x8c, 0x01, 0x70, 0x00 */
    0x00,0x00,0x00,0x00,0xc0,0x03,0x30,0x0c,0x08,0x10,0xc8,0x13,0xe4,0x27,0xe4,
    0x27,0xe4,0x27,0xe4,0x27,0xc8,0x13,0x08,0x10,0x30,0x0c,0xc0,0x03,0x00,0x00,
    0x00,0x00
};
unsigned char button_radio_off_bits[] = {
    /*     0x70, 0x00, 0x8c, 0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x04, 0x01, 0x04, */
    /*     0x01, 0x04, 0x02, 0x02, 0x02, 0x02, 0x8c, 0x01, 0x70, 0x00 */
    0x00,0x00,0x00,0x00,0xc0,0x03,0x30,0x0c,0x08,0x10,0x08,0x10,0x04,0x20,0x04,
    0x20,0x04,0x20,0x04,0x20,0x08,0x10,0x08,0x10,0x30,0x0c,0xc0,0x03,0x00,0x00,
    0x00,0x00
};

/*
 * Bitmaps for left-hand markers in Xaw pulldown menus; these only
 * use the mark elements from the button markers (dot, or tick).
 */

/* check options: similar to button_check_*_bits. */
unsigned char menu_check_on_bits[] = {
    0x00, 0x04, 0x00, 0x06, 0x00, 0x03, 0x80, 0x01, 0xc6, 0x00, 0x6e, 0x00,
    0x3c, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned char menu_check_off_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* radio options: tick mark if on, else empty. These are different from
   the `round' buttons in button_radio_* (not ideal ...) since the round
   buttons are too large for menus.
*/
unsigned char menu_radio_on_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0xf8, 0x00, 0xf8, 0x00,
    0xf8, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned char menu_radio_off_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* arrow for Xaw submenus */
unsigned char menu_arrow_bits[] = {
    /*     0x00, 0x00, 0x08, 0x00, 0x18, 0x00, 0x38, 0x00, 0x78, 0x00, 0xf8, 0x00, */
    /*     0x78, 0x00, 0x38, 0x00, 0x18, 0x00, 0x08, 0x00, 0x00, 0x00 */

    0x00, 0x00, 0x20, 0x00, 0x60, 0x00, 0xe0, 0x00, 0xe0, 0x01, 0xe0, 0x03,
    0xe0, 0x01, 0xe0, 0x00, 0x60, 0x00, 0x20, 0x00, 0x00, 0x00
    /*      0x00, 0x00, 0x00, 0x08, 0x00, 0x18, 0x00, 0x38, 0x00, 0x78, 0x00, 0xf8, */
    /*      0x00, 0x78, 0x00, 0x38, 0x00, 0x18, 0x00, 0x08, 0x00, 0x00 */
    /*         0x00, 0x02, 0x06, 0x0e, 0x1e, 0x3e, 0x1e, 0x0e, 0x06, 0x02, 0x00 */
};


/* double arrow for popup menus */
unsigned char menu_double_arrow_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x70, 0x00, 0xf8, 0x00,
    0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0xf8, 0x00, 0x70, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
