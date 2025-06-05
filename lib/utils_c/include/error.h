#pragma once
#include <stdbool.h>

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

