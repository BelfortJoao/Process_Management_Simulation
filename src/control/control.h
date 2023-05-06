/**
 * @file control.h
 * @brief Header file for the Control module.
 */

#ifndef CONTROL_H
#define CONTROL_H

#include "../manager/processmanager.h"
#include "../printer/printer.h"

/**
 * @struct Control
 * @brief Struct that holds pointers to the process manager and printer.
 */
typedef struct Control
{
    ProcessManager *processManager; /**< Pointer to the process manager. */
    Printer *printer; /**< Pointer to the printer. */
} Control;

/**
 * @brief Allocates memory for a new Control struct and initializes its process manager and printer.
 *
 * @return Pointer to the newly created Control struct, or NULL if memory allocation fails.
 */
Control *initializeControl();

/**
 * @brief Runs the control loop for the simulation.
 *
 * @param control Pointer to the Control struct.
 * @return 0 if successful, -1 if an error occurs.
 */
int runControl(Control *control);

#endif /* CONTROL_H */