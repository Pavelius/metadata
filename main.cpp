#include "bsman.h"

struct point
{
	short			x, y;
};
bsfield point_type[] = {
	BSREQ(point, x, number_type),
	BSREQ(point, y, number_type),
	{0}
};

struct rect
{
	int				x1, y1, x2, y2;
};
bsfield rect_type[] = {
	BSREQ(rect, x1, number_type),
	BSREQ(rect, y1, number_type),
	BSREQ(rect, x2, number_type),
	BSREQ(rect, y2, number_type),
	{0}
};

struct area
{
	int				x, y;
	const char*		name[2];
	rect			rectangles[10];
	aref<point>		points;
};
bsfield area_type[] = {
	BSREQ(area, x, number_type),
	BSREQ(area, y, number_type),
	BSREQ(area, name, text_type),
	BSREQ(area, rectangles, rect_type),
	BSTBL(area, points, point_type)
};
static area objects[32];
BSMAN(area);

bool test_simple_member_access()
{
	point t = {10, -20};
	bsref e = {point_type, &t};
	auto x = e.get("x");
	auto y = e.get("y");
	return x == 10 && y == -20;
}

bool test_indexed_object()
{
	static area t = {10, 10, {"Test", "Тест"}, {{10, 10, 5, 5}, {1, 2, 3, -1}}};
	bsref e = {area_type, &t};
	auto e1 = e.getr("rectangles", 1);
	if(!e1)
		return false;
	auto y2 = e1.get("y2");
	return y2 == -1;
}

bool test_table()
{
	static area t = {10, 10, {"Test", "Тест"}, {{10, 10, 5, 5}, {1, 2, 3, -1}}};
	bsref e = {area_type, &t};
	auto pb = (aref<point>*)e.ptr("points");
	if(pb)
	{
		auto& p1 = pb->addr();
		auto& p2 = pb->addr();
		auto& p3 = pb->addr();
		p1.x = -2; p1.y = 1;
		p2.x = 10; p2.y = 12;
		p3.x = 5; p3.y = 5;
	}
	return true;
}

bool test_manager()
{
	auto pm = bsman::find("area");
	if(!pm)
		return false;
	auto& r1 = bsman::create("area"); // Add new record to manager
	r1.set("x", 12);
	r1.set("y", 24);
	r1.set("z", 124); // This value is not found, but nothing happened and this is a feature
	auto& r2 = pm->add(); // same effect as bsman::create
	r2.set("x", 2);
	r2.set("y", 4);
	unsigned char temp[3] = {0};
	// Find manager by record
	if(bsman::find(r2) != pm)
		return false;
	// For each record in manager
	for(auto e : *pm)
	{
		if(pm->isempthy(e))
			continue;
		temp[pm->indexof(e)] = e.get("x");
	}
	r1.clear(); // Clear only specified fields and cleanup tables.
	pm->clear(r2); // Clear all fields (inlucde protected and virtual) and cleanup tables
	return true;
}

int main(int argc, char* argv[])
{
	if(!test_simple_member_access())
		return -1;
	if(!test_indexed_object())
		return -1;
	if(!test_table())
		return -1;
	if(!test_manager())
		return -1;
	return 0;
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw)
{
	return main(0, 0);
}