/*
 * gui.h
 *
 *  Created on: Apr 29, 2012
 *      Author: matcat
 *      GUI Handling
 */

#ifndef GUI_H_
#define GUI_H_

#include <stdint.h>
#include "target.h"
#include "_gui.h"

#define RGB888_to_RGB565(r, g, b) (((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >>3))
enum DialogType {
    dtOk, dtCancel, dtOkCancel, dtNone,
};

enum BarGraphDirection {
    BAR_HORIZONTAL,
    BAR_VERTICAL,
    TRIM_HORIZONTAL,
    TRIM_INVHORIZONTAL,
    TRIM_VERTICAL,
};
enum TextSelectType {
    TEXTSELECT_224,
    TEXTSELECT_128,
    TEXTSELECT_64,
    TEXTSELECT_96,
    TEXTSELECT_DEVO10, // indication for textsel specically in devo10
// only used for RTC config in Devo12
#if HAS_RTC
    TEXTSELECT_VERT_64, // TEXTSELECT_64 with arrows above and beyond instead of sideways
#endif
};

enum KeyboardType {
    KEYBOARD_ALPHA,
    KEYBOARD_NUM,
    KEYBOARD_SPECIAL,
};

enum LabelType {
    LABEL_NO_BOX,
    LABEL_CENTER,
    LABEL_FILL,
    LABEL_TRANSPARENT,
    LABEL_UNDERLINE,
    LABEL_SQUAREBOX,
    LABEL_INVERTED,
    LABEL_LEFT,    // align left and top vertically
    LABEL_LEFTCENTER, // align left and center vertically
    LABEL_BOX,
    LABEL_BRACKET,
    LABEL_BLINK,
};

enum ListBoxType {
    LISTBOX_DEVO10,
    LISTBOX_OTHERS,
};

enum ListBoxNavigateKeyType {
    LISTBOX_KEY_UPDOWN,
    LISTBOX_KEY_RIGHTLEFT,
};

struct LabelDesc {
    uint8_t font;
    uint16_t font_color;
    uint16_t fill_color;
    uint16_t outline_color;
    enum LabelType style;
};

struct ImageMap {
    const char *file;
    uint8_t width;
    uint8_t height;
    uint16_t x_off;
    uint16_t y_off;
};

#include "buttons.h"

extern const struct ImageMap image_map[IMAGE_MAP_END];

enum GUIType {
    UnknownGUI,
    Button,
    Label,
    Image,
    CheckBox,
    Dropdown,
    Dialog,
    XYGraph,
    BarGraph,
    TextSelect,
    Listbox,
    Keyboard,
    Scrollbar,
    Scrollable,
    Rect,
};

struct guiBox {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};

#define guiObject guiHeader
struct guiHeader {
    struct guiBox box;
    struct guiObject *next;
    enum GUIType Type;
    uint8_t flags;
};
typedef struct guiHeader guiObject_t;



typedef struct guiImage {
    struct guiHeader header;
    const char *file;
    uint16_t x_off;
    uint16_t y_off;
    uint32_t crc;
    void (*callback)(struct guiObject *obj, int8_t press_type, const void *data);
    const void *cb_data;
} guiImage_t;

typedef struct guiLabel {
    struct guiHeader header;
    struct LabelDesc desc;
    const char *(*strCallback)(struct guiObject *obj, const void *data);
    void (*pressCallback)(struct guiObject *obj, int8_t press_type, const void *data);
    const void *cb_data;
} guiLabel_t;

typedef struct guiKeyboard {
    struct guiHeader header;
    uint8_t last_row;
    uint8_t last_col;
    uint8_t lastchar;
    char *text;
    int32_t max_size;
    uint8_t flags;
    enum KeyboardType type;
    buttonAction_t action;
    void (*CallBack)(struct guiObject *obj, void *data);
    void *cb_data;
} guiKeyboard_t;

