#pragma once

#if !defined(IGNORE_WARNINGS_START) || !defined(IGNORE_WARNINGS_END)

#define IW_STRINGIZE_IMPL(x) #x
#define IW_STRINGIZE(x) IW_STRINGIZE_IMPL(x)

#define IW_PRAGMA(compiler, x) _Pragma(IW_STRINGIZE(compiler x))

#if defined(__clang__)
#define IW_COMPILER clang
#elif defined(__GNUC__)
#define IW_COMPILER GCC
#else
#define IW_COMPILER
#endif

#if defined(IW_COMPILER)
#define IW_DIAGNOSTIC_PUSH() IW_PRAGMA(IW_COMPILER, diagnostic push)
#define IW_DIAGNOSTIC_POP() IW_PRAGMA(IW_COMPILER, diagnostic pop)
#define IW_DIAGNOSTIC_IGNORED(w) IW_PRAGMA(IW_COMPILER, diagnostic ignored w)
#else
#define IW_DIAGNOSTIC_PUSH()
#define IW_DIAGNOSTIC_POP()
#define IW_DIAGNOSTIC_IGNORED(w)
#endif

#define IGNORE_WARNINGS_START(warning)                                              \
    IW_DIAGNOSTIC_PUSH()                                                            \
    IW_DIAGNOSTIC_IGNORED(warning)

#define IGNORE_WARNINGS_END() IW_DIAGNOSTIC_POP()

#endif
