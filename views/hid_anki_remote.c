#include "hid_anki_remote.h"
#include <gui/elements.h>
#include "../hid.h"

#include "hid_icons.h"

#define TAG "HidAnkiRemote"

struct HidAnkiRemote {
    View* view;
    Hid* hid;
};

typedef struct {
    bool left_pressed;
    bool up_pressed;
    bool right_pressed;
    bool down_pressed;
    bool ok_pressed;
    bool back_pressed;
    bool connected;
    HidTransport transport;
} HidAnkiRemoteModel;


static void hid_anki_remote_draw_callback(Canvas* canvas, void* context) {
    furi_assert(context);
    HidAnkiRemoteModel* model = context;

    // Header
    if(model->transport == HidTransportBle) {
        if(model->connected) {
            canvas_draw_icon(canvas, 0, 0, &I_Ble_connected_15x15);
        } else {
            canvas_draw_icon(canvas, 0, 0, &I_Ble_disconnected_15x15);
        }
    }

    canvas_set_font(canvas, FontPrimary);
    elements_multiline_text_aligned(canvas, 17, 3, AlignLeft, AlignTop, "Anki");

    canvas_draw_icon(canvas, 68, 2, &I_Pin_back_arrow_10x8);
    canvas_set_font(canvas, FontSecondary);
    elements_multiline_text_aligned(canvas, 127, 3, AlignRight, AlignTop, "Hold to exit");

    // Up
    canvas_draw_icon(canvas, 21, 24, &I_Button_18x18);
    if(model->up_pressed) {
        elements_slightly_rounded_box(canvas, 24, 26, 13, 13);
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_str(canvas, 28, 35, "2");
    canvas_set_color(canvas, ColorBlack);

    // Down
    canvas_draw_icon(canvas, 21, 45, &I_Button_18x18);
    if(model->down_pressed) {
        elements_slightly_rounded_box(canvas, 24, 47, 13, 13);
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_str(canvas, 28, 57, "3");
    canvas_set_color(canvas, ColorBlack);

    // Left
    canvas_draw_icon(canvas, 0, 45, &I_Button_18x18);
    if(model->left_pressed) {
        elements_slightly_rounded_box(canvas, 3, 47, 13, 13);
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_str(canvas, 8, 57, "1");
    canvas_set_color(canvas, ColorBlack);

    // Right
    canvas_draw_icon(canvas, 42, 45, &I_Button_18x18);
    if(model->right_pressed) {
        elements_slightly_rounded_box(canvas, 45, 47, 13, 13);
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_str(canvas, 50, 57, "4");
    canvas_set_color(canvas, ColorBlack);

    // Ok
    canvas_draw_icon(canvas, 63, 25, &I_Space_65x18);
    if(model->ok_pressed) {
        elements_slightly_rounded_box(canvas, 66, 27, 60, 13);
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_icon(canvas, 74, 29, &I_Ok_btn_9x9);
    elements_multiline_text_aligned(canvas, 91, 36, AlignLeft, AlignBottom, "Space");
    canvas_set_color(canvas, ColorBlack);

    // Back
    canvas_draw_icon(canvas, 63, 45, &I_Space_65x18);
    if(model->back_pressed) {
        elements_slightly_rounded_box(canvas, 66, 47, 60, 13);
        canvas_set_color(canvas, ColorWhite);
    }
    canvas_draw_icon(canvas, 74, 49, &I_Pin_back_arrow_10x8);
    elements_multiline_text_aligned(canvas, 91, 57, AlignLeft, AlignBottom, "Back");
}

static void hid_anki_remote_process(HidAnkiRemote* hid_anki_remote, InputEvent* event) {
    with_view_model(
        hid_anki_remote->view,
        HidAnkiRemoteModel * model,
        {
            if(event->type == InputTypePress) {
                if(event->key == InputKeyUp) {
                    model->up_pressed = true;
                    hid_hal_keyboard_press(hid_anki_remote->hid, HID_KEYBOARD_2);
                } else if(event->key == InputKeyDown) {
                    model->down_pressed = true;
                    hid_hal_keyboard_press(hid_anki_remote->hid, HID_KEYBOARD_3);
                } else if(event->key == InputKeyLeft) {
                    model->left_pressed = true;
                    hid_hal_keyboard_press(hid_anki_remote->hid, HID_KEYBOARD_1);
                } else if(event->key == InputKeyRight) {
                    model->right_pressed = true;
                    hid_hal_keyboard_press(hid_anki_remote->hid, HID_KEYBOARD_4);
                } else if(event->key == InputKeyOk) {
                    model->ok_pressed = true;
                    hid_hal_keyboard_press(hid_anki_remote->hid, HID_KEYBOARD_SPACEBAR);
                } else if(event->key == InputKeyBack) {
                    model->back_pressed = true;
                }
            } else if(event->type == InputTypeRelease) {
                if(event->key == InputKeyUp) {
                    model->up_pressed = false;
                    hid_hal_keyboard_release(hid_anki_remote->hid, HID_KEYBOARD_2);
                } else if(event->key == InputKeyDown) {
                    model->down_pressed = false;
                    hid_hal_keyboard_release(hid_anki_remote->hid, HID_KEYBOARD_3);
                } else if(event->key == InputKeyLeft) {
                    model->left_pressed = false;
                    hid_hal_keyboard_release(hid_anki_remote->hid, HID_KEYBOARD_1);
                } else if(event->key == InputKeyRight) {
                    model->right_pressed = false;
                    hid_hal_keyboard_release(hid_anki_remote->hid, HID_KEYBOARD_4);
                } else if(event->key == InputKeyOk) {
                    model->ok_pressed = false;
                    hid_hal_keyboard_release(hid_anki_remote->hid, HID_KEYBOARD_SPACEBAR);
                } else if(event->key == InputKeyBack) {
                    model->back_pressed = false;
                }
            } else if(event->type == InputTypeShort) {
                if(event->key == InputKeyBack) {
                    hid_hal_keyboard_press(hid_anki_remote->hid, HID_KEYBOARD_L_GUI);
                    hid_hal_keyboard_press(hid_anki_remote->hid, HID_KEYBOARD_Z);
                    hid_hal_keyboard_release(hid_anki_remote->hid, HID_KEYBOARD_Z);
                    hid_hal_keyboard_release(hid_anki_remote->hid, HID_KEYBOARD_L_GUI);
                    hid_hal_consumer_key_press(hid_anki_remote->hid, HID_CONSUMER_AC_BACK);
                    hid_hal_consumer_key_release(hid_anki_remote->hid, HID_CONSUMER_AC_BACK);
                }
            }
        },
        true);
}

static bool hid_anki_remote_input_callback(InputEvent* event, void* context) {
    furi_assert(context);
    HidAnkiRemote* hid_anki_remote = context;
    bool consumed = false;

    if(event->type == InputTypeLong && event->key == InputKeyBack) {
        hid_hal_keyboard_release_all(hid_anki_remote->hid);
    } else {
        hid_anki_remote_process(hid_anki_remote, event);
        consumed = true;
    }

    return consumed;
}

HidAnkiRemote* hid_anki_remote_alloc(Hid* hid) {
    HidAnkiRemote* hid_anki_remote = malloc(sizeof(HidAnkiRemote));
    hid_anki_remote->view = view_alloc();
    hid_anki_remote->hid = hid;
    view_set_context(hid_anki_remote->view, hid_anki_remote);
    view_allocate_model(hid_anki_remote->view, ViewModelTypeLocking, sizeof(HidAnkiRemoteModel));
    view_set_draw_callback(hid_anki_remote->view, hid_anki_remote_draw_callback);
    view_set_input_callback(hid_anki_remote->view, hid_anki_remote_input_callback);

    with_view_model(
        hid_anki_remote->view, HidAnkiRemoteModel * model, { model->transport = hid->transport; }, true);

    return hid_anki_remote;
}

void hid_anki_remote_free(HidAnkiRemote* hid_anki_remote) {
    furi_assert(hid_anki_remote);
    view_free(hid_anki_remote->view);
    free(hid_anki_remote);
}

View* hid_anki_remote_get_view(HidAnkiRemote* hid_anki_remote) {
    furi_assert(hid_anki_remote);
    return hid_anki_remote->view;
}

void hid_anki_remote_set_connected_status(HidAnkiRemote* hid_anki_remote, bool connected) {
    furi_assert(hid_anki_remote);
    with_view_model(
        hid_anki_remote->view, HidAnkiRemoteModel * model, { model->connected = connected; }, true);
}
