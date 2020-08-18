#include "Key.h"
#include <controls.h>
#include <unordered_map>

using KeyParamMap = std::unordered_map<Key, Param>;
extern KeyParamMap increaseMap;
extern KeyParamMap decreaseMap;

void keyStore();
void keyLoad();