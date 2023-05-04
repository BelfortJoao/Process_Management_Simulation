#include "control/control.h"

int main()
{
    Control *control = initializeControl();
    runControl(control);

    return 0;
}