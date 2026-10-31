#include <stdio.h>
#include <stdlib.h>
typedef int ElemType; //循环双链表的元素类型

// ======循环双链表结构体定义======
typedef struct DNode {
    ElemType data; //数据域：存储元素的值
    struct DNode *prior; //前驱指针域：指向前一个节点
    struct DNode *next; //后继指针域：指向后一个节点
} DNode, *DLinkList; //循环双链表的结构体类型
// DNode是节点类型；DLinkList 是指向节点的指针类型（即"循环双链表"类型）
// ★ 节点结构和双链表完全相同（data+prior+next），只是逻辑上首尾双向相连

// ======1. 循环双链表初始化（有头节点）======
bool InitList(DLinkList &L) {
    L = (DNode *)malloc(sizeof(DNode)); //为头节点分配内存空间
    if(L == NULL)
        return false; //内存分配失败，返回false
    L->next = L; //★ 头节点的后继指针域指向自己（空表）
    L->prior = L; //★ 头节点的前驱指针域也指向自己（空表，和双链表不同！）
    return true;
}

// ======2. 头插法 创建循环双链表（逆序输入）======
DLinkList List_HeadInsert(DLinkList &L) {
    DNode *s; //定义一个指针s，用于创建新节点
    ElemType x; //定义一个变量x，用于存储输入的元素值
    L = (DNode *)malloc(sizeof(DNode)); //为头节点分配内存空间
    L->next = L; //头节点的后继指向自己（空表）
    L->prior = L; //头节点的前驱指向自己（空表）
    scanf("%d", &x);
    while(x != 9999) { //输入9999表示结束输入
        s = (DNode *)malloc(sizeof(DNode)); //为新节点分配内存空间
        s->data = x; //新节点的数据域赋值为输入的元素值
        s->next = L->next; //① 新节点的后继指向原第一个节点
        s->prior = L; //② 新节点的前驱指向头节点
        L->next->prior = s; //③ 原第一个节点的前驱指向新节点
        L->next = s; //④ 头节点的后继指向新节点
        scanf("%d", &x);
    }
    return L; //返回头节点指针
}

// ======3. 尾插法 创建循环双链表======
DLinkList List_TailInsert(DLinkList &L) {
    DNode *s, *r; //定义两个指针s和r，s用于创建新节点，r用于指向尾节点
    ElemType x; //定义一个变量x，用于存储输入的元素值
    L = (DNode *)malloc(sizeof(DNode)); //为头节点分配内存空间
    L->next = L; //头节点的后继指向自己（空表）
    L->prior = L; //头节点的前驱指向自己（空表）
    r = L; //r指向头节点（初始时尾节点就是头节点）
    scanf("%d", &x);
    while(x != 9999) { //输入9999表示结束输入
        s = (DNode *)malloc(sizeof(DNode)); //为新节点分配内存空间
        s->data = x; //新节点的数据域赋值为输入的元素值
        s->prior = r; //① 新节点的前驱指向当前尾节点
        s->next = L; //② 新节点的后继指向头节点（循环特性）
        r->next = s; //③ 当前尾节点的后继指向新节点
        L->prior = s; //④ ★ 头节点的前驱指向新节点（新节点成为尾节点，和双链表不同！）
        r = s; //r指向新节点，成为新的尾节点
        scanf("%d", &x);
    }
    return L; //返回头节点指针
}

// ======4. 循环双链表按位查找元素======
DNode *GetElem(DLinkList L, int i) {
    if(i < 0)
        return NULL; //位序i不合法，返回NULL
    DNode *p = L; //定义一个指针p，指向头节点
    int j = 0; //定义一个计数器j，初始值为0
    while(j < i && p->next != L) { //当还没找到第i个节点且还没转完一圈时，继续查找
        p = p->next; //指针p指向下一个节点
        j++; //计数器j加1
    }
    return p; //返回指向第i个节点的指针p
}

// ======5. 循环双链表按值查找元素（考研标准版：返回节点指针）======
DNode *LocateElem(DLinkList L, ElemType e) {
    DNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != L && p->data != e) { //★ 循环结束条件：p != L（转了一圈回到头节点）
        p = p->next; //指针p指向下一个节点
    }
    if(p == L)
        return NULL; //如果转了一圈都没找到，返回NULL
    return p; //返回指向e的节点的指针p
}

// ======6. 插入：在第i个位置插入元素e（★ 改4个指针，和双链表相同）======
bool ListInsert(DLinkList &L, int i, ElemType e) {
    if(i < 1)
        return false; //位序i不合法，返回false
    DNode *p = GetElem(L, i - 1); //查找第i-1个节点（前驱节点）
    DNode *s = (DNode *)malloc(sizeof(DNode)); //为新节点分配内存空间
    s->data = e; //新节点的数据域赋值为e
    s->prior = p; //① 新节点的前驱指向前驱节点p
    s->next = p->next; //② 新节点的后继指向原第i个节点
    p->next->prior = s; //③ 原第i个节点的前驱指向新节点
    p->next = s; //④ 前驱节点p的后继指向新节点
    return true; //返回true
    // ★ 循环双链表的插入和双链表完全相同！因为循环特性自动维护（p->next可能是头节点，头节点的prior会被③自动更新）
}

