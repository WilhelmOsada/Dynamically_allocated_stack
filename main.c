#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
int main(int argc, char **argv)
{
    argc--;
    argv++;
    if(argc < 1)
    {
        printf("Not enough arguments");
        return 9;
    }
    struct stack_t *stack;
    for(int i = 0 ; i < argc ; i++)
    {
        int ret = stack_load(&stack,*(argv + i));
        if(ret == 2)
        {
            printf("Couldn't open file %s\n",*(argv + i));
        }
        else if(ret == 3)
        {
            printf("Failed to allocate memory");
            stack_destroy(&stack);
            return 8;
        }
        else
        {
            ret = stack_save(stack,*(argv + i));
            if(ret == 2)
            {
                printf("Couldn't create file");
                stack_destroy(&stack);
                return 5;
            }
            printf("File %s saved\n",*(argv + i));
        }
        stack_destroy(&stack);
    }
    return 0;
}