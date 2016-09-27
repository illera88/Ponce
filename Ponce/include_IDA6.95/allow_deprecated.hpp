// this file should be included before calling deprecated functions
// it should be included at the point where the definitions of deprecated
// functions begin in the source file. this way a deprecated function may call
// another deprecated function without raising a warning.

// deprecated functions may call each other
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
