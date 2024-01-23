#pragma once

#define DEBUG true

#if DEBUG

#include <sys/stat.h>
#include <stdio.h>
#include <typeinfo>

#include "maths.hpp"

extern FILE *debug_file;

void debug_init();

#define _debug_log(args...) fprintf(debug_file, args)
#define debug_log(args...) {_debug_log(args); fflush(debug_file);}

void debug_log_obj(double n);
void debug_log_obj(vec3 vec);
void debug_log_obj(triangle3 tri);
void debug_log_obj(camera cam);

#define debug_obj(obj) {_debug_log("%s = ", #obj); debug_log_obj(obj); debug_log("\n");}

void debug_quit();

#else  // if !DEBUG

#define debug_file NULL
#define debug_init(...) NULL
#define debug_log(...) NULL
#define debug_log_obj(...) NULL
#define debug_obj(...) NULL
#define debug_quit(...) NULL

#endif  // DEBUG
