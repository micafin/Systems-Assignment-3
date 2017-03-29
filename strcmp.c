#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int newStrCmp (const char *s1, const char *s2) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    while (*p1 != '\0') {
        if(*p1>=48&&*p1<=57){
            if(isalpha(*p2)){
                return 1;
            }
        }
        if(*p2>=48&&*p2<=57){
            if(isalpha(*p1)){
                return -1;
            }
        }

        if (*p2 == '\0') return  1;
        if (*p2 > *p1)   return -1;
        if (*p1 > *p2)   return  1;

        p1++;
        p2++;
    }

    if (*p2 != '\0') return -1;

    return 0;
}

int main(int argc, char** argv){
    int result=newStrCmp(argv[1],argv[2]);
    printf("result %d\n",result);
}