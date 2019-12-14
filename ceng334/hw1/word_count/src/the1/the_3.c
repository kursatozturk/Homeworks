#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define foreachexcept(i, m, except) for(int i = 0; i < m && !(except); i++)

#define foreach(i, m) for(int i = 0; i < m; i++)

#define PATH_MAX 4096

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
void msg_create_with_int(char* msg, int num){
    char line[256];
    sprintf(line, "%s is %d.", msg, num);
    error_writer(line);
}
void controlled_close(int fd){
    if(close(fd) == -1){
        error_writer("Can't close fd");
    }
}
void controlled_close1(int fd, char *msg){
    controlled_close(fd);
    error_writer(msg);
}
void controlled_dup(int newfd, int oldfd){

    if(dup2(newfd, oldfd) == -1){
        error_writer("Can't duplicate fd");
    }
}
char *path_adder(char *file){
    char *path = malloc(PATH_MAX);
    int last_letter;
    foreach(k, PATH_MAX){
        if(!cwd[k]){
            path[k++] = '/';
            last_letter = k;
            break;
        }
        path[k] = cwd[k];
    }
    foreach(k, PATH_MAX){
        if(!file[k]){
            path[k + last_letter] = 0;
            break;
        }
        path[k + last_letter] = file[k];
    }

    return path;

}
map_info parse_params(int argc, char **argv){
    if(argc <= 1){
        printf("Missing parameters\n");
        exit(EXIT_FAILURE);
    }
    getcwd(cwd, sizeof(cwd));
    map_info m;
    sscanf (argv[1],"%d",&m.N);
    m.path_to_mapper = path_adder(argv[2]);
    if(argc == 4){
        // Has reducer
        m.path_to_reducer = path_adder(argv[3]);
    }
    else{
        m.path_to_reducer = NULL;
    }

    return m;
}

int read_and_write_to_stdo(){
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
            fprintf(stdout, "%s", line);
            fflush(stdout);
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
            fprintf(stdout, "%s", line);
            fflush(stdout);
            free(line);
            k = 10;
            count = 0;
            line = malloc(k);
            return 1;
        }

    }while(1);

}

void close_both_end(int *fd){
    controlled_close(fd[0]);
    controlled_close(fd[1]);
}

int main(int argc, char **argv){
    m = parse_params(argc, argv);

    int i;
    int fd_array[m.N][2];
    int mtrfd_array[m.N][2]; // mapper_to_reducer_fd_array
    int rtrfd_array[m.N - 1][2]; // reducer_to_reducer_fd_array
    int parent_pid = getpid();
    char msg[150];
    char has_reducer;
    if(m.path_to_reducer){
        has_reducer = 1;
    }
    else{
        has_reducer = 0;
    }
    pid_t child_pids[2 * m.N];
    for(i = 0; i < m.N; i++){
        int *fd = fd_array[i];
        int *map2red = NULL;

        char m_id[3];
        sprintf(m_id, "%d", i);


        pipe(fd);
        if(has_reducer){

            map2red = mtrfd_array[i];
            pipe(map2red);
            if(i < m.N - 1){
                pipe(rtrfd_array[i]);
                }
        }

        pid_t pid = fork();
        if(!pid){
            //Mapper
            controlled_dup(fd[0], STDIN_FILENO);
            foreach(k, i + 1){
                close_both_end(fd_array[k]);
                }
            if(has_reducer){
                //duplicate stdout to the write end of pipe btw mapper and reducer
                controlled_dup(mtrfd_array[i][1], STDOUT_FILENO);

                // kill unused pipe ends
                foreach(k, i + 1){
                    close_both_end(mtrfd_array[k]);
                    if(k != m.N - 1){
                        close_both_end(rtrfd_array[k]);
                    }
                }

            }
            execl(m.path_to_mapper, m.path_to_mapper, m_id, (char *)0);
            break;
        }
        else{
            // Parent
            child_pids[i] = pid;
            if(has_reducer){
                int *map2red = mtrfd_array[i];
                int *red2red_r = NULL;
                int *red2red_w = NULL;


                if(i < m.N - 1){
                    red2red_w = rtrfd_array[i];
                }
                if(i > 0){
                    red2red_r = rtrfd_array[i - 1];
                }
                pid_t s_pid = fork();
                if(!s_pid){
                    //Reducer
                    controlled_dup(map2red[0], STDIN_FILENO);

                    if(red2red_r){
                        controlled_dup(red2red_r[0], STDERR_FILENO);
                    }
                    if(red2red_w){
                        controlled_dup(red2red_w[1], STDOUT_FILENO);
                    }

                    foreach(k, i + 1){
                        if(k != m.N-1){
                            close_both_end(rtrfd_array[k]);
                        }
                        close_both_end(mtrfd_array[k]);
                        close_both_end(fd_array[k]);
                    }
                    int p = execl(m.path_to_reducer, m.path_to_reducer, m_id, (char *)0);
                    break;
                }
                else{
                    child_pids[i + m.N] = s_pid;
                    // Parent

                }
            }
        }
    }
    if(getpid() == parent_pid){
        int k = 0;

        while(1){
            int *fd = fd_array[(k%m.N)];
            k++;
            controlled_dup(fd[1], STDOUT_FILENO);
            int success = read_and_write_to_stdo();
            if(success != 1){
                break;
            }
        }

        foreach(cc, m.N){
            int *fd = fd_array[cc];
            close_both_end(fd_array[cc]);
            if(has_reducer){
            close_both_end(mtrfd_array[cc]);
            if(cc != m.N -1){
                close_both_end(rtrfd_array[cc]);
            }
          }
        }
        close(1);
        int child_count = m.N;
        if(has_reducer){
            child_count *= 2;
        }
        foreach(k, child_count){
            int status;
            pid_t t;
            while(t = waitpid(child_pids[k], &status, 0) != -1);
            //msg_create_with_int("Message", t);
        }

    }
    return 0;
}
