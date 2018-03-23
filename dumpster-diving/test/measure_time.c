#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    // printf("%s\n", argv[1]);
    pid_t child_pid;
    int child_status;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    child_pid = fork();
    if(child_pid == 0) {
        /* This is done by the child process. */
        if (argc == 3) { // for files
            printf("%s\n", argv[2]);
            char *args[]={argv[1],argv[2],NULL};
            execvp(args[0],args);
        } else if (argc == 4) { // for directories
            printf("%s\n", argv[3]);
            char *args[]={argv[1],argv[2],argv[3],NULL};
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
        sync();
        gettimeofday(&end, NULL);
        printf("%ld\n", ((end.tv_sec * 1000 + end.tv_usec/1000)
            - (start.tv_sec * 1000000 + start.tv_usec)));
        printf("%s\n", "");
        return child_status;
     }
}