/* =========================================================================

   Deklaration allgemeiner Konstanten und Funktionen

   ====================================================================== */


#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <unistd.h>
#define MEMORY_USAGE 1


#ifndef CLK_TCK 
  #ifndef __CYGWIN__
	#define CLK_TCK sysconf(_SC_CLK_TCK)
	#endif
#endif

#ifdef MEMORY_USAGE
void* common_malloc (size_t);
void common_free (void *,size_t);
char* common_strdup (char*);
void* common_realloc (void*,size_t,size_t);
void common_check_leak ();
#else
#include <string.h>
#define common_free(ptr,bytes) free(ptr)
#define common_malloc(bytes) malloc(bytes)
#define common_strdup(s) strdup(s)
#define common_realloc(p,s,x) (realloc(p,s))
#define common_check_leak()
#endif

void common_resource_init ();
void common_resource_setmark (int);
void common_resource_displaymark (int,char*,float*);
void common_resource_patch (int , char *, float *, int );

void common_error(char*,...);

float common_time_getseconds();

extern char* common_strbuf;
void common_strbuf_init ();
void common_strbuf_new ();
void common_addbuf (char*,...);

#endif
