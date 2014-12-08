#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "gui.h"

#define DEFAULT_FONT   (Display.font[0])
#define MODELNAME_FONT (Display.font[1])
#define TITLE_FONT     (Display.font[2])
#define BIGBOX_FONT    (Display.font[3])
#define SMALLBOX_FONT  (Display.font[4])
#define BATTERY_FONT   (Display.font[5])
#define BATTALARM_FONT (Display.font[6])
#define ERROR_FONT     (Display.font[6])  //Same as BATTALARM_FONT
#define TINY_FONT      (Display.font[7])
#define MICRO_FONT     (Display.font[8])
#define BOLD_FONT      (Display.font[9])
#define NARROW_FONT    (Display.font[10])
#define SMALL_FONT     (Display.font[11])
#define BIGBOXNEG_FONT   (Display.font[12])
#define SMALLBOXNEG_FONT (Display.font[13])
#define DIALOGTITLE_FONT (Display.font[14])
#define DIALOGBODY_FONT  (Display.font[15])
#define NORMALBOX_FONT     (Display.font[16])
#define NORMALBOXNEG_FONT  (Display.font[17])
#define SECTION_FONT       (Display.font[18])
#define TEXTSEL_FONT       (Display.font[19])
#define BUTTON_FONT        (Display.font[20])
#define NUM_LABELS 21

struct disp_keyboard {
    uint8_t font;
    uint16_t fill_color;
    uint16_t fg_key1;
    uint16_t bg_key1;
    uint16_t fg_key2;
    uint16_t bg_key2;
    uint16_t fg_key3;
    uint16_t bg_key3;
};

struct disp_listbox {
    uint16_t font;
    uint16_t bg_color;
    uint16_t fg_color;
    uint16_t bg_select;
    uint16_t fg_select;
    uint16_t bg_bar;
    uint16_t fg_bar;
};

struct disp_bargraph {
    uint16_t bg_color;
    uint16_t fg_color_pos;
    uint16_t fg_color_neg;
    uint16_t fg_color_zero;
    uint16_t outline_color;
};

struct disp_xygraph {
    uint16_t bg_color;
    uint16_t fg_color;
    uint16_t grid_color;
    uint16_t axis_color;
    uint16_t point_color;
    uint16_t outline_color;
};

struct disp_scrollbar {
    uint16_t bg_color;
    uint16_t fg_color;
};

struct disp_metrics {
    uint8_t header_height;
    uint8_t header_widget_height;
    uint8_t line_height;
    uint8_t line_space;
};

enum DispFlags {
    BAR_TRANSPARENT   = 0x01,
    TRIM_TRANSPARENT  = 0x02,
    SHOW_TIME         = 0x40,
    SHOW_BAT_ICON     = 0x80,
};

struct display_settings {
    struct LabelDesc font[NUM_LABELS];
    struct disp_keyboard keyboard;
    struct disp_listbox listbox;
    struct disp_scrollbar scrollbar;
    struct disp_metrics metrics;
    struct disp_bargraph bargraph;
    struct disp_xygraph xygraph;
    struct disp_bargraph trim;
    uint16_t select_color;
    uint8_t select_width;
    enum DispFlags flags;
};

extern struct display_settings Display;


uint8_t CONFIG_ReadDisplay(void);
#endif
