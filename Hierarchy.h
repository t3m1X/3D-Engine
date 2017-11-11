#ifndef __Hierarchy_H__
#define __Hierarchy_H__

#include "Panel.h"

class Hierarchy : public Panel
{
public:
	Hierarchy();
	virtual ~Hierarchy();

	void Start();
	void Draw();

};

#endif