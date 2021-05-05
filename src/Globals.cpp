#include "Globals.h"
Globals::Globals() {}

int Globals::getLoOrderKey() {
    return ++loOrderKey;
}

int Globals::getBatch() {
    return ++batch;
}