// ======7. 删除：删除第i个位置的元素e（★ 改2个指针，和双链表相同）======
bool ListDelete(DLinkList &L, int i, ElemType &e) {
    if(i < 1)
        return false; //位序i不合法，返回false
    DNode *p = GetElem(L, i - 1); //查找第i-1个节点（前驱节点）
    if(p->next == L)
        return false; //前驱的next指向头节点，说明第i个节点不存在（i超过表长）
    DNode *q = p->next; //q指向第i个节点（被删节点）
    e = q->data; //将第i个节点的数据域赋值给e
    p->next = q->next; //① 前驱节点p的后继指向第i+1个节点
    q->next->prior = p; //② 第i+1个节点的前驱指向前驱节点p
    free(q); //③ 释放被删节点的内存空间
    return true; //返回true
}

// ======8. 求循环双链表的长度======
int Length(DLinkList L) {
    int len = 0; //定义一个计数器len，初始值为0
    DNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != L) { //当p没有转回头节点时，循环计数
        len++; //计数器len加1
        p = p->next; //指针p指向下一个节点
    }
    return len; //返回循环双链表的长度len
}

// ======9. 判断循环双链表是否为空======
bool Empty(DLinkList L) {
    return L->next == L; //★ 头节点的后继指向自己即为空表（prior也必然指向自己）
}

// ======10. 打印循环双链表（从前往后）======
void PrintList(DLinkList L) {
    DNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != L) { //当p没有转回头节点时，循环打印
        printf("%d ", p->data); //打印节点的数据域
        p = p->next; //指针p指向下一个节点
    }
    printf("\n"); //打印换行符
}

// ======10-补. 逆序打印循环双链表（从后往前，双链表特色）======
void PrintListReverse(DLinkList L) {
    DNode *p = L->prior; //★ 头节点的prior直接指向尾节点（循环双链表特性，不用遍历找尾）
    while(p != L) { //从尾节点往回走，直到头节点
        printf("%d ", p->data); //打印节点的数据域
        p = p->prior; //指针p指向前一个节点
    }
    printf("\n"); //打印换行符
}

// ======11. 销毁循环双链表======
void DestroyList(DLinkList &L) {
    DNode *p; //定义一个指针p，指向当前要释放的节点
    while(L->next != L) { //当链表中还有数据节点时
        p = L->next; //p指向第一个数据节点
        L->next = p->next; //头节点指向第二个数据节点
        free(p); //释放第一个数据节点
    }
    free(L); //最后释放头节点
    L = NULL; //头指针置空，防止野指针
}

// ======主函数：测试所有函数操作======
int main() {
    DLinkList L, L1, L2; //定义循环双链表L、L1、L2
    InitList(L); //初始化循环双链表L

    //头插法测试
    printf("请输入循环双链表的元素（以9999结束）：\n");
    List_HeadInsert(L1); //头插法创建循环双链表L1
    printf("头插法创建的循环双链表为：");
    PrintList(L1); //打印循环双链表L1
    printf("头插法逆序打印为：");
    PrintListReverse(L1); //逆序打印循环双链表L1

    //尾插法测试
    printf("请输入循环双链表的元素（以9999结束）：\n");
    List_TailInsert(L2); //尾插法创建循环双链表L2
    printf("尾插法创建的循环双链表为：");
    PrintList(L2); //打印循环双链表L2
    printf("尾插法逆序打印为：");
    PrintListReverse(L2); //逆序打印循环双链表L2

    //依次插入
    for(int i = 1; i <= 15; i++) {
        ListInsert(L, i, 100 + i); //在第i个位置插入元素100+i
    }
    printf("初始循环双链表为：");
    PrintList(L); //打印循环双链表L

    //第a位插入元素j
    int a, j;
    printf("请输入要插入的位序和元素值（以空格分隔）：");
    scanf("%d %d", &a, &j); //输入要插入的位序和元素值
    if(a < 1 || a > Length(L) + 1) {
        printf("位序%d不合法\n", a);
        printf("请重新输入位序和元素值：");
        scanf("%d %d", &a, &j);
    }
    ListInsert(L, a, j); //在第a个位置插入元素j
    printf("插入后的循环双链表为：");
    PrintList(L); //打印插入后的循环双链表L

    //第b位删除元素
    int b, e;
    printf("请输入要删除的位序：");
    scanf("%d", &b); //输入要删除的位序
    if(b < 1 || b > Length(L)) {
        printf("位序%d不合法\n", b);
        printf("请重新输入位序：");
        scanf("%d", &b);
    }
    ListDelete(L, b, e); //删除第b个位置的元素e
    printf("删除后的循环双链表为：");
    PrintList(L); //打印删除后的循环双链表L

    //按位查找元素
    int c;
    printf("请输入要查找的位序：");
    scanf("%d", &c); //输入要查找的位序
    if(c < 1 || c > Length(L)) {
        printf("位序%d不合法\n", c);
        printf("请重新输入位序：");
        scanf("%d", &c);
    }
    DNode *p = GetElem(L, c); //查找第c个节点
    printf("第%d个节点的数据域为：%d\n", c, p->data); //打印节点的数据域

    //按值查找元素
    int d;
    printf("请输入要查找的元素值：");
    scanf("%d", &d); //输入要查找的元素值
    DNode *q = LocateElem(L, d); //查找值为d的节点
    if(q == NULL) {
        printf("值为%d的节点不存在\n", d);
    } else {
        printf("值为%d的节点存在，数据域为：%d\n", d, q->data);
    }

    //打印长度和判空
    printf("循环双链表的长度为：%d\n", Length(L));
    printf("循环双链表是否为空：%s\n", Empty(L) ? "是" : "否");

    //销毁
    DestroyList(L);
    DestroyList(L1);
    DestroyList(L2);
    printf("循环双链表已销毁\n");
    return 0;
}
