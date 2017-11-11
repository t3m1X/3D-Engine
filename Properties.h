#ifndef __Properties_H__
#define __Properties_H__

#include "Panel.h"

class Properties : public Panel
{
public:
	Properties();
	virtual ~Properties();

	void Start();
	void Draw();

};

#endif