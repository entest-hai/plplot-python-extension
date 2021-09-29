// 29 SEP 2021 Hai Tran 
// gcc -shared -o libtest.so -fPIC add.c
// gcc -o app test.c 

#include<stdio.h>

void myprint(){
    printf("tran minh hai\n");
}

void add(int a, int b){
    printf("%d + %d = %d\n", a, b, a+b);
}

void sum(int* array, int len){
    for (int i = 0; i < len; i++){
        printf("i=%d, a=%d \n",i,array[i]);
    }
}

int mul(int a, int b){
    return a*b;
}


int plot_ctg(double* mhr, double* fhr, double* ua, int len){
    for (int i = 0; i < len; i++){
        printf("i=%d,mhr=%3.1f,fhr=%3.1f,ua=%3.1f \n",i,mhr[i],fhr[i],ua[i]);
    }
    return 1;
}

void main(){
    int c = 0; 
    int array[10] = {1,2,3,4,5,6,7,8,9,10};
    int len = 10; 
    double mhr[10] = {60.0,60.1,60.2,60.3,60.4,60.5,60.6,60.7,60.8,60.9};
    double fhr[10] = {60.0,60.1,60.2,60.3,60.4,60.5,60.6,60.7,60.8,60.9};
    double ua[10] = {60.0,60.1,60.2,60.3,60.4,60.5,60.6,60.7,60.8,60.9};

    myprint();
    add(100,200);
    sum(array,10);
    c = mul(10,10);
    printf("mul %d, %d = %d",10,10,c);
    plot_ctg(mhr,fhr,ua,len); 
}

