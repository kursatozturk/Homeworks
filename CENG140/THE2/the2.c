#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LIMIT 1000
#define K_SIZE 3
#define V_SIZE 5

typedef struct node{
    int key;
    char value[V_SIZE];
    struct node *next;
    struct node *down;
} node;

node *init(int branch){
    node *head, *zero;
    head = malloc(sizeof(node));
    if(head==NULL){return head;}
    zero = malloc(sizeof(node));
    if(zero==NULL){return zero;}
    head->key = branch*1000;
    head->down = zero;
    zero->key = 0;
    return head;
}

void clear(node *list){
    node *hor, *ver, *temp;
    int flag;
    ver=list;
    temp=list->down;
    ver->next=NULL;
    free(ver);
    while(1){
        flag=0;
        hor=temp;
        if(temp->key!=0){
            flag=1;
            temp=temp->down;}
        while(hor->next!=NULL){
            ver=hor;
            hor=hor->next;
            ver->next=NULL;
            ver->down=NULL;
            free(ver);
        }
        hor->next=NULL;
        hor->down=NULL;
        free(hor);
        if(flag==0){break;}
    }
}

int is_empty(node *list){
    if(list->down->down!=NULL || list->down->next!=NULL){return 0;}
    else{return 1;}
}
int num_levels(node *list){
    return ((list->down->key)*-1)+1;
}
int num_nodes(node *list){
    node *temp;
    int counter=0;
    temp=list;
    while(temp->down!=NULL){
        temp = temp->down;
    }
    while(temp->next!=NULL){
        temp = temp->next;
        counter++;
    }
    return counter;
}

node *find(node *list, int key){
    node *temp, *hor;
    temp = list->down;
    if(key<=0){return NULL;}
    while(1){
        hor=temp;
        while(hor->key <=key){
            if(hor->key==key){
                break;
            }
            else{
                if(hor->next){
                    hor=hor->next;
                }
                else{break;}
            }
        }
        if(hor->key==key){
            return hor;
        }
        if(temp->key!=0){
            temp=temp->down;            
        }
        else{return NULL;}
    }
}

void path(node *list, int key){
    node *hor, *temp;
    hor=list->down;
    printf("%d v ",list->key);
    printf("%3d", hor->key);
    while(hor->next->key>key){
        hor=hor->down;
        printf(" v ");
        printf("%3d", hor->key);            
    }
    hor=hor->next;
    while(1){
        while(hor->key<=key){
            printf(" > ");
            printf("%3d", hor->key);
            temp=hor;
            if(hor->next!=NULL){hor=hor->next;}
            else{break;}
        }
        if(temp->key==key){
            printf(":");
            printf("%5s\n", temp->value);           
            break;
        }
        else{
            hor=temp;
            hor=hor->down;
            printf(" v ");
            printf("%3d", hor->key);
            temp = hor;
            hor=hor->next;
        }
    }
}


void print(node *list){
    node *temp;
    int num_level, num_node,i=0,key_num;
    num_level=num_levels(list);
    num_node=num_nodes(list);
    temp=list;
    printf("%3d      ",num_node);
    while(i<num_level){printf(" %d",i++);}
    printf("\n         ");
    for(i=0;i<num_level;i++){printf(" +");}
    printf(" -\n");
    while(temp->key!=0){temp=temp->down;}
    while(temp->next!=NULL){
        node *checkpoint;
        temp=temp->next;
        checkpoint=find(list, temp->key);
        key_num=checkpoint->key;
        printf("%3d:%5s",checkpoint->key,checkpoint->value);
        for(i=1;checkpoint->down;checkpoint=checkpoint->down,i++);
        for(key_num=0;key_num<i;key_num++){printf(" +");}
        printf("\n");
    }
}

