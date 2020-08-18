#include "KeyHandler.h"
#include "MidiController.h"
#include "KeyDialogUpdater.h"
#include <gdk/gdkevents.h>
#include <src/main.h>
#include <iostream>
#include "KeyMapper.h"

struct KeyLearnDialog;
extern KeyLearnDialog *keyIncreaseLearnDialog;
extern KeyLearnDialog *keyDecreaseLearnDialog;

Param keyLearnParam;
KeyBehaviour keyLearnBehaviour;
KeyHandleMode keyHandleMode = KeyHandleMode::Operate;


Key lastKeyAssigned;

namespace {
    void increase(Param paramId) {
        auto mc = s_synthesizer->getMidiController();
        auto pc = mc->getPresetController();
        pc->getCurrentPreset().getParameter(paramId).increase();
        //std::cout << "increase " << paramId << std::endl;
    }

    void decrease(Param paramId) {
        auto mc = s_synthesizer->getMidiController();
        auto pc = mc->getPresetController();
        pc->getCurrentPreset().getParameter(paramId).decrease();
        //std::cout << "decrease " << paramId << std::endl;
    }
}

gboolean keypress_function(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    Key key = event->keyval;

    if (keyHandleMode == KeyHandleMode::Operate) {
        if (increaseMap.find(key) != increaseMap.end()) {
            increase(increaseMap[key]);
            return TRUE;
        }
        if (decreaseMap.find(key) != decreaseMap.end()) {
            decrease(decreaseMap[key]);
            return TRUE;
        }
    }

    if (keyHandleMode == KeyHandleMode::Learn) {
        increaseMap.erase(key);
        decreaseMap.erase(key);
        lastKeyAssigned = key;
        if (keyLearnBehaviour == KeyBehaviour::Increase) {
            //std::cout << "learn increase " << keyLearnParam << std::endl;
            increaseMap.insert(std::make_pair(key, keyLearnParam));
            updateKeyLearnDialog(keyIncreaseLearnDialog);
        }
        if (keyLearnBehaviour == KeyBehaviour::Decrease) {
            //std::cout << "learn decrease " << keyLearnParam << std::endl;
            decreaseMap.insert(std::make_pair(key, keyLearnParam));
            updateKeyLearnDialog(keyDecreaseLearnDialog);
        }
        return TRUE;
    }

    return FALSE;
}
