#include <cstdlib>




//#define MIN_CUT(a,b,c) ((a) < (b))? (a) : (c)

#define MIN(a,b) ((a) < (b))? (a) : (b)
#define MAX(a,b) ((a) > (b))? (a) : (b)



#define INF_LOG(enable, ...) \
{ \
	if (enable) \
	{ \
		fprintf(stderr, __VA_ARGS__); \
	} \
		\
	fflush(stderr); \
}


#define ERROR_LOG(enable, ...) \
{ \
	if (enable) \
	{ \
		fprintf(stderr, "%s --> %s, %d: \n", __TIME__, __FILE__, __LINE__); \
		fprintf(stderr, __VA_ARGS__); \
	} \
		\
	fflush(stderr); \
} 