void print_level(node *list, int level){
    node *temp,*behind,*horb, *hort;
    int m=0;
    temp=list->down;
    level=-level;
    if(num_levels(list)-1==(-level)){
        while(temp){
            printf("%3d:%5s ->",temp->key,temp->value);
            if(temp->next){printf(" ");}
            temp=temp->next;
        }
        printf(" ");
        printf("\n");
        return;
    }
    while(temp->key!=level){
        behind=temp;
        if(temp->down!=NULL){temp=temp->down;}
    }
    horb=behind;
    hort=temp;
    while(m++<13){printf(" ");}
    m=0;
    while(horb->next){
        int i=1,k=0;
        horb=horb->next;
        hort=hort->next;
        if(horb->key==hort->key){
            printf("%3d:%5s ->",horb->key,horb->value);
            if(horb->next){printf(" ");}
        }
        else{
            while(horb->key!=hort->key){
                hort=hort->next;
                i++;
            }
            while(k++<i-1){
                while(m++<13){printf(" ");}
                m=0;              
            }
            printf("%3d:%5s ->",horb->key,horb->value);
            if(horb->next){printf(" ");}         
        }
    }
    printf(" ");
    printf("\n");
    horb=behind;
    hort=temp;
    while(m++<16){printf(" ");}
    m=0;
    while(horb->next){
        int i=1,k=0;
        horb=horb->next;
        hort=hort->next;
        if(horb->key==hort->key){
            printf("|");
        }
        else{
            while(horb->key!=hort->key){
                hort=hort->next;
                i++;
            }
            while(k++<i-1){
                while(m++<13){printf(" ");}
                m=0;            
            }
            printf("|");
        }
        if(horb->next){
            while(m++<12){printf(" ");}
            m=0;
        }
    }
    printf("         ");
    printf("\n");
    while(temp){
        printf("%3d:%5s ->",temp->key,temp->value);
        if(temp->next){printf(" ");}
        temp=temp->next;
    }
    printf(" ");
    printf("\n");
}
int MyPow(int a,int b){
    if(b<0){
        return (  1 / MyPow (a,-b));
    }
    else if(b==0){
        return 1;
    }
    else if(b==1){
        return a;
    }
    else{
        return a*MyPow(a,b-1);
    }
}



node *insert(node *list, int key, char *value){
    node *p;
    node *q=list;
    if(find(list, key) != NULL){ // eger list icinde varsa
        p=find(list, key);
        while(p->down !=NULL){
            strncpy(p->value,value,V_SIZE);
            p=p->down;
        }
        strncpy(p->value,value,V_SIZE);
        return p;
    }
    else {
        node *new;
        node *new2;
        int i = 0;
        int branch;
        int num_nodes;
        int level = 0;
        int cntrl;
        new = malloc(sizeof(node));
        new->next=NULL;
        new->down=NULL;
        new->key = key;
        strncpy(new->value, value, V_SIZE);
        q->key++;
        branch = (q->key / 1000);
        num_nodes = (q->key % 1000);
        if (num_nodes % branch != 0) {
            p = list;
            while (p->down != NULL) p = p->down;   //en alt levela gidiyor
            if (p->next!=NULL && p->next->key > key) {  // ilk data node keyden buyukse
                new->next = p->next;
                p->next = new;
                return new;
            }
            p = p->next;
            while (p) {
                if (p->key < key && p->next!=NULL && p->next->key < key) p = p->next;
                else if (p->key < key && p->next!=NULL && p->next->key > key) {
                    new->next = p->next;
                    p->next = new;
                    return new;
                } else if (p->key < key && p->next == NULL) {
                    p->next = new;
                    new->next = NULL;
                    return new;
                }
            }
        }
        else {
        	cntrl=MyPow(branch, i) ;
            while (cntrl<= num_nodes && num_nodes% cntrl==0) { //hangi levellara gelecegini gelecek en ust leveli bularak yapiyorum
                level = i;
				i++;
				cntrl=MyPow(branch, i) ;
            }
            p = list;
            p = p->down;
            while (abs(p->key) > level) p = p->down;
            if (p->next == NULL) {              //ekleyecegimiz yeni nodu icin o level bos ise
                p->next = new;
                new->next = NULL;
                return new;
            }
            else {
                p = p->next;
                new2=new;
                while (p) {
                    if (p->key < key && p->next && p->next->key < key) p = p->next;
                    else if (p->key < key && p->next && p->next->key > key) {   // eger iki data node arasina konulacaksa
                        new2->next = p->next;
                        p->next = new2;
                        while (p->down != NULL) {   //konulacak en ust yeri bulduktan sonra asagi dogru hepsini tek tek ekliyorum
                            p = p->down;
                            new->next = p->next;
                            p->next=new;
                            new2->down = new;
                            new2=new;
                            if(p->down == NULL){
                                new2->down =NULL;
                            }
                        }
                        return new2;
                    }
                    else if (p->key <key && p->next==NULL){  // level da en sona konulacaksa
                        new2->next=NULL;
                        p->next=new2;
                        while(p->down != NULL){
                            p=p->down;
                            new->next =NULL;
                            p->next = new;
                            new2->down=new;
                            new2=new;
                            if (p->next != NULL){  //eger ilk level da sondaysa ama asagilarda sonda degilse
                                p = p->down;
                                new->next = p->next;
                                p->next=new;
                                new2->down = new;
                                new2=new;
                            }
                        }
                        if(p->down==NULL) {
                            new2->down = NULL;
                        }
                        return new2;
                    }
                }
            }
        }
    }
}

int delete(node *list, int key){}