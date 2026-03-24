#include "color.h"

color* color::get()
{
	static color inst;
	return &inst;
}