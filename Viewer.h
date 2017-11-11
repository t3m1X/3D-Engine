#ifndef __Viewer_G__
#define __Viewer_G__

#include "Panel.h"
#include "Geomath.h"

struct DockContext;

class Viewer : public Panel {

public:

	Viewer();

	void Start();
	void Draw();
	void Clear();

public:
	const float4 GetRect() const;
private:
	DockContext* dock;
	bool first_time = true;

	uint my_id = 0;

	float2 position;
	float2 size;
	float2 saved_size;
};
#endif