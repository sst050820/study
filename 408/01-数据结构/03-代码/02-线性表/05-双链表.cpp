#include <stdio.h>
#include <stdlib.h>
typedef int ElemType; //双链表的元素类型

// ======双链表结构体定义======
typedef struct DNode {
    ElemType data; //数据域：存储元素的值
    struct DNode *prior; //前驱指针域：指向前一个节点
    struct DNode *next; //后继指针域：指向后一个节点
} DNode, *DLinkList; //双链表的结构体类型
// DNode是节点类型；DLinkList 是指向节点的指针类型（即"双链表"类型）

// ======1. 双链表初始化（有头节点）======
bool InitList(DLinkList &L) {
    L = (DNode *)malloc(sizeof(DNode)); //为头节点分配内存空间
    if(L == NULL)
        return false; //内存分配失败，返回false
    L->next = NULL; //头节点的后继指针域指向空
    L->prior = NULL; //头节点的前驱指针域指向空（头节点没有前驱）
    return true;
}

// ======2. 头插法 创建双链表（逆序输入）======
DLinkList List_HeadInsert(DLinkList &L) {
    DNode *s; //定义一个指针s，用于创建新节点
    ElemType x; //定义一个变量x，用于存储输入的元素值
    L = (DNode *)malloc(sizeof(DNode)); //为头节点分配内存空间
    L->next = NULL; //头节点的后继指针域指向空
    L->prior = NULL; //头节点的前驱指针域指向空
    scanf("%d", &x);
    while(x != 9999) { //输入9999表示结束输入
        s = (DNode *)malloc(sizeof(DNode)); //为新节点分配内存空间
        s->data = x; //新节点的数据域赋值为输入的元素值
        s->next = L->next; //新节点的后继指向原第一个节点
        s->prior = L; //新节点的前驱指向头节点
        if(L->next != NULL) //如果原第一个节点存在（不是空表）
            L->next->prior = s; //原第一个节点的前驱指向新节点
        L->next = s; //头节点的后继指向新节点
        scanf("%d", &x);
    }
    return L; //返回头节点指针
}

// ======3. 尾插法 创建双链表======
DLinkList List_TailInsert(DLinkList &L) {
    DNode *s, *r; //定义两个指针s和r，s用于创建新节点，r用于指向尾节点
    ElemType x; //定义一个变量x，用于存储输入的元素值
    L = (DNode *)malloc(sizeof(DNode)); //为头节点分配内存空间
    L->prior = NULL; //头节点的前驱指向空
    r = L; //r指向头节点（初始时尾节点就是头节点）
    scanf("%d", &x);
    while(x != 9999) { //输入9999表示结束输入
        s = (DNode *)malloc(sizeof(DNode)); //为新节点分配内存空间
        s->data = x; //新节点的数据域赋值为输入的元素值
        s->prior = r; //新节点的前驱指向当前尾节点
        r->next = s; //当前尾节点的后继指向新节点
        r = s; //r指向新节点，成为新的尾节点
        scanf("%d", &x);
    }
    r->next = NULL; //尾节点的后继指向空
    return L; //返回头节点指针
}

// ======4. 双链表按位查找元素======
DNode *GetElem(DLinkList L, int i) {
    if(i < 0)
        return NULL; //位序i不合法，返回NULL
    DNode *p = L; //定义一个指针p，指向头节点
    int j = 0; //定义一个计数器j，初始值为0
    while(p != NULL && j < i) { //当p不为空且j小于i时，循环查找第i个节点
        p = p->next; //指针p指向下一个节点
        j++; //计数器j加1
    }
    return p; //返回指向第i个节点的指针p，如果未找到则返回NULL
}

// ======5. 双链表按值查找元素（考研标准版：返回节点指针）======
DNode *LocateElem(DLinkList L, ElemType e) {
    DNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != NULL && p->data != e) { //当p不为空且p指向的节点的数据域不等于e时，循环查找
        p = p->next; //指针p指向下一个节点
    }
    return p; //返回指向e的节点的指针p，如果未找到则返回NULL
}

