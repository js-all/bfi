/*
     ____  _____ ___  
    | __ )|  ___|_ _|    V 0.9.6
    |  _ \| |_   | |     stdin in brainfuck
    | |_) |  _|  | |     idk what to put here
    |____/|_|   |___|    so random bullshit go

*/

#include "queue.h"
#include "stack.h"
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// brainfuck config
#define BRAINFUCK_MEMORY_SIZE 30000
#define BRAINFUCK_CELL_TYPE unsigned int
// 0 -> abort
// 1 -> wrap
#define BRAINFUCK_CELL_OVERFLOW_BEHAVIOUR 1

// alias
#define BFCT BRAINFUCK_CELL_TYPE

struct bfThreadData {
    char* bfcode;
    long bfSize;
    struct Queue *inputs;
};

struct termios settupTTYRaw() {
    struct termios raw;
    struct termios backup;
    tcgetattr(STDIN_FILENO, &raw);
    tcgetattr(STDIN_FILENO, &backup);
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    return backup;
}

void resetTTYState(struct termios *backupState) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &*backupState);
}

pthread_mutex_t lock;

void *inputReadingThread(void *arg) {
    struct Queue *inputs = arg;
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        pthread_mutex_lock(&lock);
        int cn = (int) c;
        queue_enqueue(inputs, &cn);
        pthread_mutex_unlock(&lock);
    }
    return 0;
}

void *bfInterpretingThread(void* arg) {
    struct bfThreadData *bftd = arg;
    char *bfCode = bftd->bfcode;
    long bfsize = bftd->bfSize;
    struct Queue *inputs = bftd->inputs;

    BFCT *bfmem = malloc(BRAINFUCK_MEMORY_SIZE * sizeof(BFCT));
    unsigned long codeindex = 0;
    long memindex = 0;
    struct Stack loopStack;
    stack_init(&loopStack);
    long ci;
    // -1 for disabled, otherwise the codeindex of where it started
    long skippingMode = -1;
    while (codeindex < bfsize) {
        switch (bfCode[codeindex]) {
        case '+':
            if(skippingMode != -1) break;
            bfmem[memindex]++;
            break;
        case '-':
            if(skippingMode != -1) break;
            bfmem[memindex]--;
            break;
        case '<':
            if(skippingMode != -1) break;
            memindex--;
            if (memindex < 0) {
                memindex = BRAINFUCK_MEMORY_SIZE - 1;
            }
            break;
        case '>':
            if(skippingMode != -1) break;
            memindex++;
            if (memindex >= BRAINFUCK_MEMORY_SIZE) {
                memindex = 0;
            }
            break;
        case '.':
            if(skippingMode != -1) break;
            printf("%c\n", bfmem[memindex]);
            break;
        case '#':
            if(skippingMode != -1) break;
            printf("%u\n", bfmem[memindex]);
            break;
        case ',':
            if(skippingMode != -1) break;
            if (!queue_is_empty(inputs)) {
                int val;
                pthread_mutex_lock(&lock);
                queue_dequeue(inputs, &val);
                pthread_mutex_unlock(&lock);
                bfmem[memindex] = val;
                //printf("set index %ld to %c\n", memindex, (char) val);;
            }
            break;
        case '[':
            ci = (long)codeindex;
            stack_push(&loopStack, &ci);
            if (bfmem[memindex] == 0) {
                skippingMode = codeindex;
            }
            break;
        case ']':
            if (bfmem[memindex] != 0 && skippingMode == -1) {
                stack_peek(&loopStack, &ci);
                codeindex = ci;
            } else {
                stack_pop(&loopStack, &ci);
                if(ci == skippingMode) {
                    skippingMode = -1;
                }
            }
            break;
        }
        codeindex++;
    }
    printf("\n");
    free(bfmem);
    stack_free(&loopStack);
    return 0;
}

bool isBFChar(char *c) {
    if (*c == '+' || *c == '-' || *c == '<' || *c == '>' || *c == '[' ||
            *c == ']' || *c == '.' || *c == ',')
        return true;
    else
        return false;
}

char *readFileBF(char *fileName, long *fileSize) {
    FILE *file = fopen(fileName, "r");
    char *code;
    int c;
    // will grow as more bf characters are read.
    size_t bfsize = 0;

    if (file == NULL)
        return NULL;
    // get file size
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    // allocate with full file size (worst case senario) because we can't know how
    // much of it is actually brainfuck.
    code = malloc(f_size);
    // read everything
    while ((c = fgetc(file)) != EOF) {
        char ch = (char)c;
        if (isBFChar(&ch)) {
            code[bfsize++] = ch;
        }
    }
    // reallocate to actuall code size (without unrecognized characters).
    code = realloc(code, (bfsize + 1) * sizeof(char));
    assert(code != NULL);
    // null terminate
    code[bfsize] = '\0';
    *fileSize = bfsize;
    return code;
}

int main() {
    printf("shitty c bf interpreter that i hope it works, v0.9.6: \n");
    struct termios ttyStateBackup = settupTTYRaw();
    struct Queue inputs;
    pthread_t inputThread;
    pthread_t bfThread;
    long bfsize;
    char *bfCode = readFileBF("./code.bf", &bfsize);
    struct bfThreadData bftd;
    
    queue_init(&inputs);
    if(pthread_mutex_init(&lock, NULL) != 0) {
        printf("error whilst initializing mutex\n");
        return 1;
    }

    bftd.bfcode = bfCode;
    bftd.bfSize = bfsize;
    bftd.inputs = &inputs;

    pthread_create(&inputThread, NULL, inputReadingThread, &inputs);
    pthread_create(&bfThread, NULL, bfInterpretingThread, &bftd);
    pthread_join(bfThread, NULL);

    pthread_cancel(inputThread);
    queue_free(&inputs);
    pthread_mutex_destroy(&lock);
    resetTTYState(&ttyStateBackup);
    return 0;
}
