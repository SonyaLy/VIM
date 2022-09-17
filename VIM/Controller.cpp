#include "Controller.h"

Controller::Controller(Model* _model)
{
	model = _model;
}

void Controller::start()
{
	int ch;
	while (1)
	{
		ch = getch();
		model->recognize_symb(ch);
	}
}