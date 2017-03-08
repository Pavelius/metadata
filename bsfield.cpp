#include "bsfield.h"

extern "C" int strcmp(const char* s1, const char* s2);

bsfield number_type[2] = {{"number"}, {0}};
bsfield text_type[2] = {{"text"}, {0}};;

const bsfield* bsfield::find(const char* name) const
{
	if(!this)
		return 0;
	for(auto p = this; p->identifier; p++)
	{
		if(strcmp(p->identifier, name) == 0)
			return p;
	}
	return 0;
}