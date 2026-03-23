#include "parser.h"
#include <stddef.h>

int parse(char *in, char **args) {
    int argc = 0;
    int sq_flag = 0, dq_flag = 0;
    char *r = in;

    while(*r == ' ' || *r == '\t') r++;
    if(*r == '\0') return 0;

    args[argc++] = r;
    char *w = r;

    while(*r != '\0') {
        if(*r == '\'') {
            if(dq_flag) {
                *w = *r;
                w++;
            }
            else
                sq_flag = !sq_flag;
        }
        else if(*r == '\"') {
            if(sq_flag) {
                *w = *r;
                w++;
            }
            else
                dq_flag = !dq_flag;
        }
        else if((*r == ' ' || *r == '\t') && !sq_flag && !dq_flag) {
            *w = '\0';
            w++;
            while(*(r+1) == ' ' || *(r+1) == '\t')
                r++;
            if(*(r+1) != '\0')
                args[argc++] = w;
        }
        else {
            *w = *r;
            w++;
        }
        r++;
    }

    *w = '\0';
    args[argc] = NULL;

    return argc;
}