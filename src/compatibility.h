#ifndef LIBA_COMPATIBILITY_H
#define LIBA_COMPATIBILITY_H

#ifdef _MSC_VER
#define THREAD_LOCAL __declspec(thread)
#else
#define THREAD_LOCAL _Thread_local
#endif

#endif //LIBA_COMPATIBILITY_H
