#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static char* readFile(const char* path) {
    FILE* file = fopen(path, "r");

    // no file
    if (file == NULL) {
        perror("error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the string (+1 for null terminator)
    char* content = (char*)malloc(fileSize + 1);
    if (content == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Read file content
    size_t bytesRead = fread(content, 1, fileSize, file);
    if (bytesRead != fileSize) {
        perror("Error reading file");
        free(content);
        fclose(file);
        return NULL;
    }

    // Null-terminate the string
    content[fileSize] = '\0';

    fclose(file);
    return content;
}

static void runFile(const char* path) {
    char* source = readFile(path);

    compile(source);

}


int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect usage. Correct usage is:\n");
        fprintf(stderr, "syrup <input.syr>\n");
        exit(64);

    } else if (argc == 2) {
        runFile(argv[1]);
    }
    
    return 0;
}

