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

#include "aref.h"
#include "bsfield.h"

#pragma once

// Metadata reference object
struct bsref
{
	const bsfield*	fields;
	void*			object;
	//
	inline operator bool() { return fields != 0; }
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