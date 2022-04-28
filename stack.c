#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <string.h>
int stack_push(struct stack_t **stack, char *value)
{
    if(!stack || !value)    return 1;
    struct stack_t *new = calloc(1,sizeof(struct stack_t));
    if(!new)    return 2;
    new->prev = *stack;
    new->sentence = value;
    *stack = new;
    return 0;
}
char *stack_pop(struct stack_t **stack, int *err_code)
{
    if(!stack || !*stack || !(*stack)->sentence)
    {
        if(err_code) *err_code = 1;
        return NULL;
    }
    char *ret = (*stack)->sentence;
    struct stack_t *temp = (*stack)->prev;
    free(*stack);
    *stack = temp;
    if(err_code) *err_code = 0;
    return ret;
}
void stack_destroy(struct stack_t **stack)
{
    if(stack && *stack)
    {
        struct stack_t *nxt;
        do {
            nxt = (*stack)->prev;
            free((*stack)->sentence);
            free(*stack);
            (*stack) = nxt;
        } while (nxt);
    }
}
int stack_load(struct stack_t **stack, const char *filename)
{
    if(!stack || !filename || *stack) return 1;
    FILE *f = fopen(filename,"r");
    if(!f)  return 2;
    while(1)
    {
        int size = 0;
        long pos = ftell(f);
        int byla = 0;
        char a;
        for( ; ; size++)
        {
            a = fgetc(f);
            if(a == '.')
            {
                byla = 1;
                break;
            }
            if(feof(f))    break;
        }
        if (byla == 1) {
            fseek(f, pos, SEEK_SET);
            char *send = calloc(size + 2, 1);
            if (!send) {
                stack_destroy(stack);
                fclose(f);
                return 3;
            }
            for (int i = 0; i <= size; i++) {
                *(send + i) = fgetc(f);
            }
            *(send + strlen(send)) = '\0';
            if(stack_push(stack, send))
            {
                stack_destroy(stack);
                free(send);
                fclose(f);
                return 3;
            }
        } else {
            fclose(f);
            return 0;
        };
    }

}
int stack_save(const struct stack_t *stack, const char *filename)
{
    if(!stack || !filename) return 1;
    FILE *f = fopen(filename,"w");
    if(!f)  return 2;
    struct stack_t *next = (struct stack_t*)stack;
    do
    {
        fwrite(next->sentence, strlen(next->sentence),1,f);
        next = next->prev;
    }while(next);
    fclose(f);
    return 0;
}