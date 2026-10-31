#include <stdio.h>

//void test(int x) {
void test(int &x) { //注意“&”的使用区别
    x = 1024;
    printf("test函数内部 x = %d\n", x);
}
int main() {
    int x = 10;
    printf("调用test函数前， x = %d\n", x);
    test(x);
    printf("调用test函数后， x = %d\n", x);
    return 0;
}
