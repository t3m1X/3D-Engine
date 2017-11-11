#ifndef __HardwarePanel_H__
#define __HardwarePanel_H__

#include "Panel.h"

class HardwarePanel : public Panel
{
public:
	HardwarePanel(bool isActive);
	virtual ~HardwarePanel();

	void Start();
	void Draw();

};

#endif