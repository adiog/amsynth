#pragma once

#include <gtk/gtk.h>
#include <controls.h>
#include <unordered_map>
#include "Key.h"

enum class KeyHandleMode {
    Operate,
    Learn
};

enum class KeyBehaviour {
    Increase,
    Decrease
};

extern Param keyLearnParam;
extern KeyBehaviour keyLearnBehaviour;
extern KeyHandleMode keyHandleMode;

extern Key lastKeyAssigned;

gboolean keypress_function(GtkWidget *widget, GdkEventKey *event, gpointer data);

