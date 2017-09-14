#include "bsref.h"

extern "C" int memset(void* p1, unsigned char value, unsigned size);

int bsref::get(int id) const
{
	if(!fields)
		return 0;
	auto pf = fields->find(id);
	if(!pf)
		return 0;
	auto p = pf->ptr(object);
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
		p = pf->ptr(object);
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
	return pf->ptr(object);
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
		memset((char*)object + f->offset, 0, f->lenght);
}

bool bsref::isempthy() const
{
	for(auto f = fields; f->id; f++)
	{
		auto p1 = (char*)f->ptr(object);
		auto p2 = p1 + f->lenght;
		while(p1 < p2)
		{
			if(*p1)
				return false;
		}
	}
	return true;
}