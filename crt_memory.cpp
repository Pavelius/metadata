/* Copyright 2015 by Pavel Chistyakov
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

extern "C" void*	malloc(unsigned size);
extern "C" void*	realloc(void *ptr, unsigned size);
extern "C" void		free(void* pointer);

// Return last element in sequence.
template<class T> inline T* seqlast(T* p)
{
	while(p->next)
		p = p->next;
	return p;
}

struct crt_memory_block
{
	char				signature[4];
	unsigned			size;
	crt_memory_block*	next;
};
static crt_memory_block	first_block = {"CRT"};

static crt_memory_block* getprev(crt_memory_block* p)
{
	for(auto p1 = &first_block; p1->next; p1 = p1->next)
	{
		if(p1->next == p)
			return p1;
	}
	return 0;
}

// Used for rellocate simple classes like new char[1000]
void* operator new(unsigned size)
{
	crt_memory_block* p = (crt_memory_block*)malloc(size + sizeof(crt_memory_block));
	p->signature[0] = 'C';
	p->signature[1] = 'R';
	p->signature[2] = 'T';
	p->signature[3] = 0;
	p->size = size;
	p->next = 0;
	seqlast(&first_block)->next = p;
	return p + 1;
}

// Used for rellocate simple classes like new char[1000]
void* operator new[](unsigned size)
{
	return operator new(size);
}

// Used for delete simple classes like delete char*
void operator delete(void* ptr)
{
	if(!ptr)
		return;
	crt_memory_block* p = (crt_memory_block*)ptr - 1;
	crt_memory_block* pr = getprev(p);
	if(!pr)
		return;
	pr->next = p->next;
	free(p);
}

void operator delete(void* p, unsigned size)
{
	operator delete(p);
}

void* rmreserve(void* ptr, unsigned size)
{
	if(!ptr)
		return new char[size];
	crt_memory_block* p = (crt_memory_block*)ptr - 1;
	crt_memory_block* pr = getprev(p);
	if(!pr)
		return ptr;
	if(p->size >= size)
		return ptr;
	p = (crt_memory_block*)realloc(p, size + sizeof(crt_memory_block));
	p->size = size;
	pr->next = p;
	return p + 1;
}

int rmblockcount()
{
	int result = 0;
	for(auto p = first_block.next; p; p = p->next)
		result++;
	return result;
}

bool rmblock(void* ptr)
{
	crt_memory_block* p = (crt_memory_block*)ptr - 1;
	return getprev(p) != 0 && p->signature[0]=='C' && p->signature[1] == 'R' && p->signature[2] == 'T';
}