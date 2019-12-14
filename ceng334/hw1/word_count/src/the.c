#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){

    char cwd[1024];
    getcwd(cwd, 1024);
    char *path = argv[1];
    printf("This is nonsense I swear.\n It is no good.\n nonsense is this how.\n");
    fprintf(stderr, "%s\n", path);
    int p = execl(path, path, (char*) 0);
    fprintf(stderr, "Why potato %d\n", p);

    return 0;

}