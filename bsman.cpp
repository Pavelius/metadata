#include "bsman.h"

extern "C" void		memcpy(void* p1, const void* p2, unsigned size);
extern "C" int		memcmp(const void* p1, const void* p2, unsigned size);
extern "C" int		strcmp(const char* s1, const char* s2);
bsman*				bsman::first;

bsman::bsman(const char* identifier, void* data, unsigned size, unsigned total_size, const bsfield* fields, const void* clone):
	identifier(identifier),
	data(data), size(size), count(0), max_count(total_size /size),
	fields(fields), clone(clone)
{
	if(!first)
		first = this;
	else
	{
		auto p = first;
		while(p->next)
			p = p->next;
		p->next = this;
	}
}

bsref bsman::add()
{
	if(count >= max_count)
		return{0, 0};
	return{fields, static_cast<char*>(data) + size*(count++)};
}

int bsman::indexof(const void* object) const
{
	if(object<data || object>(char*)data + size*count)
		return -1;
	return ((char*)object - (char*)data) / size;
}

void bsman::clear(const bsref& e)
{
	memcpy(e.object, clone, size);
}

bool bsman::isempthy(const void* object) const
{
	return memcmp(clone, object, size) == 0;
}

bsman* bsman::find(const char* name)
{
	for(auto p = bsman::first; p; p = p->next)
	{
		if(strcmp(p->identifier, name)==0)
			return p;
	}
	return 0;
}

bsman* bsman::find(const bsfield* fields)
{
	for(auto p = bsman::first; p; p = p->next)
	{
		if(p->fields==fields)
			return p;
	}
	return 0;
}

bsman* bsman::find(const void* object)
{
	for(auto p = bsman::first; p; p = p->next)
	{
		if(p->data<=object && object<((char*)p->data + p->size*p->count))
			return p;
	}
	return 0;
}

bsref bsman::create(const char* name)
{
	auto p = find(name);
	if(!p)
		return{0, 0};
	return p->add();
}