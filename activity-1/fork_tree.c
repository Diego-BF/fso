#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

sig_atomic_t sig_count = 0;
char text[100];
int size_text = 0;
pid_t child1, child2;

pid_t create_child() {
    pid_t child_pid;
    child_pid = fork();

    if (child_pid) {
        printf("!!! PAI %d - FILHO %d\n", (int) getpid(), (int) child_pid);
        wait(NULL);
    } else {
        printf("### FILHO %d - PAI %d\n", (int) getpid(), (int) getppid());
    }
    sig_count++;
    printf("\tsig_count: %d\n", sig_count);
    return child_pid;
}

void child_count_chars() {
    printf("DEBUG");
    char c = ' ';
    pid_t child_pid;
    child_pid = create_child();

    if (!child_pid) {
        while (c != '\0' && size_text < 100) {
            c = text[size_text];
            if (c != '\0') {
                size_text++;
            }
        }
        exit(0);
    }
}

int main() {
    printf(">>> ");
    scanf("%100s", text);
    printf("%s\n\n", text);
    child1 = create_child();
    int pid = (int) getpid();
    printf("\t\tAqui quem fala e %d.\n", pid);
    if (child1) {
        child2 = create_child();
    }
    if (child1 && !child2) {
        child_count_chars();
    }

    if(!child1 && !child2) {
        printf("num of chars: %d\n", size_text);
    }

    return 0;
}