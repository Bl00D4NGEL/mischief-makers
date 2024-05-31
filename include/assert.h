#ifndef ASSERT_H
#define ASSERT_H

extern void __assert(const char *, const char *, int);

// assert for matching
#ifndef NDEBUG
# ifndef NON_MATCHING
#  define assert(cond, file, line) ((cond) ? ((void)0) : __assert(#cond, file, line))
# else
#  define assert(cond, file, line) ((cond) ? ((void)0) : __assert(#cond, __FILE__, __LINE__))
# endif
#else
# define assert(cond, file, line) ((void)0)
#endif

#endif
