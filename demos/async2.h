#ifdef ASYNC
typedef struct { } async_context;

// used to insert argument for function declarations
#define DECL_ASYNC_ARG ,async_context * _async_context
#define DECL_ASYNC_ARG_ONLY async_context * _async_context
#define DECL_ASYNC_ARG_FIRST async_context * _async_context,
// the KR style
#define DECL_ASYNC_ARG_KR async_context * _async_context;

// used when calling async functions
#define ASYNC_ARG , _async_context
#define ASYNC_ARG_ONLY _async_context
#define ASYNC_ARG_FIRST _async_context,
#else
#define DECL_ASYNC_ARG
#define DECL_ASYNC_ARG_ONLY
#define DECL_ASYNC_ARG_FIRST
#define DECL_ASYNC_ARG_KR
#define ASYNC_ARG 
#define ASYNC_ARG_ONLY
#define ASYNC_ARG_FIRST 
#endif
