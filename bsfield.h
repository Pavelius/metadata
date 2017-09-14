#pragma once

// Metadata descriptor
struct bsfield
{
	enum type_s {
		Number, Text, Object
	};
	template<class T> struct tinf { static constexpr type_s type = Object; };
	template<> struct tinf<int> { static constexpr type_s type = Number; };
	template<> struct tinf<short> { static constexpr type_s type = Number; };
	template<> struct tinf<unsigned> { static constexpr type_s type = Number; };
	template<> struct tinf<unsigned short> { static constexpr type_s type = Number; };
	template<> struct tinf<char> { static constexpr type_s type = Number; };
	template<> struct tinf<unsigned char> { static constexpr type_s type = Number; };
	template<> struct tinf<const char*> { static constexpr type_s type = Text; };
	template<> struct tinf<const char*[2]> { static constexpr type_s type = Text; };
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
	int			id, type; // field identifier
	unsigned	offset; // offset from begin of class or object
	unsigned	size; // size of single element
	unsigned	lenght; // total size in bytes of all field (array has size*count)
	unsigned	count; // count of elements
	// Find element field by ID
	const bsfield* find(int id) const
	{
		if(!this)
			return 0;
		for(auto p = this; p->id; p++)
		{
			if(p->id == id)
				return p;
		}
		return 0;
	}
	// Get element pointer
	inline const char* ptr(const void* data) const
	{
		return (const char*)data + offset;
	}
	// Get element pointer by index
	inline const char* ptr(const void* data, int index) const
	{
		return (const char*)data + offset + index*size;
	}
	// If element if array
	inline bool isarray() const
	{
		return size != lenght;
	}
};
#define FO(c,f) (const int)&((c*)0)->f
#define	BSREQ(cls, field, id) {id, bsfield::tinf<decltype(cls::field)>::type, FO(cls,field), bsfield::info<decltype(cls::field)>::size, sizeof(cls::field), bsfield::info<decltype(cls::field)>::count}