// ======5-补. 双链表按值查找（返回位序，辅助用）======
int LocateElemPos(DLinkList L, ElemType e) {
    DNode *p = L->next; //定义一个指针p，指向第一个节点
    int i = 1; //定义一个计数器i，初始值为1
    while(p != NULL && p->data != e) { //当p不为空且p指向的节点的数据域不等于e时，循环查找
        p = p->next; //指针p指向下一个节点
        i++; //计数器i加1
    }
    if(p == NULL)
        return 0; //如果未找到，则返回0
    else
        return i; //返回e的位序i
}

// ======6. 插入：在第i个位置插入元素e（★★★ 双链表重点，改4个指针）======
bool ListInsert(DLinkList &L, int i, ElemType e) {
    if(i < 1)
        return false; //位序i不合法，返回false
    DNode *p = GetElem(L, i - 1); //查找第i-1个节点（前驱节点）
    if(p == NULL)
        return false; //第i-1个节点不存在，返回false
    DNode *s = (DNode *)malloc(sizeof(DNode)); //为新节点分配内存空间
    s->data = e; //新节点的数据域赋值为e
    s->prior = p; //① 新节点的前驱指向前驱节点p
    s->next = p->next; //② 新节点的后继指向原第i个节点
    if(p->next != NULL) //如果原第i个节点存在（不是在表尾插入）
        p->next->prior = s; //③ 原第i个节点的前驱指向新节点
    p->next = s; //④ 前驱节点p的后继指向新节点
    return true; //返回true
}

// ======7. 删除：删除第i个位置的元素e（★★★ 双链表重点，改2个指针）======
bool ListDelete(DLinkList &L, int i, ElemType &e) {
    if(i < 1)
        return false; //位序i不合法，返回false
    DNode *p = GetElem(L, i - 1); //查找第i-1个节点（前驱节点）
    if(p == NULL || p->next == NULL)
        return false; //第i-1个节点不存在或第i个节点不存在，返回false
    DNode *q = p->next; //q指向第i个节点（被删节点）
    e = q->data; //将第i个节点的数据域赋值给e
    p->next = q->next; //① 前驱节点p的后继指向第i+1个节点
    if(q->next != NULL) //如果第i+1个节点存在（不是删除尾节点）
        q->next->prior = p; //② 第i+1个节点的前驱指向前驱节点p
    free(q); //③ 释放被删节点的内存空间
    return true; //返回true
}

// ======8. 求双链表的长度======
int Length(DLinkList L) {
    int len = 0; //定义一个计数器len，初始值为0
    DNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != NULL) { //当p不为空时，循环计数
        len++; //计数器len加1
        p = p->next; //指针p指向下一个节点
    }
    return len; //返回双链表的长度len
}

// ======9. 判断双链表是否为空======
bool Empty(DLinkList L) {
    return L->next == NULL; //如果头节点的后继指针域指向空，则双链表为空，返回true；否则返回false
}

// ======10. 打印双链表（从前往后）======
void PrintList(DLinkList L) {
    DNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != NULL) { //当p不为空时，循环打印
        printf("%d ", p->data); //打印节点的数据域
        p = p->next; //指针p指向下一个节点
    }
    printf("\n"); //打印换行符
}

// ======10-补. 逆序打印双链表（双链表特色功能，单链表做不到这么方便）======
void PrintListReverse(DLinkList L) {
    DNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p->next != NULL) //先走到尾节点
        p = p->next;
    while(p != L) { //从尾节点往回走，直到头节点
        printf("%d ", p->data); //打印节点的数据域
        p = p->prior; //指针p指向前一个节点（双链表才能这样往回走）
    }
    printf("\n"); //打印换行符
}

// ======11. 销毁双链表======
void DestroyList(DLinkList &L) {
    DNode *p; //定义一个指针p，指向当前节点
    while(L != NULL) { //当L不为空时，循环销毁
        p = L; //p指向当前节点
        L = L->next; //L指向下一个节点
        free(p); //释放当前节点的内存空间
    }
}

