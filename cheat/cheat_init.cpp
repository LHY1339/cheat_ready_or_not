#include "cheat_mod.h"
#include "gdefine.h"
#include "cheat.h"

#include <iostream>
#include <Windows.h>

void cheat_mod::init()
{
#ifdef CHEAT_DEBUG
    AllocConsole();
    FILE* value = freopen("CONOUT$", "w+", stdout);
#endif

    cheat::get()->module_init();
}
