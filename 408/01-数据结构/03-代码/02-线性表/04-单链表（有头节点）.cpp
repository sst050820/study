#include <stdio.h>
#include <stdlib.h>
typedef int ElemType; //单链表的元素类型

// ======单链表结构体定义======
typedef struct LNode {
    ElemType data; //数据域：存储元素的值
    struct LNode *next; //指针域：指向下一个节点
} LNode, *LinkList; //单链表的结构体类型
// LNode是节点类型；LinkList 是指向节点的指针类型（即“单链表”类型）

// ======1. 单链表初始化（有头节点）======
bool InitList(LinkList &L) {
    L=(LNode *)malloc(sizeof(LNode)); //为头节点分配内存空间
    if(L==NULL)
        return false;
    L->next=NULL; //头节点的指针域指向空
    return true;
   }

// ======2. 头插法 创建单链表（逆序输入）======
LinkList List_HeadInsert(LinkList &L) {
    LNode *s; //定义一个指针s，用于创建新节点
    ElemType x; //定义一个变量x，用于存储输入的元素值
    L = (LNode *)malloc(sizeof(LNode)); //为头节点分配内存空间
    L->next = NULL; //头节点的指针域指向空
    scanf("%d", &x);
    while(x != 9999) { //输入9999表示结束输入
        s = (LNode *)malloc(sizeof(LNode)); //为新节点分配内存空间
        s->data = x; //新节点的数据域赋值为输入的元素值
        s->next = L->next; //新节点的指针域指向原来的第一个节点
        L->next = s; //头节点的指针域指向新节点
        scanf("%d", &x);
    }
    return L; //返回头节点指针
}

// ======3. 尾插法 创建单链表======
LinkList List_TailInsert(LinkList &L) {
    LNode *s, *r; //定义两个指针s和r，s用于创建新节点，r用于指向尾节点
    ElemType x; //定义一个变量x，用于存储输入的元素值
    L = (LNode *)malloc(sizeof(LNode)); //为头节点分配内存空间
    r = L; //r指向头节点
    scanf("%d", &x);
    while(x != 9999) { //输入9999表示结束输入
        s = (LNode *)malloc(sizeof(LNode)); //为新节点分配内存空间
        s->data = x; //新节点的数据域赋值为输入的元素值
        r->next = s; //尾节点的指针域指向新节点
        r = s; //r指向新节点，成为新的尾节点
        scanf("%d", &x);
    }
    r->next = NULL; //尾节点的指针域指向空
    return L; //返回头节点指针
}

// ======4. 单链表按位查找元素======
LNode *GetElem(LinkList L, int i) {
    if(i < 0)
        return NULL; //位序i不合法，返回NULL
    LNode *p = L; //定义一个指针p，指向头节点
    int j=0; //定义一个计数器j，初始值为0
    while(p != NULL &&j < i) { //当p不为空且j小于i时，循环查找第i个节点
        p = p->next; //指针p指向下一个节点
        j++; //计数器j加1
    }
    return p; //返回指向第i个节点的指针p，如果未找到则返回NULL
}

// ======5. 单链表按值查找元素======
/*
LNode *LocateElem(LinkList L, ElemType e) { //不能返回位序，只能返回指针
    LNode *p = L; //定义一个指针p，指向头节点
    while(p != NULL && p->data != e) { //当p不为空且p指向的节点的数据域不等于e时，循环查找
        p = p->next; //指针p指向下一个节点
    }
    return p; //返回指向e的节点的指针p，如果未找到则返回NULL
}
*/
int LocateElem(LinkList L, ElemType e) { //返回位序
    LNode *p = L->next; //定义一个指针p，指向第一个节点
    int i = 1; //定义一个计数器i，初始值为1
    while(p != NULL && p->data != e) { //当p不为空且p指向的节点的数据域不等于e时，循环查找
        p = p->next; //指针p指向下一个节点
        i++; //计数器i加1
    }
    if(p == NULL)
        return -1; //如果未找到，则返回-1
    else
        return i; //返回e的位序i
}

// ======6. 插入：在第i个位置插入元素e======
bool ListInsert(LinkList &L, int i, ElemType e) {
    if(i<1)
        return false; //位序i不合法，返回false
    LNode *p = GetElem(L, i-1); //查找第i-1个节点
    if(p == NULL)
        return false; //第i-1个节点不存在，返回false
    LNode *s = (LNode *)malloc(sizeof(LNode)); //为新节点分配内存空间
    s->data = e; //新节点的数据域赋值为输入的元素值
    s->next = p->next; //新节点的指针域指向原来的第i个节点
    p->next = s; //第i-1个节点的指针域指向新节点
    return true; //返回true
}

// ======7. 删除：删除第i个位置的元素e======
bool ListDelete(LinkList &L, int i, ElemType &e) {
    if(i<1)
        return false; //位序i不合法，返回false
    LNode *p = GetElem(L, i-1); //查找第i-1个节点
    if(p == NULL || p->next == NULL)
        return false; //第i-1个节点不存在或第i个节点不存在，返回false
    LNode *q = p->next; //q指向第i个节点
    e = q->data; //将第i个节点的数据域赋值给e
    p->next = q->next; //第i-1个节点的指针域指向第i+1个节点
    free(q); //释放第i个节点的内存空间
    return true; //返回true
}

