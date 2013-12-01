#define BROKEN_STACK_ALIGNMENT 1
#define HAVE_MMX 1
#define HAVE_32B_STACK_ALIGNMENT 1
#if defined(_M_X64)
#   define ARCH_X86_64 1
#   define ARCH_X86_32 0
#   define ARCH_X86 0
#else
#   define ARCH_X86_64 0
#   define ARCH_X86_32 1
#   define ARCH_X86 1
#endif
#define SYS_WINDOWS 1
#define HAVE_WIN32THREAD 1
#define HAVE_THREAD 1
#define HAVE_LOG2F 1
#define HAVE_AVS 1
#define USE_AVXSYNTH 0
#define fseek _fseeki64
#define ftell _ftelli64
#define HAVE_GPL 1
#define HAVE_INTERLACED 1
#define HAVE_OPENCL 1
#define HAVE_MALLOC_H 0
#define HAVE_ALTIVEC 0
#define HAVE_ALTIVEC_H 0
#define HAVE_ARMV6 0
#define HAVE_ARMV6T2 0
#define HAVE_NEON 0
#define HAVE_BEOSTHREAD 0
#define HAVE_POSIXTHREAD 0
#define HAVE_VISUALIZE 0
#define HAVE_SWSCALE 0
#define HAVE_LAVF 0
#define HAVE_FFMS 0
#define HAVE_GPAC 0
#define HAVE_GF_MALLOC 0
#define HAVE_VECTOREXT 0
#define HAVE_CPU_COUNT 0
#define HAVE_THP 0
