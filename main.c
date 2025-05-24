#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mgclisp.h"  

/*
    MIT License

    (C) 2025 Max Goren, MaxGCoding.com
    
    mgclisp - a scheme interpreter in c
    Just for kicks.
    
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
    and associated documentation files (the “Software”), to deal in the Software without restriction, 
    including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or 
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
    OTHER DEALINGS IN THE SOFTWARE.

*/

void showHelp() {
    showVersion();
    printf("  Usage: \n");
    printf("    misp <options> [filename]\n");
    printf("         -h  view this screen\n");
    printf("         -v  start in verbose mode\n");
    printf("(c) 2024 MaxGCoding.com\n");
}

void parseOptions(int argc, char* argv[]) {
    List* env = createList();
    if (argv[1][0] == '-') {
        switch (argv[1][1]) {
            case 'h': { showHelp(); return; } break;
            case 'v': { trace_eval = true; } break;
            default: break;
        }
        if (argc == 3) {
            env = init(env);
            runScript(argv[2], env);
        } else repl();
    } else {
        env = init(env);
        runScript(argv[1], env);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        repl();
    } else {
        parseOptions(argc, argv);
    }
    return 0;
}