// ======8. 求单链表的长度======
int Length(LinkList L) {
    int len = 0; //定义一个计数器len，初始值为0
    LNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != NULL) { //当p不为空时，循环计数
        len++; //计数器len加1
        p = p->next; //指针p指向下一个节点
    }
    return len; //返回单链表的长度len
}

// ======9. 判断单链表是否为空======    
bool Empty(LinkList L) {
    return L->next == NULL; //如果头节点的指针域指向空，则单链表为空，返回true；否则返回false
}

// ======10. 打印单链表======
void PrintList(LinkList L) {
    LNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != NULL) { //当p不为空时，循环打印
        printf("%d ", p->data); //打印节点的数据域
        p = p->next; //指针p指向下一个节点
    }
    printf("\n"); //打印换行符
}

// ======11. 销毁单链表======
void DestroyList(LinkList &L) {
    LNode *p; //定义一个指针p，指向当前节点
    while(L != NULL) { //当L不为空时，循环销毁
        p = L; //定义一个指针p，指向当前节点
        L = L->next; //指针L指向下一个节点
        free(p); //释放当前节点的内存空间
    }
}

// ======主函数：测试所有函数操作======
int main() {
    LinkList L, L1, L2; //定义一个单链表L
    InitList(L); //初始化单链表L

    //头插法测试
    printf("请输入单链表的元素（以9999结束）：\n");
    List_HeadInsert(L1); //头插法创建单链表L
    printf("头插法创建的单链表为：");
    PrintList(L1); //打印单链表L1

    //尾插法测试
    printf("请输入单链表的元素（以9999结束）：\n"); 
    List_TailInsert(L2); //尾插法创建单链表L
    printf("尾插法创建的单链表为：");
    PrintList(L2); //打印单链表L2

    //依次插入
    for(int i = 1; i<=15; i++) {
        ListInsert(L, i, 100+i); //在第i个位置插入元素100+i
    }
    printf("初始单链表为：");
    PrintList(L); //打印单链表L

    //第a位插入元素j
    int a, j;
    printf("请输入要插入的位序和元素值（以空格分隔）：");
    scanf("%d %d", &a, &j); //输入要插入的位序和元素值
    while(a<1 || a>Length(L)+1) {
        printf("位序%d不合法\n", a); //如果位序a不合法，打印错误信息
        printf("请重新输入位序和元素值：");
        scanf("%d %d", &a, &j); //重新输入要插入的位序和元素值
    }
    ListInsert(L, a, j); //在第a个位置插入元素j
    printf("插入后的单链表为：");
    PrintList(L); //打印插入后的单链表L

    //第b位删除元素
    int b, e;
    printf("请输入要删除的位序：");
    scanf("%d", &b); //输入要删除的位序
    while(b<1 || b>Length(L)) {
        printf("位序%d不合法\n", b); //如果位序b不合法，打印错误信息
        printf("请重新输入位序：");
        scanf("%d", &b); //重新输入要删除的位序
    }
    ListDelete(L, b, e); //删除第b个位置的元素e
    printf("删除后的单链表为：");
    PrintList(L); //打印删除后的单链表L

    //按位查找元素
    int c;
    printf("请输入要查找的位序：");
    scanf("%d", &c); //输入要查找的位序
    while(c<1 || c>Length(L)) {
        printf("位序%d不合法\n", c); //如果位序c不合法，打印错误信息
        printf("请重新输入位序：");
        scanf("%d", &c); //重新输入要查找的位序
    }
    LNode *p = GetElem(L, c); //查找第c个节点
    if(p == NULL) {
        printf("第%d个节点不存在\n", c); //如果未找到则打印错误信息
    } else {
        printf("第%d个节点的数据域为：%d\n", c, p->data); //如果找到则打印节点的数据域
    }

    //按值查找元素
    int d;
    printf("请输入要查找的元素值：");
    scanf("%d", &d); //输入要查找的元素值
    int pos = LocateElem(L, d); //查找值为d的节点
    if(pos == -1) {
        printf("值为%d的节点不存在\n", d); //如果未找到则打印错误信息
    } else {
        printf("值为%d的节点的位序为：%d\n", d, pos); //如果找到则打印节点的位序
    }

    //打印单链表长度
    printf("单链表的长度为：%d\n", Length(L)); //打印单链表的长度
    //判断单链表是否为空
    if(Empty(L)) {
        printf("单链表为空\n"); //如果单链表为空，打印错误信息
    } else {
        printf("单链表不为空\n"); //如果单链表不为空，打印正确信息
    }

    //销毁单链表
    DestroyList(L); //销毁单链表L
    DestroyList(L1); //销毁单链表L1
    DestroyList(L2); //销毁单链表L2
    printf("单链表已销毁\n"); //打印销毁信息

    return 0; //返回0，表示程序正常结束
}