/*
 * mac.c
 *
 * Simple VM implemnetation.
 *
 * Taken from https://felixangell.com/blog/implementing-a-virtual-machine-in-c/
 */
#include <stdbool.h>
#include <stdio.h>

bool running = true;
int ip = 0;
int sp = -1;

int stack[256];

typedef enum
{
    PSH,
    ADD,
    POP,
    SET,
    HLT
} InstructionSet;


const int program[] =
{
    PSH, 5,
    PSH, 6,
    ADD,
    POP,
    HLT
};

void eval(int instr);
int  fetch();

int main(void)
{
    while (running)
    {
        eval(fetch());
        ++ip;
    }

    return 0;
}

/*
 * eval - Evaulate instruction.
 */
void eval(int instr)
{
    switch (instr)
    {
        case HLT: 
        {
            running = false;
            printf("DONE\n");
            break;
        }

        case PSH:
        {
            ++sp;
            ++ip;
            stack[sp] = program[ip];
            break;
        }

        case POP:
        {
            int val_popped = stack[sp];
            --sp;
            printf("popped %d\n", val_popped);
            break;
        }

        case ADD:
        {
            int a = stack[sp];
            --sp;

            int b = stack[sp];
            --sp;

            ++sp;
            stack[sp] = b + a;
            break;
        }
    }
}

/*
 * fetch - Fetch an instruction from a program.
 */
int fetch()
{
    return program[ip];
}
