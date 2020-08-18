#pragma once

#include "../UpdateListener.h"

#include <gtk/gtk.h>
#include "KeyHandler.h"

class MidiController;
class PresetController;

class KeyLearnDialog : public UpdateListener
{
public:

	KeyLearnDialog(KeyBehaviour keyBehaviour, MidiController *midiController, PresetController *presetController, GtkWindow *parent);
	~KeyLearnDialog();

	void run_modal(Param param_idx);

	void update() override;

	static gboolean key_assigned(gpointer data);

private:

    KeyBehaviour keyBehaviour;

	GtkWidget		*_dialog;
	GtkWidget		*_key_name;

    GtkTextBuffer *buffer;

	MidiController	*_midiController;
	PresetController *_presetController;
};
