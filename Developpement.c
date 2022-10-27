#include <stdio.h>
#include <string.h>

void int_switch(int *a, int *b){
    int temp;
    int *pA, *pB;

    pA = &a;
    pB = &b;

    printf("valeur de départ -> a = %d, b = %d\n", *pA,*pB);

    temp = *pB;
    *pB = *pA;
    *pA = temp;

    printf("valeur modifiée -> a = %d, b = %d\n", *pA,*pB);
}

int *tab(int tableau[3], int size){
    int *pTab = tableau;
    printf("%d \n", *pTab);
}

int *creation_tab(){
    int tableau[4];
    int *pTab = tableau;

    printf("%p", *pTab);
    return *pTab;

}

int str_func(char chn_caract[], int decalage){

    for(int i = 0; chn_caract[i] != '\0'; i++){
        char *ptr_tab = &chn_caract[i];
        *ptr_tab += decalage;
        printf("%c",chn_caract[i]);
    }
}

char *check_char(char letter, char ptr_letter[]){

    for(int i = 0; ptr_letter[i] != '\0'; i++){
        if(ptr_letter[i] == letter){
            char *ptr_addr = &ptr_letter[i];
            return &ptr_addr;
        }
    }
    return NULL;
}

int main() {
    /*nt a = 5;
    int b = 45;
    int * pA;
    pA = &a;

    printf("Avant changement :\n");
    printf("a = %d\n",*pA);

    *pA = 10;

    printf("Après changement :\n");
    printf("a = %d\n\n",*pA);

    printf("Switch de valeurs : \n");

    int_switch(a,b);

    printf("\nChangement de STR (version ascii)\n");
    char c = 'a';
    char *pC = &c;
    printf("Départ -> c : %c\n", *pC);
    (*pC)++;
    printf("Modif -> c : %c\n", *pC);

    int tableau[3] = {1,2,3};
    tab(tableau, 3);

    int *ptr_tab = creation_tab();


    char cesar[] = "hello";
    str_func(cesar, 11);*/

    char string[] = "abcdef";
    check_char('a', string);

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