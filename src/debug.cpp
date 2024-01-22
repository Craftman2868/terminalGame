#include "debug.hpp"

#if DEBUG

#include <sys/stat.h>
#include <stdio.h>

#include "maths.hpp"

FILE *debug_file;

void debug_init()
{
    mkfifo("debug/pipe", S_IRUSR | S_IWUSR);
    debug_file = fopen("debug/pipe", "w");
}

#define _debug_log_arg(obj, arg) {_debug_log("  " #arg " = "); debug_log_obj(obj.arg); _debug_log(",\n");}

void debug_log_obj(double n)
{
    _debug_log("%f", n);
}

void debug_log_obj(vec3 vec)
{
    _debug_log("{%f, %f, %f}", vec.x, vec.y, vec.z);
}

void debug_log_obj(triangle3 tri)
{
    _debug_log("{\n");
    _debug_log_arg(tri, p1);
    _debug_log_arg(tri, p2);
    _debug_log_arg(tri, p3);
    _debug_log("}");
}

void debug_log_obj(camera cam)
{
    _debug_log("{\n");
    _debug_log_arg(cam, pos);
    _debug_log_arg(cam, pitch);
    _debug_log_arg(cam, yaw);
    _debug_log_arg(cam, fl);
    _debug_log("}");
}

#undef _debug_log_arg

void debug_quit()
{
    fclose(debug_file);
}

#endif  // DEBUG
