#include "KeyMapper.h"

KeyParamMap increaseMap;
KeyParamMap decreaseMap;

template<typename Map>
void save(const std::string &filename, Map &map) {
    FILE *f = fopen(filename.c_str(), "wb");
    using KeyType = typename Map::key_type;
    using ValType = typename Map::mapped_type;
    for (auto &entry : map) {
        fwrite(&entry.first, sizeof(KeyType), 1, f);
        fwrite(&entry.second, sizeof(ValType), 1, f);
    }
    fclose(f);
}

template<typename Map>
void load(const std::string &filename, Map &map) {
    FILE *f = fopen(filename.c_str(), "rb");
    if (!f) {
        return;
    }
    using KeyType = typename Map::key_type;
    using ValType = typename Map::mapped_type;
    KeyType key;
    ValType val;
    while (fread(&key, sizeof(KeyType), 1, f)) {
        fread(&val, sizeof(ValType), 1, f);
        map[key] = val;
    }
    fclose(f);
}

void keyStore() {
    save("inc.map", increaseMap);
    save("dec.map", decreaseMap);
}

void keyLoad() {
    load("inc.map", increaseMap);
    load("dec.map", decreaseMap);
}