typedef struct guiScrollbar {
    struct guiHeader header;
    uint8_t state;
    uint8_t num_items;
    uint8_t cur_pos;
    buttonAction_t action;
    struct guiObject *parent;
    int (*callback)(struct guiObject *obj, uint8_t pos, int8_t dir, void *data);
    void *cb_data;
} guiScrollbar_t;

typedef struct guiButton {
    struct guiHeader header;
    struct LabelDesc desc;
    const struct ImageMap *image;
    uint16_t fontColor;
    const char *(*strCallback)(struct guiObject *obj, const void *data);
    void (*CallBack)(struct guiObject *obj, const void *data);
    const void *cb_data;
    uint8_t enable;
} guiButton_t;

typedef struct guiListbox {
    struct guiHeader header;
    struct LabelDesc desc;
    uint8_t text_height;
    uint8_t entries_per_page;
    uint8_t item_count;
    uint8_t cur_pos;
    int16_t selected;
    struct guiScrollbar scrollbar;
    const char * (*string_cb)(uint8_t idx, void * data);
    void (*select_cb)(struct guiObject *obj, uint16_t selected, void * data);
    void (*longpress_cb)(struct guiObject *obj, uint16_t selected, void * data);
    void *cb_data;
    enum ListBoxType style;
    enum ListBoxNavigateKeyType key_style;
    buttonAction_t action; // fix bug for issue #81: DEVO10: Model list should be browsable with UP/DOWN
} guiListbox_t;

typedef struct guiScrollable {
    struct guiHeader header;
    uint8_t item_count;
    uint8_t row_height;
    uint8_t num_selectable;
    uint8_t cur_row;
    uint8_t visible_rows;
    uint8_t max_visible_rows;
    guiObject_t *head;
    struct guiScrollbar scrollbar;
    int (*row_cb)(int absrow, int relrow, int x, void *data);
    guiObject_t *(*getobj_cb)(int relrow, int col, void *data);
    int (*size_cb)(int absrow, void *data);
    void *cb_data;
    //buttonAction_t action;
}  guiScrollable_t;
 
typedef struct guiXYGraph {
    struct guiHeader header;
    int16_t min_x;
    int16_t min_y;
    int16_t max_x;
    int16_t max_y;
    uint16_t grid_x;
    uint16_t grid_y;
    int16_t (*CallBack)(int16_t xval, void * data);
    uint8_t (*point_cb)(int16_t *x, int16_t *y, uint8_t pos, void *data);
    uint8_t (*touch_cb)(int16_t x, int16_t y, void *data);
    void *cb_data;
} guiXYGraph_t;

typedef struct guiBarGraph {
    struct guiHeader header;
    int16_t min;
    int16_t max;
    uint8_t direction;
    int16_t (*CallBack)(void * data);
    void *cb_data;
} guiBarGraph_t;

typedef struct guiDialog {
    struct guiHeader header;
    struct guiBox txtbox;
    guiButton_t but1;
    guiButton_t but2;
    const char *title;
    const char *(*string_cb)(guiObject_t *obj, void *data);
    void (*CallBack)(uint8_t state, void *data);
    void *cbData;
} guiDialog_t;

typedef struct guiTextSelect {
    struct guiHeader header;
    const struct ImageMap *button;
    uint8_t state;
    enum TextSelectType type;
    uint16_t fontColor;
    const char *(*ValueCB)(guiObject_t *obj, int dir, void *data);
    void (*SelectCB)(guiObject_t *obj, void *data);
    void *cb_data;
    struct LabelDesc desc;
    uint8_t enable;
} guiTextSelect_t;

typedef struct guiRect {
    struct guiHeader header;
    struct LabelDesc desc;
    void (*CallBack)(guiObject_t *, const void *data);
    const void *cbData;
} guiRect_t;

