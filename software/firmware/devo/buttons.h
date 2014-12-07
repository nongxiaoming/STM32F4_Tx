#ifndef _BUTTONS_H_
#define _BUTTONS_H_

struct buttonAction {
    uint32_t button;
    uint8_t flags;
    unsigned (*callback)(uint32_t button, unsigned flags, void *data);
    void *data;
    struct buttonAction *next;
};

enum ButtonFlags {
    BUTTON_PRESS         = 0x01,
    BUTTON_RELEASE       = 0x02,
    BUTTON_LONGPRESS     = 0x04,
    BUTTON_REPEAT        = 0x20,
    BUTTON_HAD_LONGPRESS = 0x40,
    BUTTON_PRIORITY      = 0x80,
};

typedef struct buttonAction buttonAction_t;

unsigned BUTTON_RegisterCallback(buttonAction_t *action, uint32_t button, unsigned flags,
                 unsigned (*callback)(uint32_t button, unsigned flags, void *data), void *data);
void BUTTON_UnregisterCallback(buttonAction_t *action);
void BUTTON_Handler(void);
void BUTTON_InterruptLongPress(void);
#endif
