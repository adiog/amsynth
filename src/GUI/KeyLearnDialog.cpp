#include "KeyLearnDialog.h"

#include "KeyHandler.h"
#include "KeyMapper.h"

#include "../MidiController.h"
#include "../PresetController.h"
#include "controllers.h"

#include <cassert>
#include <glib/gi18n.h>


KeyLearnDialog::KeyLearnDialog(KeyBehaviour keyBehaviour, MidiController *midiController, PresetController *presetController, GtkWindow *parent)
: _dialog(nullptr)
, _midiController(midiController)
, _presetController(presetController)
,   keyBehaviour(keyBehaviour)
{
    _dialog = gtk_dialog_new_with_buttons(_("Key Controller Assignment"), parent, GTK_DIALOG_MODAL,
    GTK_STOCK_OK,     GTK_RESPONSE_ACCEPT,
    GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
    NULL);

    buffer = gtk_text_buffer_new (NULL);
  _key_name = gtk_text_view_new_with_buffer(buffer);
    gtk_text_buffer_set_text(buffer, "None", 4);


  GtkWidget *table = gtk_table_new(3, 2, FALSE);
  gtk_table_attach(GTK_TABLE(table), gtk_label_new(_("Key Controller")),  0, 1, 1, 2, GTK_FILL, GTK_FILL, 5, 5);
  gtk_table_attach(GTK_TABLE(table), _key_name,                               1, 2, 1, 2, GTK_FILL, GTK_FILL, 5, 5);

  GtkWidget *content = gtk_dialog_get_content_area (GTK_DIALOG (_dialog));
  gtk_box_pack_start(GTK_BOX(content), table, TRUE, TRUE, 0);

  _midiController->getLastControllerParam().addUpdateListener(this);

  gtk_widget_add_events(_dialog, GDK_KEY_PRESS_MASK);
  g_signal_connect(G_OBJECT(_dialog), "key_press_event", G_CALLBACK(keypress_function), NULL);
}

KeyLearnDialog::~KeyLearnDialog()
{
  _midiController->getLastControllerParam().removeUpdateListener(this);
}

void
KeyLearnDialog::run_modal(Param param_idx)
{
    keyHandleMode = KeyHandleMode::Learn;
    keyLearnBehaviour = keyBehaviour;
    keyLearnParam = param_idx;

    auto & refMap = keyBehaviour == KeyBehaviour::Increase ? increaseMap : decreaseMap;
    for(auto & entry : refMap) {
        if (entry.second == keyLearnParam) {
            auto currentKey = refMap[keyLearnParam];
            char text[2];
            sprintf(text, "%c", entry.first);
            gtk_text_buffer_set_text(buffer, text, 1);
            break;
        }
    }

    gtk_widget_show_all(_dialog);
  const gint response = gtk_dialog_run(GTK_DIALOG(_dialog));
  gtk_widget_hide(_dialog);

  keyStore();
  keyHandleMode = KeyHandleMode::Operate;
}

void
KeyLearnDialog::update()
{
    g_idle_add(KeyLearnDialog::key_assigned, this);
}

gboolean
KeyLearnDialog::key_assigned(gpointer data)
{
  KeyLearnDialog *dialog = (KeyLearnDialog *) data;
  char text[2];
  sprintf(text, "%c", lastKeyAssigned);
  gtk_text_buffer_set_text(dialog->buffer, text, 1);
  return G_SOURCE_REMOVE;
}
