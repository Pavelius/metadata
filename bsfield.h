#pragma once

// Metadata field descriptor
struct bsfield
{
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
	const char*			identifier; // field identifier
	unsigned			offset; // offset from begin of class or object
	unsigned			size; // size of single element
	unsigned			lenght; // total size in bytes of all field (array has size*count)
	unsigned			count; // count of elements
	unsigned char		reference; // 1+ is reference
	const bsfield*		type; // metadata of element
	//
	const bsfield*		find(const char* name) const;
	inline bool			istable() const { return count == 0; }
	inline const char*	ptr(const void* data) const { return (const char*)data + offset; }
	inline const char*	ptr(const void* data, int index) const { return ptr(data) + index*size; }
};

extern bsfield			number_type[2]; // standart integer value
extern bsfield			text_type[2]; // stantart zero ending string

#define FO(c,f) (const int)&((c*)0)->f
#define	BSREQ(cls, field, type) {#field, FO(cls,field), bsfield::info<decltype(cls::field)>::size, sizeof(cls::field), bsfield::info<decltype(cls::field)>::count, 0, type}
#define	BSTBL(cls, field, type) {#field, FO(cls,field), sizeof(cls::field), sizeof(cls::field), 0, 0, type}