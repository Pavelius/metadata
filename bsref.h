#include "bsfield.h"
#pragma once

// Metadata object
struct bsref
{
	const bsfield*	fields;
	void*			object;
	//
	inline operator bool() { return fields != 0; }
	//
	void			clear() const;
	const bsfield*	find(int id) const { return fields->find(id); }
	int				get(int id) const;
	int				get(int id, unsigned index) const;
	bsref			getr(int id) const;
	bsref			getr(int id, unsigned index) const;
	bool			isempthy() const;
	const void*		ptr(int id) const;
	const void*		ptr(int id, unsigned index) const;
	void			set(int id, int value, unsigned index = 0);
};
