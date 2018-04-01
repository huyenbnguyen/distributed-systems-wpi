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
        if (argc == 7 || argc == 6) {
            char *args[]={argv[1],argv[2],argv[3],argv[4],argv[5],NULL};
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
        long duration = (end.tv_sec * 1000000 + end.tv_usec)
            - (start.tv_sec * 1000000 + start.tv_usec);

        // write to file
        FILE *fp;
        fp = fopen(argv[6], "a");
        const int n = snprintf(NULL, 0, "%lu", duration);
        char buf[n+1];
        int c = snprintf(buf, n+1, "%lu", duration);
        fputs(buf, fp);
        fputs("\n", fp);
        fclose(fp);
        printf("%ld\n", duration);
        printf("%s\n", "");
        return child_status;
     }
}