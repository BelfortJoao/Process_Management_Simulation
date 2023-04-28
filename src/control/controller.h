#ifndef SRC_CONTROLER_H
#define SRC_CONTROLER_H

#include "../manager/manager.h"
#include "../printer/printer.h"

typedef struct control {
    Computer *comp;
    Print *printer;
} Control;

void control(Control *cont);

#endif