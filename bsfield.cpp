//Copyright 2017 by Pavel Chistyakov
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

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