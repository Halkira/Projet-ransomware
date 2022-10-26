#include <stdio.h>

void int_switch(int *a, int *b){
    int * pA, pB;
    pA = &a;
    pB = &b;
}


int main() {
    int a = 5;
    int b = 45;
    int * pA;
    pA = &a;

    printf("a = %d, pA = %d, a* = %d, *pA = %d\n",a,pA,&a,&pA);

    *pA = 10;

    printf("Après changement :\n");
    printf("a = %d, pA = %d, a* = %d, *pA = %d\n",a,pA,&a,&pA);

    printf("&a = %d, &b = %d\n", &a,&b);
    int_switch(a,b);
    printf("&a = %d, &b = %d", &a,&b);
}

/*
char connection (IP_attaquant, IP_victime, port){

}

char read_file (filename){

}

char file_location (path){

}

char gen_key (){

}

char send_key (key){

}

char crypt (key, filename){

}

char decrypt (key, filename){

}

char delete_key (path_key){

}*/