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

#include "bsref.h"

extern "C" int		memset(void* p1, unsigned char value, unsigned size);

int bsref::get(int id) const
{
	if(!fields)
		return 0;
	auto pf = fields->find(id);
	if(!pf)
		return 0;
	auto p = pf->to ? pf->ptr(object, id-pf->id) : pf->ptr(object);
	switch(pf->size)
	{
	case sizeof(char) : return *((char*)p);
	case sizeof(short) : return *((short*)p);
	default: return *((int*)p);
	}
}

int bsref::get(int id, unsigned index) const
{
	if(!fields)
		return 0;
	auto pf = fields->find(id);
	if(!pf || index >= pf->count)
		return 0;
	auto p = pf->ptr(object, index);
	switch(pf->size)
	{
	case sizeof(char) : return *((char*)p);
	case sizeof(short) : return *((short*)p);
	default: return *((int*)p);
	}
}

void bsref::set(int id, int value, unsigned index)
{
	if(!fields)
		return;
	auto pf = fields->find(id);
	if(!pf)
		return;
	const void* p;
	if(index)
	{
		if(index >= pf->count)
			return;
		p = pf->ptr(object, index);
	}
	else
		p = pf->to ? pf->ptr(object, id - pf->id) : pf->ptr(object);
	switch(pf->size)
	{
	case sizeof(char) : *((char*)p) = value; break;
	case sizeof(short) : *((short*)p) = value; break;
	default: *((int*)p) = value; break;
	}
}

const void* bsref::ptr(int id) const
{
	if(!fields)
		return 0;
	auto pf = fields->find(id);
	if(!pf)
		return 0;
	return pf->to ? pf->ptr(object, id - pf->id) : pf->ptr(object);
}

const void* bsref::ptr(int id, unsigned index) const
{
	if(!fields)
		return 0;
	auto pf = fields->find(id);
	if(!pf || index >= pf->count)
		return 0;
	return pf->ptr(object, index);
}

void bsref::clear() const
{
	if(!fields)
		return;
	for(auto f = fields; f->id; f++)
	{
		if(f->istable())
		{
			auto p = reinterpret_cast<aref<char*>*>(static_cast<char*>(object) + f->offset);
			p->clear();
		}
		else
			memset(static_cast<char*>(object) + f->offset, 0, f->lenght);
	}
}

bool bsref::isempthy() const
{
	for(auto f = fields; f->id; f++)
	{
		auto p1 = static_cast<const char*>(f->ptr(object));
		auto p2 = p1 + f->lenght;
		while(p1 < p2)
		{
			if(*p1)
				return false;
		}
	}
	return true;
}