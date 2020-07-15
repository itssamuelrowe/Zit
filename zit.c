#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void printHelp() {
    printf(
        "[Usage]\n"
        "    zit command [options]\n\n"
        "[Commands]\n"
        "\n"
        " * commit\n"
        "   Create a new commit of the contents of the current implicit index.\n"
        "\n"
        "   --message|-m <message>    Specify the log message for the new commit.\n"
        "\n"
        "    switch\n"
        "    update\n"
        "    git\n"
        "    new               Create a new repository.\n"
        "    help              Print the help message.\n"
        "    version           Print the current version of Zit.\n"
        );
}

void printVersion() {
    printf("Zit v0.1\n");
}

void newRepository() {
    printf("[*] git init\n");
    system("git init");
}

/* TODO: zit commit --root */
/* The index is implicit in Zit. The index can be manipulated via Git. */
void commit(const char* message) {
    printf("[*] git add .\n");
    system("git add .");

    if (message == NULL) {
        printf("[*] git commit\n");
        system("git commit");
    }
    else {
        // WARNING: The following code is unsafe and can be exploited.
        char buffer[1000];
        sprintf(buffer, "git commit -m \"%s\"", message);
        printf("[*] %s\n", buffer);
        system(buffer);
    }
}

void z_switch(const char* reference) {
    // WARNING: The following code is unsafe and can be exploited.
    char buffer[1000];
    sprintf(buffer, "git checkout %s", reference);
    printf("[*] %s\n", buffer);
    system(buffer);
}

char* join_strings(char* strings[], char* seperator, int count) {
    char* str = NULL;             /* Pointer to the joined strings  */
    size_t total_length = 0;      /* Total length of joined strings */
    int i = 0;                    /* Loop counter                   */

    /* Find total length of joined strings */
    for (i = 0; i < count; i++) total_length += strlen(strings[i]);
    total_length++;     /* For joined string terminator */
    total_length += strlen(seperator) * (count - 1); // for seperators

    str = (char*) malloc(total_length);  /* Allocate memory for joined strings */
    str[0] = '\0';                      /* Empty string we can append to      */

    /* Append all the strings */
    for (i = 0; i < count; i++) {
        strcat(str, strings[i]);
        if (i < (count - 1)) strcat(str, seperator);
    }

    return str;
}

// TODO: workflows, prevent editing master branch, (count line numbers?) and much more!
int main(int length, char** arguments) {
    if (length < 2) {
        printHelp();
    }
    else {
        if (strcmp(arguments[1], "commit") == 0) {
            const char* message = NULL;
            int32_t i;
            for (i = 2; i < length; i++) {
                if ((strcmp(arguments[i], "--message") == 0) || (strcmp(arguments[i], "-m") == 0)) {
                    if (i + 1 < length) {
                        message = arguments[++i];
                    }
                    else {
                        printf("[error] `%s` expects an argument\n", arguments[i]);
                        break;
                    }
                }
            }

            commit(message);
        }
        else if (strcmp(arguments[1], "switch") == 0) {
            if (length != 3) {
                printf("[error] `switch` requires either branch name or commit identifier.\n");
            }
            else {
                const char* reference = arguments[2];
                z_switch(reference);
            }
        }
        else if (strcmp(arguments[1], "new") == 0) {
            newRepository();
        }
        else if (strcmp(arguments[1], "git") == 0) {
            // Unsafe
            char* line = join_strings(arguments + 1, " ", length - 1);
            printf("[*] %s\n", line);
            system(line);
        }
        else if (strcmp(arguments[1], "help") == 0) {
            printHelp();
        }
        else if (strcmp(arguments[1], "version") == 0) {
            printVersion();
        }
        else {
            printf("[error] Unknown command `%s`\n", arguments[1]);
        }
    }
    
    return 0;
}