#ifdef __CYGWIN__
#include "http_win.c"
#else
#include "http_unix.c"
#endif
