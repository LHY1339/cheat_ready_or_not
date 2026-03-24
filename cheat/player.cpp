#include "player.h"
#include "gvalue.h"
#include "sdk/SDK/BasePlayer_classes.hpp"
#include "sdk/SDK/Engine_classes.hpp"
#include "kismet.h"

player* player::get()
{
	static player inst;
	return &inst;
}

void player::init()
{

}

void player::main()
{
	if (!gvalue::controller->Pawn ||
		!gvalue::controller->Pawn->IsA(SDK::ABasePlayer_C::StaticClass()))
	{
		return;
	}

	SDK::ABasePlayer_C* player = static_cast<SDK::ABasePlayer_C*>(gvalue::controller->Pawn);

	player->CustomTimeDilation = kismet::fround(gvalue::global_speed);
}