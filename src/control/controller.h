#ifndef SRC_CONTROLER_H
#define SRC_CONTROLER_H
#endif
#include "../manager/manager.h"
#include "../printer/printer.h"
typedef struct control{
    Computer* comp;
    Print* prin;
} Control;

void control(Control* cont);
