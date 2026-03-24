#include "cheat.h"
#include "cheat_mod.h"
#include "gui.h"
#include "menu.h"
#include "network.h"
#include "save.h"
#include "gdefine.h"
#include "esp.h"
#include "visual.h"
#include "aim.h"
#include "player.h"
#include "manager.h"

#include <Windows.h>

cheat* cheat::get()
{
	static cheat inst;
	return &inst;
}

void cheat::main()
{
	cheat_mod::save();
	cheat_mod::init();
	cheat_mod::hook();
	cheat_mod::exit();
}

void cheat::module_init()
{
	manager::get()->init();
	network::get()->init();
	esp::get()->init();
	visual::get()->init();
	aim::get()->init();
	player::get()->init();
}

void cheat::module_main()
{
	__try
	{
		gui::main();
		network::get()->main();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		printf("framework error : %d\n", GetExceptionCode());
	}

	__try
	{
		manager::get()->main();
		esp::get()->main();
		visual::get()->main();
		aim::get()->main();
		player::get()->main();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		printf("module error : %d\n", GetExceptionCode());
	}

	__try
	{
		menu::get()->main();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		printf("menu error : %d\n", GetExceptionCode());
	}
}