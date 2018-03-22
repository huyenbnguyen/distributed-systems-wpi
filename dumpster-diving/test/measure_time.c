#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    printf("%s\n", argv[1]);
    pid_t child_pid;
    int child_status;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    child_pid = fork();
    if(child_pid == 0) {
        /* This is done by the child process. */
        if (argc == 2) { // for files
            char *args[]={"../rm",argv[1],NULL};
            execvp(args[0],args);
        } else if (argc == 3) { // for directories
            char *args[]={"../rm",argv[1],argv[2],NULL};
            execvp(args[0],args);
        }

        /* If execvp returns, it must have failed. */
        printf("Unknown command\n");
        exit(0);
    }
    else {
       /* This is run by the parent.  Wait for the child
          to terminate. */
        pid_t tpid;
        do {
            tpid = wait(&child_status);
        } while(tpid != child_pid);
        gettimeofday(&end, NULL);
        printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec)
            - (start.tv_sec * 1000000 + start.tv_usec)));
        printf("%s\n", "");
        return child_status;
     }
}