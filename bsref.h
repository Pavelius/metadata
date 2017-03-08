#include "aref.h"
#include "bsfield.h"

#pragma once

// Metadata reference object
struct bsref
{
	const bsfield*	fields;
	void*			object;
	//
	inline operator bool() const { return fields != 0; }
	//
	void			clear() const;
	const bsfield*	find(const char* name) const { return fields->find(name); }
	int				get(const char* name) const;
	int				get(const char* name, unsigned index) const;
	bsref			getr(const char* name) const;
	bsref			getr(const char* name, unsigned index) const;
	bool			isempthy() const;
	const void*		ptr(const char* name) const;
	const void*		ptr(const char* name, unsigned index) const;
	void			set(const char* name, int value, unsigned index = 0);
};
