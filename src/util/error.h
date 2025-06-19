#pragma once
#include <stdbool.h>
#include <stdlib.h>

// #define ERROR_EXIT(...) fprintf(stderr, __VA_ARGS__); exit(1)
// #define ERROR_RETURN(R, ...) fprintf(stderr, __VA_ARGS__); return R

#define ERROR(enumerator)	\
    typedef struct {		\
	enumerator type;	\
	bool ok;		\
	void *data;		\
    }

#define THROW(E,T,D)		\
    E.ok = false;		\
    E.data = malloc(sizeof(T)); \
    *(T *)E.data = D;

#define CATCH(T,E)		\
    *(T *)E.data

