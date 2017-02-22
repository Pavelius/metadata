#include "bsref.h"

#pragma once

struct bsman
{
	struct iter
	{
		bsref		ref;
		unsigned	size;
		void		operator++() { ref.object = static_cast<char*>(ref.object) + size; }
		bool		operator!=(const iter& e) const { return ref.object != e.ref.object; }
		bsref&		operator*() { return ref; }
	};
	void*			data;
	unsigned		count;
	unsigned		size;
	unsigned		max_count;
	const bsfield*	fields;
	const char*		identifier;
	const void*		clone;
	static bsman*	first;
	bsman*			next;
	//
	bsman(const char* identifier, void* object, unsigned size, unsigned total_size, const bsfield* fields, const void* clone);
	//
	bsref			operator[](unsigned index) { if(index >= count) return{0, 0}; return{fields, static_cast<char*>(data) + size*index}; }
	iter			begin() const { return{{fields, const_cast<void*>(data)}, size}; }
	iter			end() const { return{{fields, static_cast<unsigned char*>(data) + size*count}, size}; }
	//
	bsref			add();
	static bsref	create(const char* name);
	void			clear(const bsref& e);
	int				indexof(const void* object) const;
	int				indexof(const bsref& e) const { return indexof(e.object); }
	bool			isempthy(const void* object) const;
	bool			isempthy(const bsref& e) const { return isempthy(e.object); }
	static bsman*	find(const char* name);
	static bsman*	find(const bsfield* fields);
	static bsman*	find(const void* object);
	static bsman*	find(const bsref& e) { return find(e.fields); }
};
#define	BSMAN(T) \
static T clone;\
bsman T##_manager(#T, objects, sizeof(objects[0]), sizeof(objects), T##_type, &clone);