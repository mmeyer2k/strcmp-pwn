#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
using namespace std;
 
// this is just strcmp() wrapped in timer calls
bool CheckEqual(char* a, char* b, bool show)
{
    int i, r, trv;
    long time_pre, time_post;
    struct timespec ts;
 
    if((trv = clock_gettime(CLOCK_MONOTONIC,&ts)) != 0) {
        printf("Timing error, pre. Code = %d\n", trv);
    }
    time_pre = ts.tv_nsec + (ts.tv_sec * 1000000000L);
 
    r = strcmp(a, b);
 
    if((trv = clock_gettime(CLOCK_MONOTONIC,&ts)) != 0) {
        printf("Timing error, post. Code = %d\n", trv);
    }
    time_post = ts.tv_nsec + (ts.tv_sec * 1000000000L);
 
    if (show)
        printf("Time: %ld\n", time_post - time_pre);
    else
        printf("First compare done.\n");
 
    return r == 0;
}
 
// this function helps us avoid the problem of the compiler being too smart.
// if we were to use string literals that were equal, it'd optimise compile-time
// constant strings into a single instance, so strcmp() would just early-out on
// the fact that the two pointers were equal, which is not what we want to
// demonstrate here - in reality we'd be comparing buffers that aren't static.
char* MakeStr(char c, int n)
{
    int i;
    char* s = (char*)calloc(n+1, sizeof(c));
    for(i=0; i<n; i++)
        s[i] = c;
    return s;
}
 
int main()
{
 
    // equal apart from last
    char* aa = MakeStr('0', 64);
    char* ab = "0000000000000000000000000000000000000000000000000000000000000001";
 
    // equal apart from first
    char* ba = MakeStr('0', 64);
    char* bb = "1000000000000000000000000000000000000000000000000000000000000000";
 
    // inequality in the middle
    char* ca = "0000000000000000000000000000010000000000000000000000000000000000";
    char* cb = "0000000000000000000000000000001000000000000000000000000000000000";
 
    // equal
    char* da = MakeStr('0', 64);
    char* db = MakeStr('0', 64);
 
    // first call to strcmp() and time functions will result in cache misses
    // so we call it once and don't display output to account for that.
    CheckEqual(aa, ab, false);
 
    printf("Completely equal, ");
    CheckEqual(da, db, true);
    printf("Equal but last, ");
    CheckEqual(aa, ab, true);
    printf("Equal but mid, ");
    CheckEqual(ca, cb, true);
    printf("Equal but first, ");
    CheckEqual(ba, bb, true);
 
    free(aa); free(ba); free(da); free(db);
    return 0;
}
