#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
typedef struct map_info{
    int N;
    char *path_to_mapper;
    char *path_to_reducer;
}map_info;
map_info m;

char cwd[1024];
void error_writer(char *msg){
    fprintf(stderr, "Error Message is : %s on process: %d\n", msg, getpid());
    fflush(stderr);
}

void controlled_close(int fd){
    if(close(fd) == -1){
        error_writer("Can't close fd");
    }
}
void controlled_dup(int newfd, int oldfd){
    if(dup2(newfd, oldfd) == -1){
        error_writer("Can't duplicate fd");
    }
}
map_info parse_params(int argc, char **argv){
    if(argc <= 1){
        printf("Missing parameters\n");
        exit(EXIT_FAILURE);
    }
    map_info m;
    sscanf (argv[1],"%d",&m.N);
    m.path_to_mapper = argv[2];
    if(argc == 4){
        // Has reducer
        m.path_to_reducer = argv[3];
    }
    else{
        m.path_to_reducer = NULL;
    }

    return m;
}

int read_input(void (*f)(char *)){
    char c = 0;
    int count = 0;
    int k = 10;
    char *line = malloc(k);
    do{
        
        fscanf(stdin, "%c", &c);
        if(!c){
            if (!count){
                return -1;
            }
            line[count] = 0;
            (*f)(line);
            free(line);
            return 0;
        }

        line[count] = c;
        count++;    
        if(count == k){
            k += 10;
            line = realloc(line, k);
            if(line == NULL){
                exit(EXIT_FAILURE);
            }
        }
        if(c == '\n'){
            line[count] = 0;
            (*f)(line);
            free(line);
            k = 10;
            count = 0;
            line = malloc(k);
            return 1;
        }

    }while(1);

}

void identity(char *msg){
    fprintf(stdout, "%s", msg);
    fflush(stdout);
}

void mapper(char *msg){
    fprintf(stdout, "%s", msg);
    fflush(stdout);

}

int main(int argc, char **argv){
    m = parse_params(argc, argv);

    getcwd(cwd, sizeof(cwd));
    int i;
    int fd_array[m.N][2];
    int mtrfd_array[m.N][2]; // mapper_to_reducer_fd_array
    int rtrfd_array[m.N - 1][2]; // reducer_to_reducer_fd_array
    int parent_pid = getpid();   
    char msg[150];
    sprintf(msg, "Parent id is : %d", parent_pid);
    error_writer(msg); 
    char has_reducer;
    if(m.path_to_reducer){
        has_reducer = 1;
    }
    else{
        has_reducer = 0;
    }
    pid_t child_pids[m.N];
    for(i = 0; i < m.N; i++){
        int *fd = fd_array[i];
        pipe(fd);
        pid_t pid = fork();
        if(!pid){
            //child process.
            char m_id[3];
            sprintf(m_id, "%d", i);

            controlled_close(fd[1]);
            controlled_dup(fd[0], STDIN_FILENO);
            controlled_close(fd[0]);
            if(has_reducer){
                int *map2red = mtrfd_array[i];
                int *red2red_r = NULL;
                int *red2red_w = NULL;
                
                pipe(map2red);

                if(i != 0){
                    red2red_r = rtrfd_array[i - 1];
                }
                if(i < m.N - 1){
                    red2red_w = rtrfd_array[i];
                }
                if(i < m.N - 1){
                    pipe(rtrfd_array[i]);
                }
                pid_t s_pid = fork();
                if(!s_pid){
                    // reducer
                    controlled_close(map2red[1]);
                    controlled_dup(map2red[0], STDIN_FILENO);
                    controlled_close(map2red[0]);
                    if(red2red_r){
                        controlled_dup(red2red_r[0], STDERR_FILENO);
                        controlled_close(red2red_r[0]);
                    }
                    if(red2red_w){
                        controlled_dup(red2red_w[1], STDOUT_FILENO);
                        controlled_close(red2red_w[1]);
                    }
                    int p = execl(m.path_to_reducer, m.path_to_reducer, m_id, (char *)0);
                    break;
                }
                else{
                    // mapper
                    controlled_close(map2red[0]);
                    controlled_dup(map2red[1], STDOUT_FILENO);
                    controlled_close(map2red[1]);
                }
            }
            execl(m.path_to_mapper, m.path_to_mapper, m_id, (char *)0);
            break;
        }
        else{
            child_pids[i] = pid;
            controlled_close(fd[0]);
     
        }
    }
    if(getpid() == parent_pid){
        int k = 0;

        while(1){
            int *fd = fd_array[(k%m.N)];
            k++;
            controlled_dup(fd[1], STDOUT_FILENO);
            int success = read_input(identity);
            if(success != 1){
                break;
            }
        }
        int cc;
        for(cc = 0; cc < m.N; cc++){
            int *fd = fd_array[cc];
            controlled_close(fd[1]);
        }
        int status;
        //wait(&status);
        //fprintf(stderr, "All children died %d\n", status);
    }
    return 0;
}