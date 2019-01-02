#include <stdio.h>

void explode_bomb(){
    printf("BOOOOM");
    printf("Bomb has exploded!");
}

void phase_1(char *string){

    int d1, d2, d3, d4;
    int i;
    int re = 0;
    sscanf(string, "%d %d %d %d", &d1,&d2,&d3,&d4);
    while(re + d4 < d1 + d2){
        re++;
    }
    if(d3 != re){
        explode_bomb();
        return;
    }
    else{
        printf("Bomb has been defused!");
    }
}


int main(void){
    char str[] = "2 6 5 3";
    phase_1(str);
    return 0;
}