// ======主函数：测试所有函数操作======
int main() {
    DLinkList L, L1, L2; //定义双链表L、L1、L2
    InitList(L); //初始化双链表L

    //头插法测试
    printf("请输入双链表的元素（以9999结束）：\n");
    List_HeadInsert(L1); //头插法创建双链表L1
    printf("头插法创建的双链表为：");
    PrintList(L1); //打印双链表L1
    printf("头插法逆序打印为：");
    PrintListReverse(L1); //逆序打印双链表L1

    //尾插法测试
    printf("请输入双链表的元素（以9999结束）：\n");
    List_TailInsert(L2); //尾插法创建双链表L2
    printf("尾插法创建的双链表为：");
    PrintList(L2); //打印双链表L2
    printf("尾插法逆序打印为：");
    PrintListReverse(L2); //逆序打印双链表L2

    //依次插入
    for(int i = 1; i <= 15; i++) {
        ListInsert(L, i, 100 + i); //在第i个位置插入元素100+i
    }
    printf("初始双链表为：");
    PrintList(L); //打印双链表L

    //第a位插入元素j
    int a, j;
    printf("请输入要插入的位序和元素值（以空格分隔）：");
    scanf("%d %d", &a, &j); //输入要插入的位序和元素值
    if(a < 1 || a > Length(L) + 1) {
        printf("位序%d不合法\n", a); //如果位序a不合法，打印错误信息
        printf("请重新输入位序和元素值：");
        scanf("%d %d", &a, &j); //重新输入要插入的位序和元素值
    }
    ListInsert(L, a, j); //在第a个位置插入元素j
    printf("插入后的双链表为：");
    PrintList(L); //打印插入后的双链表L

    //第b位删除元素
    int b, e;
    printf("请输入要删除的位序：");
    scanf("%d", &b); //输入要删除的位序
    if(b < 1 || b > Length(L)) {
        printf("位序%d不合法\n", b); //如果位序b不合法，打印错误信息
        printf("请重新输入位序：");
        scanf("%d", &b); //重新输入要删除的位序
    }
    ListDelete(L, b, e); //删除第b个位置的元素e
    printf("删除后的双链表为：");
    PrintList(L); //打印删除后的双链表L

    //按位查找元素
    int c;
    printf("请输入要查找的位序：");
    scanf("%d", &c); //输入要查找的位序
    if(c < 1 || c > Length(L)) {
        printf("位序%d不合法\n", c); //如果位序c不合法，打印错误信息
        printf("请重新输入位序：");
        scanf("%d", &c); //重新输入要查找的位序
    }
    DNode *p = GetElem(L, c); //查找第c个节点
    if(p == NULL) {
        printf("第%d个节点不存在\n", c); //如果未找到则打印错误信息
    } else {
        printf("第%d个节点的数据域为：%d\n", c, p->data); //如果找到则打印节点的数据域
    }

    //按值查找元素
    int d;
    printf("请输入要查找的元素值：");
    scanf("%d", &d); //输入要查找的元素值
    DNode *q = LocateElem(L, d); //查找值为d的节点
    if(q == NULL) {
        printf("值为%d的节点不存在\n", d); //如果未找到则打印错误信息
    } else {
        printf("值为%d的节点存在，数据域为：%d，位序为：%d\n", d, q->data, LocateElemPos(L, d)); //如果找到则打印节点信息
    }

    //打印双链表长度
    printf("双链表的长度为：%d\n", Length(L)); //打印双链表的长度
    //判断双链表是否为空
    if(Empty(L)) {
        printf("双链表为空\n"); //如果双链表为空，打印信息
    } else {
        printf("双链表不为空\n"); //如果双链表不为空，打印信息
    }

    //逆序打印（双链表特色）
    printf("双链表逆序打印为：");
    PrintListReverse(L); //逆序打印双链表L

    //销毁双链表
    DestroyList(L); //销毁双链表L
    DestroyList(L1); //销毁双链表L1
    DestroyList(L2); //销毁双链表L2
    printf("双链表已销毁\n"); //打印销毁信息
    return 0; //返回0，表示程序正常结束
}
