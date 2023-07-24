#pragma once

#include <gui/view.h>

typedef struct Hid Hid;
typedef struct HidAnkiRemote HidAnkiRemote;

HidAnkiRemote* hid_anki_remote_alloc(Hid* bt_hid);

void hid_anki_remote_free(HidAnkiRemote* hid_anki_remote);

View* hid_anki_remote_get_view(HidAnkiRemote* hid_anki_remote);

void hid_anki_remote_set_connected_status(HidAnkiRemote* hid_anki_remote, bool connected);
