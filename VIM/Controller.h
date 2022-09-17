#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Model.h"

class Controller
{
public:
	Controller(Model* model);
	void start();
private:
	Model* model;
};

#endif //__CONTROLLER_H__