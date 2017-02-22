//Copyright 2013 by Pavel Chistyakov
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

// Standart reserve procedure
void* rmreserve(void* ptr, unsigned size);
// Get total count of records.
inline int optimal(int need_count, int mc = 256 * 256)
{
	int m = 64;
	while(m < mc)
	{
		if(need_count < m)
			return m;
		m = m << 1;
	}
	return m;
}
// Simplest array - pointer and count. Used only for POD types.
// Not POD types NEED sublass clear(), reserve(), reserve(n).
template<class T> struct aref
{
	T*						data;
	int						count;
	//
	inline T&				operator[](int index) { return data[index]; }
	//
	T&						add() { return data[count++]; }
	T&						addr() { reserve(); return data[count++]; }
	void					add(const T& e) { data[count++] = e; }
	template<class Z> T*	addu(Z value) { auto e = find(value); if(e) return e; reserve(); return data + (count++); }
	T*						begin() { return data; }
	const T*				begin() const { return data; }
	void					initialize() { data = 0; count = 0; }
	void					clear() { delete data; initialize(); }
	T*						end() { return data + count; }
	const T*				end() const { return data + count; }
	template<class Z> T*	find(Z value) { for(auto& e : *this) { if(e.id == value) return &e; } return 0; }
	int						indexof(const T* t) const { if(t<data || t>data + count) return -1; return t - data; }
	int						indexof(const T t) const { for(int i = 0; i < count; i++) if(data[i] == t) return i; return -1; }
	void					reserve(int count) { data = (T*)rmreserve(data, sizeof(T)*optimal(count, 256 * 256 * 256)); }
	void					reserve() { reserve(count + 1); }
	void					remove(int index) { if(index < 0 || index >= count) return; count--; if(index >= count) return; memmove(data + index, data + index + 1, sizeof(data[0])*(count - index)); }
};