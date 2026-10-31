#include <stdio.h> //包含stdio.h头文件，用于输入输出
#include <stdlib.h> //包含stdlib.h头文件，用于动态分配内存
#define InitSize 10 //顺序表初始最大容量
typedef int ElemType; //顺序表的元素类型

// ======顺序表结构体定义（动态分配）======
typedef struct {
    ElemType *data; //顺序表的存储空间（动态分配）
    int length; //顺序表的当前长度
    int MaxSize; //顺序表当前最大容量
} SqList; //顺序表的结构体类型

// ======1. 顺序表初始化(动态分配)）======
void InitList(SqList &L) {
    L.data = (ElemType *)malloc(InitSize * sizeof(ElemType)); //为顺序表分配存储空间
    L.length = 0; //空表，长度和最大容量初始化为0
    L.MaxSize = InitSize; //最大容量为InitSize
}

// ======2. 顺序表销毁(释放内存)======
void DestroyList(SqList &L) {
    free(L.data); //释放顺序表的存储空间
    L.data = NULL; //将顺序表的存储空间指针赋值为NULL
    L.length = 0; //将顺序表的长度赋值为0
    L.MaxSize = 0; //将顺序表的最大容量赋值为0
}

//=======3. 扩容(动态分配的特色操作)======
void ListInsert(SqList &L, int len) {
        ElemType *p = L.data;
        L.data = (ElemType *)malloc(sizeof(ElemType) * (L.MaxSize + len)); //为顺序表分配新的存储空间
        for(int i=0; i<L.length; i++) //将旧的元素复制到新的存储空间中
            L.data[i] = p[i]; //将旧的元素赋值给新的存储空间中的元素
        L.MaxSize += len; //最大容量扩展为L.MaxSize + len
        free(p); //释放旧的存储空间
}

// ======4. 顺序表插入元素======
bool ListInsert(SqList &L, int i, ElemType e) {
    if(i < 1 || i > L.length + 1) //位序i是否合法
        return false;
    if(L.length >= L.MaxSize) //顺序表已满
        ListInsert(L, InitSize); //扩容顺序表
    for(int j = L.length; j >= i; j--) //将第i个位置及之后的元素向后移动
        L.data[j] = L.data[j-1];
    L.data[i-1] = e; //将新元素插入到第i个位置（下标为i-1）
    L.length++; //顺序表长度加1
    return true; //插入成功，返回true
}

// ======5. 顺序表删除元素======
bool ListDelete(SqList &L,int i, ElemType &e) {
    if(i<1 || i>L.length) //位序i是否合法
        return false;
    e=L.data[i-1]; //将被删除的元素赋值给e,保存被删除的元素
    for(int j=i; j<L.length; j++)
        L.data[j-1]=L.data[j]; //将第i个位置之后的元素向前移动
    L.length--; //顺序表长度减1
    return true; //删除成功，返回true
}

// ======6. 顺序表按位查找元素======
/*
ElemType GetElem(SqList L, int i) { //没有判断位序i是否合法
    return L.data[i-1]; //返回第i个位置的元素（下标为i-1）
}
*/
bool GetElem(SqList L, int i, ElemType &e) { //有判断位序i是否合法
    if(i<1 || i>L.length) //位序i是否合法
        return false;
    e=L.data[i-1]; //将第i个位置的元素赋值给e
    return true; //查找成功，返回true
}

// ======7. 顺序表按值查找元素======
int LocateElem(SqList L, ElemType e) {
    for(int i=0; i<L.length; i++) {
        if(L.data[i]==e)
            return i+1; //返回元素e的位序（下标为i+1）
    }
    return 0; //未找到元素e，返回0
}

// ======8. 求顺序表的长度======
int ListLength(SqList L) {
    return L.length;
}

// ======9. 判断顺序表是否为空======
bool ListEmpty(SqList L) {
    return L.length == 0; //如果顺序表的长度为0，返回true，否则返回false
}

// ======10. 打印顺序表======
void PrintList(SqList L) {
    for(int i=0; i<L.length; i++) {
        printf("%d ", L.data[i]); //打印顺序表中的元素
    }
    printf("\n");

}

// ======主函数：测试所有函数操作======
int main() {
    SqList L; //创建一个顺序表L

    InitList(L); //初始化顺序表L
    printf("顺序表L初始化后：长度为：%d，最大容量为：%d\n", L.length, L.MaxSize); //输出顺序表L的长度和最大容量

    //在顺序表L中插入15个元素
    for(int i=1; i<=15; i++) //在顺序表L中插入15个元素
        ListInsert(L, i, 100+i); //在第i个位置插入元素100+i
    printf("顺序表L的长度为：%d，最大容量为：%d\n", L.length, L.MaxSize); //输出顺序表L的长度和最大容量
    PrintList(L); //输出顺序表中的元素

    //删除第a个位置的元素
    ElemType delVal; //定义一个变量delVal，用于保存被删除的元素
    int a = 2; //定义一个变量a，用于保存被删除的元素的位置
    if(ListDelete(L, a, delVal)) //删除第a个位置的元素，并将被删除的元素保存到delVal
        printf("删除第%d个位置的元素，被删除的元素为：%d\n", a, delVal); //输出被删除的元素
    else //删除失败
        printf("删除第%d个位置的元素失败\n", a);
    printf("顺序表L的长度为：%d，最大容量为：%d\n", L.length, L.MaxSize); //输出顺序表L的长度和最大容量
    PrintList(L); //输出顺序表中的元素

    //查找第b个位置的元素
    ElemType getVal; //定义一个变量getVal，用于保存查找的元素
    int b = 7; //定义一个变量b，用于保存查找的元素的位置
    if(GetElem(L, b, getVal)) 
        printf("第%d个位置的元素为：%d\n", b, getVal); //输出第b个位置的元素
    else //查找失败
        printf("第%d个位置不存在元素\n", b);

    //按值查找元素
    int c = 105; //定义一个变量c，用于保存要查找的元素的值
    int pos = LocateElem(L, c); //查找元素c在顺序表L中的位置
    if(pos != 0) //如果找到元素c
        printf("元素%d在顺序表L中的位置为：%d\n", c, pos);
    else //如果未找到元素c
        printf("元素%d不在顺序表L中\n", c);

    DestroyList(L); //销毁顺序表L，释放内存
    printf("顺序表L销毁后：长度为：%d，最大容量为：%d\n", L.length, L.MaxSize); //输出顺序表L的长度和最大容量

    return 0;
}