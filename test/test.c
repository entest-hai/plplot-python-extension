// 29 SEP 2021 Hai Tran 
// gcc -L/home/haitran/c-workspace/ctypes/test/ -o app test.c -l:libtest.so
// gcc -shared -o lib.so -fPIC add.c


#include<stdio.h>

void myprint();

int main(){
    myprint();
    printf("testing so lib\n"); 
}