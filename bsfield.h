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

#pragma once

enum fieldtypes {
	Number, Text, Table, Object
};

// Metadata field descriptor
struct bsfield
{
	template<class T> struct tinf { static constexpr int type = Object; };
	template<> struct tinf<int> { static constexpr int type = Number; };
	template<> struct tinf<short> { static constexpr int type = Number; };
	template<> struct tinf<unsigned> { static constexpr int type = Number; };
	template<> struct tinf<unsigned short> { static constexpr int type = Number; };
	template<> struct tinf<char> { static constexpr int type = Number; };
	template<> struct tinf<unsigned char> { static constexpr int type = Number; };
	template<class T> struct info
	{
		static constexpr int size = sizeof(T);
		static constexpr int count = 1;
	};
	template<class T, unsigned N> struct info<T[N]>
	{
		static constexpr int size = sizeof(T);
		static constexpr int count = N;
	};
	template<class T> struct info<T[]>
	{
		static constexpr int size = sizeof(T);
		static constexpr int count = 0;
	};
	int					id, to, type; // field identifier
	unsigned			offset; // offset from begin of class or object
	unsigned			size; // size of single element
	unsigned			lenght; // total size in bytes of all field (array has size*count)
	unsigned			count; // count of elements
	const bsfield*		child; // metadata of tables
	//
	const bsfield*		find(int id) const;
	inline bool			istable() const { return child == 0; }
	inline const char*	ptr(const void* data) const { return (const char*)data + offset; }
	inline const char*	ptr(const void* data, int index) const { return ptr(data) + index*size; }
};

#define FO(c,f) (const int)&((c*)0)->f
#define	BSREQ(cls, field, id, type) {id, 0, type, FO(cls,field), bsfield::info<decltype(cls::field)>::size, sizeof(cls::field), bsfield::info<decltype(cls::field)>::count}
#define	BSTBL(cls, field, id, type) {id, 0, Table, FO(cls,field), sizeof(cls::field), sizeof(cls::field), 0, type}