#define OBJ_IS_USED(x)        (((guiObject_t *)(x))->flags & 0x01) /* bool: UI element is in use */
#define OBJ_IS_HIDDEN(x)      ((x)->flags & 0x02) /* bool: UI element is not visible */
#define OBJ_IS_MODAL(x)       ((x)->flags & 0x04) /* bool: UI element is active and all non-model elements are not */
#define OBJ_IS_DIRTY(x)       ((x)->flags & 0x08) /* bool: UI element needs redraw */
#define OBJ_IS_TRANSPARENT(x) ((x)->flags & 0x10) /* bool: UI element has transparency */
#define OBJ_IS_SELECTABLE(x)  ((x)->flags & 0x20) /* bool: UI element can be selected */
#define OBJ_IS_SCROLLABLE(x)  ((x)->flags & 0x40) /* bool: UI element is part of a scrollable container */
#define OBJ_SET_FLAG(obj,flag,set)  ((guiObject_t *)(obj))->flags = (set) \
                                    ? ((guiObject_t *)(obj))->flags | (flag) \
                                    : ((guiObject_t *)(obj))->flags & ~(flag)
#define OBJ_SET_USED(x,y)        OBJ_SET_FLAG(x, 0x01, y)
#define OBJ_SET_HIDDEN(x,y)      OBJ_SET_FLAG(x, 0x02, y)
#define OBJ_SET_MODAL(x,y)       OBJ_SET_FLAG(x, 0x04, y)
#define OBJ_SET_DIRTY(x,y)       OBJ_SET_FLAG(x, 0x08, y)
#define OBJ_SET_TRANSPARENT(x,y) OBJ_SET_FLAG(x, 0x10, y)
#define OBJ_SET_SELECTABLE(x,y)  OBJ_SET_FLAG(x, 0x20, y)
#define OBJ_SET_SCROLLABLE(x,y)  OBJ_SET_FLAG(x, 0x40, y)
#define CLEAR_OBJ(x) memset((x), 0, sizeof(*(x)))

#define DRAW_NORMAL  0
#define DRAW_PRESSED 1
/* internal use only */
extern struct guiObject *objHEAD;
extern struct guiObject *objTOUCHED;
extern struct guiObject *objSELECTED;
extern struct guiObject *objDIALOG;
extern struct guiObject *objModalButton;

guiScrollable_t *GUI_FindScrollableParent(guiObject_t *obj);
void GUI_RemoveScrollableObjs(guiObject_t *obj);
void GUI_DrawScrollable(guiObject_t *obj);
guiObject_t *GUI_ScrollableGetNextSelectable(guiScrollable_t *scrollable, guiObject_t *obj);
guiObject_t *GUI_ScrollableGetPrevSelectable(guiScrollable_t *scrollable, guiObject_t *obj);
int GUI_ScrollableGetObjRowOffset(guiScrollable_t *scrollable, guiObject_t *obj);
int GUI_ScrollableCurrentRow(guiScrollable_t *scrollable);
int GUI_ScrollableVisibleRows(guiScrollable_t *scrollable);


void GUI_DrawKeyboard(struct guiObject *obj);
uint8_t GUI_TouchKeyboard(struct guiObject *obj, struct touch *coords, int8_t press_type);

void GUI_DrawTextSelect(struct guiObject *obj);
uint8_t GUI_TouchTextSelect(struct guiObject *obj, struct touch *coords, int8_t press_type);
void GUI_PressTextSelect(struct guiObject *obj, uint32_t button, uint8_t press_type);

void GUI_DrawXYGraph(struct guiObject *obj);
uint8_t GUI_TouchXYGraph(struct guiObject *obj, struct touch *coords, uint8_t long_press);

void GUI_DrawListbox(struct guiObject *obj, uint8_t redraw_all);
uint8_t GUI_TouchListbox(struct guiObject *obj, struct touch *coords, uint8_t long_press);
void GUI_PressListbox(struct guiObject *obj, uint32_t button, uint8_t press_type);

void GUI_DrawLabel(struct guiObject *obj);
uint8_t GUI_TouchLabel(struct guiObject *obj, struct touch *coords, int8_t press_type);

