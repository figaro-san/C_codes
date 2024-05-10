#ifndef __LIBPERSONAL_H__
#define __LIBPERSONAL_H__

#include <stdint.h>
extern void exit_with_msg(const char *, ...);

extern void print_as_binary(int32_t);

extern int32_t ec_strtol_10(const char *, char *);

extern void multi_free(uint32_t , ...) ;

extern void show_int(int);
extern void show_short(short);

#endif
