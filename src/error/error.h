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

#define INVALID_COMMAND "Invalid command: '%c'."
#define INVALID_LINE "\nERROR: Invalid line in file. Please make sure that the file is correct.\n"
#define CONVERSION_ERROR "\nERROR: Couldn't convert string to integer.\n"
#define ALLOCATION_ERROR "\nERROR: Couldn't allocate memory correctly for %s.\n"
#define ERROR_FILE "\nERROR: Couldn't open the file at '%s'.\n"


#endif /* SRC_ERROR_H */