void GUI_DrawDialog(struct guiObject *obj);
void GUI_DialogDrawBackground(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void DialogClose(struct guiObject *obj, uint8_t state);

void GUI_DrawImage(struct guiObject *obj);
uint8_t GUI_TouchImage(struct guiObject *obj, struct touch *coords, int8_t press_type);

void GUI_DrawButton(struct guiObject *obj);
void GUI_DrawBarGraph(struct guiObject *obj);
void GUI_DrawScrollbar(struct guiObject *obj);
uint8_t GUI_TouchScrollbar(struct guiObject *obj, struct touch *coords, int8_t press_type);

void GUI_DrawRect(struct guiObject *obj);

void GUI_DrawObject(struct guiObject *obj);
void GUI_DrawBackground(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void GUI_DrawImageHelper(uint16_t x, uint16_t y, const struct ImageMap *map, uint8_t idx);
uint8_t coords_in_box(struct guiBox *box, struct touch *coords);
void connect_object(struct guiObject *obj);
void GUI_HandleModalButtons(uint8_t enable);
int GUI_ButtonWidth(enum ButtonType type);
int GUI_ButtonHeight(enum ButtonType type);

guiObject_t *GUI_CreateDialog(guiDialog_t *,uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *title,
        const char *(*string_cb)(guiObject_t *obj, void *data),
        void (*CallBack)(uint8_t state, void *data),
        enum DialogType dgType, void *data);
#define GUI_CreateLabel(obj, x, y, cb, desc, data) GUI_CreateLabelBox(obj, x, y, 0, 0, &desc, cb, NULL, data)
guiObject_t *GUI_CreateLabelBox(guiLabel_t *,uint16_t x, uint16_t y, uint16_t width, uint16_t height, const struct LabelDesc *desc,
             const char *(*strCallback)(guiObject_t *, const void *),
             void (*pressCallback)(guiObject_t *obj, int8_t press_type, const void *data),
             const void *data);
void GUI_DrawLabelHelper(uint16_t obj_x, uint16_t obj_y, uint16_t obj_width, uint16_t obj_height,
        const char *str, const struct LabelDesc *desc, uint8_t is_selected);
void GUI_SetLabelDesc(guiLabel_t *obj, struct LabelDesc *desc);

#define GUI_CreateImage(obj, x, y, w,h, file) GUI_CreateImageOffset(obj, x, y, w, h, 0, 0, file, NULL, NULL)
guiObject_t *GUI_CreateImageOffset(guiImage_t *, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t x_off, uint16_t y_off, const char *file,
        void (*CallBack)(guiObject_t *obj, int8_t press_type, const void *data), const void *cb_data);

guiObject_t *GUI_CreateButton(guiButton_t *, uint16_t x, uint16_t y, enum ButtonType type,
        const char *(*strCallback)(guiObject_t *, const void *),
        uint16_t fontColor, void (*CallBack)(guiObject_t *obj, const void *data), const void *cb_data);
guiObject_t *GUI_CreateButtonPlateText(guiButton_t *, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const struct LabelDesc *desc,
        const char *(*strCallback)(guiObject_t *, const void *),
        uint16_t fontColor, void (*CallBack)(guiObject_t *obj, const void *data), const void *cb_data);
guiObject_t *GUI_CreateIcon(guiButton_t *, uint16_t x, uint16_t y, const struct ImageMap *image,
        void (*CallBack)(guiObject_t *obj, const void *data), const void *cb_data);
void GUI_ButtonEnable(guiObject_t *obj, uint8_t enable);
uint8_t GUI_IsButtonEnabled(guiObject_t *obj);

guiObject_t *GUI_CreateListBox(guiListbox_t *, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t item_count, int16_t selected,
        const char *(*string_cb)(uint8_t idx, void *data),
        void (*select_cb)(guiObject_t *obj, uint16_t selected, void *data),
        void (*longpress_cb)(guiObject_t *obj, uint16_t selected, void *data),
        void *cb_data);
guiObject_t *GUI_CreateListBoxPlateText(guiListbox_t *, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t item_count, int16_t selected,
        const struct LabelDesc *desc, enum ListBoxNavigateKeyType keyType,
        const char *(*string_cb)(uint8_t idx, void *data),
        void (*select_cb)(guiObject_t *obj, uint16_t selected, void *data),
        void (*longpress_cb)(guiObject_t *obj, uint16_t selected, void *data),
        void *cb_data);
void GUI_ListBoxSelect(guiListbox_t *obj, uint16_t selected);

guiObject_t *GUI_CreateScrollable(guiScrollable_t *scrollable, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t row_height, uint8_t item_count,
     int (*row_cb)(int absrow, int relrow, int x, void *data),
     guiObject_t * (*getobj_cb)(int relrow, int col, void *data),
     int (*size_cb)(int absrow, void *data),
     void *data);

guiObject_t *GUI_GetScrollableObj(guiScrollable_t *, int row, int col);
guiObject_t *GUI_ShowScrollableRowCol(guiScrollable_t *scrollable, int absrow, int col);
guiObject_t *GUI_ShowScrollableRowOffset(guiScrollable_t *scrollable, int row_idx);

guiObject_t *GUI_CreateXYGraph(guiXYGraph_t *, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                      int16_t min_x, int16_t min_y, int16_t max_x, int16_t max_y,
                      uint16_t gridx, uint16_t gridy,
                      int16_t (*Callback)(int16_t xval, void *data), 
                      uint8_t (*point_cb)(int16_t *x, int16_t *y, uint8_t pos, void *data),
                      uint8_t (*touch_cb)(int16_t x, int16_t y, void *data),
                      void *cb_data);
guiObject_t *GUI_CreateBarGraph(guiBarGraph_t *, uint16_t x, uint16_t y, uint16_t width, uint16_t height, int16_t min,
        int16_t max, uint8_t direction, int16_t (*Callback)(void * data), void * cb_data);
guiObject_t *GUI_CreateTextSelect(guiTextSelect_t *, uint16_t x, uint16_t y, enum TextSelectType type,
        void (*select_cb)(guiObject_t *obj, void *data),
        const char *(*value_cb)(guiObject_t *obj, int value, void *data),
        void *cb_data);
guiObject_t *GUI_CreateTextSelectPlate(guiTextSelect_t *, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const struct LabelDesc *desc,
        void (*select_cb)(guiObject_t *obj, void *data),
        const char *(*value_cb)(guiObject_t *obj, int value, void *data),
        void *cb_data);
guiObject_t *GUI_CreateKeyboard(guiKeyboard_t *, enum KeyboardType type, char *text, int32_t max_size,
        void (*CallBack)(guiObject_t *obj, void *data), void *cb_data);

guiObject_t *GUI_CreateScrollbar(guiScrollbar_t *, uint16_t x, uint16_t y, uint16_t height,
        uint8_t num_items, guiObject_t *parent,
        int (*press_cb)(guiObject_t *parent, uint8_t pos, int8_t direction, void *data), void *data);
void GUI_SetScrollbar(guiScrollbar_t *obj, uint8_t pos);
uint8_t GUI_GetScrollbarNumItems(guiScrollbar_t *obj);

guiObject_t *GUI_CreateRect(guiRect_t *, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const struct LabelDesc *desc);
guiObject_t *GUI_CreateRectCB(guiRect_t *, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const struct LabelDesc *desc, void (*)(guiObject_t *, const void *), const void *);

uint8_t GUI_CheckTouch(struct touch *coords, uint8_t long_press);
void GUI_TouchRelease(void);
void GUI_DrawScreen(void);
void GUI_RefreshScreen(void);
void _GUI_Redraw(guiObject_t *obj);
#define GUI_Redraw(x) _GUI_Redraw((guiObject_t *)(x))
void GUI_RedrawAllObjects(void);
void GUI_RemoveObj(guiObject_t *obj);
void GUI_RemoveAllObjects(void);
void GUI_RemoveHierObjects(guiObject_t *obj);
void GUI_DrawObjects(void);
void GUI_SetHidden(guiObject_t *obj, uint8_t state);
guiObject_t *GUI_GetSelected(void);
uint8_t GUI_IsSelectable(guiObject_t *obj);
void GUI_SetSelected(guiObject_t *obj);
void GUI_SetSelectable(guiObject_t *obj, uint8_t selectable);
uint8_t GUI_ObjectNeedsRedraw(guiObject_t *obj);
guiObject_t *GUI_IsModal(void);
void GUI_HandleButtons(uint8_t enable);
struct guiObject *GUI_GetNextSelectable(struct guiObject *origObj);
struct guiObject *GUI_GetPrevSelectable(struct guiObject *origObj);
void GUI_GetSize(guiObject_t *obj, int *width, int *height);

int32_t GUI_TextSelectHelper(int32_t value, int32_t min, int32_t max, int8_t dir, uint32_t shortstep, uint32_t longstep, uint8_t *_changed);
void GUI_TextSelectEnablePress(guiTextSelect_t *obj, uint8_t enable);
void GUI_TextSelectEnable(guiTextSelect_t *obj, uint8_t enable);
uint8_t GUI_IsTextSelectEnabled(struct guiObject *obj);
void GUI_ChangeImage(guiImage_t *obj, const char *file, uint16_t x_off, uint16_t y_off);
void GUI_ReplaceImage(guiImage_t *obj, const char *file, uint16_t x_off, uint16_t y_off);

// logical view, only available in text-based LCD, such as devo10
struct viewObject {
    /*This is a hack to allow use of OBJ_* macros */
    /*This provides the same layout as guiHeader */
    uint16_t origin_absoluteX;
    uint16_t origin_absoluteY;
    uint16_t width;
    uint16_t height;
    struct viewObject *next;
    enum GUIType Type;
    uint8_t flags;
    /*end*/
    int16_t orgin_relativeX;  // can be negative to indicate the whole view is hidden
    int16_t orgin_relativeY; // can be negative to indicate the whole view is hidden
    uint16_t boundary;
};
typedef struct viewObject viewObject_t;

#define LOGICALVIEW_COUNT 3
#define LOGICAL_VIEW_BOUNDARY 5000 // a coordinate that is >= 5000 is a relative coordinate
#define GUI_IsLogicViewCoordinate(x) (x)>= LOGICAL_VIEW_BOUNDARY
uint8_t GUI_IsObjectInsideCurrentView(uint8_t view_id, struct guiObject *obj);
uint8_t GUI_IsCoordinateInsideLogicalView(uint16_t *x, uint16_t *y);
void GUI_SetupLogicalView(uint8_t view_id, uint16_t view_orgin_relativeX, uint16_t view_orgin_relativeY, uint8_t width, uint8_t height,
        uint8_t view_origin_absoluteX, uint8_t view_origin_absoluteY);
uint16_t GUI_MapToLogicalView(uint8_t view_id, uint16_t x_or_y);
void GUI_SetRelativeOrigin(uint8_t view_id, int16_t new_originX, int16_t new_originY);
void GUI_ScrollLogicalView(uint8_t view_id, int16_t y_offset);
void GUI_ScrollLogicalViewToObject(uint8_t view_id, struct guiObject *obj, int8_t direction);
int16_t GUI_GetLogicalViewOriginRelativeY(uint8_t view_id);
void GUI_Select1stSelectableObj(void);
int8_t GUI_GetViewId(int16_t x, int16_t y);
void GUI_ViewInit(void);
int GUI_IsEmpty(void);
void GUI_SelectionNotify(void (*notify_cb)(guiObject_t *obj));
#endif /* GUI_H_ */
