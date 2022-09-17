#include "PDCurses.h"
#include "Model.h"
#include "Controller.h"
#include "View.h"

int main() 
{
    PDCursesAdapter adapter;

    Model model;
    Controller controller(&model);
    View view(&model, &controller, &adapter);

    controller.start();
    return 0;
}