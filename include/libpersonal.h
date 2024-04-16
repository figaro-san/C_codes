#ifndef __LIBPERSONAL_H__
#define __LIBPERSONAL_H__

#include <stdint.h>
void exit_with_msg(const char *, ...);
void print_as_binary(int32_t);
int32_t ec_strtol_10(const char *, char *);
void multi_free(uint32_t , ...) ;

#endif
