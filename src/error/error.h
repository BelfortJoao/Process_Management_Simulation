/**
 * @file error.h
 * @brief Defines error messages as preprocessor macros.
 *
 * This header file defines several preprocessor macros to be used as error messages
 * in different parts of the program. These macros are used to display specific
 * error messages when the program encounters certain errors, such as invalid input
 * or memory allocation failure.
 */

#ifndef SRC_ERROR_H
#define SRC_ERROR_H

#include "../colour/colour.h"

#define INVALID_COMMAND "%sInvalid command: '%c'.", RED
#define INVALID_LINE "%s\nERROR: Invalid line in file. Please make sure that the file is correct.\n", RED
#define CONVERSION_ERROR "%s\nERROR: Couldn't convert string to integer.\n", RED
#define ALLOCATION_ERROR "%s\nERROR: Couldn't allocate memory correctly for %s.\n", RED
#define ERROR_FILE "%s\nERROR: Couldn't open the file at '%s'.\n", RED


#endif /* SRC_ERROR_H */