#ifndef _PAGECFG_H_
#define _PAGECFG_H_
#define GUI_ADVANCED 1
#include "gui.h"

#define NUM_QUICKPAGES 4
enum DisplayTrims {
    TRIMS_NONE,
    TRIMS_4OUTSIDE,
    TRIMS_4INSIDE,
    TRIMS_6,
    TRIMS_LAST,
};

enum BarSize {
    BARS_NONE,
    BARS_4,
    BARS_8,
    BARS_LAST,
};

struct elem_xy {
    uint16_t x;
    uint16_t y;
};
struct elem_trim {
    struct elem_xy pos;
    uint8_t src;
    uint8_t is_vert;
};
struct elem_toggle {
    struct elem_xy pos;
    uint8_t src;
    uint8_t ico[3];
};
struct elem_box {
    struct elem_xy pos;
    uint8_t src;
    uint8_t type;
};
struct elem_modelico {
    struct elem_xy pos;
};
struct elem_bar {
    struct elem_xy pos;
    uint8_t src;
};

struct elem {
    uint8_t blob[3];
    uint8_t src;
    uint8_t extra[3];
};

#define ELEM_X(elem)    (*((uint32_t *)(&(elem))) & 0x1FF)
#define ELEM_Y(elem)    ((*((uint32_t *)(&(elem))) >> 9) & 0x1FF)
#define ELEM_USED(elem) (*((uint32_t *)(&(elem))) & 0x3FE00)
#define ELEM_TYPE(elem) ((*((uint32_t *)(&(elem))) >> 18) & 0x0F)
#define ELEM_SET_X(elem, x)       *((uint32_t *)(&(elem))) = ((*((uint32_t *)(&(elem))) & ~0x1FF) | (x))
#define ELEM_SET_Y(elem, y)       *((uint32_t *)(&(elem))) = ((*((uint32_t *)(&(elem))) & ~(0x1FF << 9)) | ((y) << 9))
#define ELEM_SET_TYPE(elem, type) *((uint32_t *)(&(elem))) = ((*((uint32_t *)(&(elem))) & ~(0x0F << 18)) | ((type) << 18))

#define ELEM_ICO(elem, j) ((elem).extra[j])
//#define ELEM_TRIM_IS_VERT(elem)       (*((uint32_t *)((elem).blob)) & (1 << 22))
//#define ELEM_BOX_IS_BIG(elem)         (*((uint32_t *)((elem).blob)) & (1 << 22))
//#define ELEM_TRIM_SET_VERT(elem, val) ((*((uint32_t *)((elem).blob)) & ~(1 << 22)) | (val) << 22)
//#define ELEM_BOX_SET_BIG(elem, val)   ((*((uint32_t *)((elem).blob)) & ~(1 << 22)) | (val) << 22)
//NUM_TRIM_ELEMS + NUM_BOX_ELEMS + NUM_BAR_ELEMS + NUM_TOGGLE_ELEMS
#ifndef NUM_ELEMS
    #define NUM_ELEMS (6 + 8 + 8 + 4 + 1)
#endif
//#define NUM_TRIM_ELEMS 6
//#define NUM_BOX_ELEMS 8
//#define NUM_BAR_ELEMS 8
//#define NUM_TOGGLE_ELEMS 4

struct PageCfg2 {
    struct elem elem[NUM_ELEMS];
    uint8_t quickpage[NUM_QUICKPAGES];
};

enum {
    ELEM_SMALLBOX,
    ELEM_BIGBOX,
    ELEM_TOGGLE,
    ELEM_BAR,
    ELEM_VTRIM,
    ELEM_HTRIM,
    ELEM_MODELICO,
    ELEM_BATTERY,
    ELEM_TXPOWER,
    ELEM_LAST,
};

#endif
