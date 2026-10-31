#include <stdio.h>
#include <stdlib.h>
typedef int ElemType; //循环单链表的元素类型

// ======循环单链表结构体定义======
typedef struct LNode {
    ElemType data; //数据域：存储元素的值
    struct LNode *next; //指针域：指向下一个节点（尾节点指向头节点，形成循环）
} LNode, *LinkList; //循环单链表的结构体类型
// LNode是节点类型；LinkList 是指向节点的指针类型（即"循环单链表"类型）
// ★ 注意：循环单链表的节点结构和单链表完全相同，只是逻辑上"首尾相连"

// ======1. 循环单链表初始化（有头节点）======
bool InitList(LinkList &L) {
    L = (LNode *)malloc(sizeof(LNode)); //为头节点分配内存空间
    if(L == NULL)
        return false; //内存分配失败，返回false
    L->next = L; //★ 和单链表不同！头节点的指针域指向自己（空表时自己指向自己）
    return true;
}

// ======2. 头插法 创建循环单链表（逆序输入）======
LinkList List_HeadInsert(LinkList &L) {
    LNode *s; //定义一个指针s，用于创建新节点
    ElemType x; //定义一个变量x，用于存储输入的元素值
    L = (LNode *)malloc(sizeof(LNode)); //为头节点分配内存空间
    L->next = L; //头节点的指针域指向自己（空表）
    scanf("%d", &x);
    while(x != 9999) { //输入9999表示结束输入
        s = (LNode *)malloc(sizeof(LNode)); //为新节点分配内存空间
        s->data = x; //新节点的数据域赋值为输入的元素值
        s->next = L->next; //新节点的指针域指向原第一个节点
        L->next = s; //头节点的指针域指向新节点
        scanf("%d", &x);
    }
    return L; //返回头节点指针
}

// ======3. 尾插法 创建循环单链表（★ 考研重点：最后要 r->next = L 形成循环）======
LinkList List_TailInsert(LinkList &L) {
    LNode *s, *r; //定义两个指针s和r，s用于创建新节点，r用于指向尾节点
    ElemType x; //定义一个变量x，用于存储输入的元素值
    L = (LNode *)malloc(sizeof(LNode)); //为头节点分配内存空间
    r = L; //r指向头节点（初始时尾节点就是头节点）
    scanf("%d", &x);
    while(x != 9999) { //输入9999表示结束输入
        s = (LNode *)malloc(sizeof(LNode)); //为新节点分配内存空间
        s->data = x; //新节点的数据域赋值为输入的元素值
        r->next = s; //尾节点的指针域指向新节点
        r = s; //r指向新节点，成为新的尾节点
        scanf("%d", &x);
    }
    r->next = L; //★ 和单链表不同！尾节点的指针域指向头节点，形成循环
    return L; //返回头节点指针
}

// ======4. 循环单链表按位查找元素======
LNode *GetElem(LinkList L, int i) {
    if(i < 0)
        return NULL; //位序i不合法，返回NULL
    LNode *p = L; //定义一个指针p，指向头节点
    int j = 0; //定义一个计数器j，初始值为0
    while(j < i && p->next != L) { //当还没找到第i个节点且还没转完一圈时，继续查找
        p = p->next; //指针p指向下一个节点
        j++; //计数器j加1
    }
    return p; //返回指向第i个节点的指针p（如果i超过表长，返回尾节点）
}

// ======5. 循环单链表按值查找元素（考研标准版：返回节点指针）======
LNode *LocateElem(LinkList L, ElemType e) {
    LNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != L && p->data != e) { //★ 和单链表不同！循环结束条件是 p != L（转了一圈回到头节点）
        p = p->next; //指针p指向下一个节点
    }
    if(p == L)
        return NULL; //如果转了一圈都没找到（p回到头节点），返回NULL
    return p; //返回指向e的节点的指针p
}

// ======5-补. 循环单链表按值查找（返回位序，辅助用）======
int LocateElemPos(LinkList L, ElemType e) {
    LNode *p = L->next; //定义一个指针p，指向第一个节点
    int i = 1; //定义一个计数器i，初始值为1
    while(p != L && p->data != e) { //当p不为空且p指向的节点的数据域不等于e时，循环查找
        p = p->next; //指针p指向下一个节点
        i++; //计数器i加1
    }
    if(p == L)
        return 0; //如果未找到，则返回0
    else
        return i; //返回e的位序i
}

// ======6. 插入：在第i个位置插入元素e======
bool ListInsert(LinkList &L, int i, ElemType e) {
    if(i < 1)
        return false; //位序i不合法，返回false
    LNode *p = GetElem(L, i - 1); //查找第i-1个节点（前驱节点）
    LNode *s = (LNode *)malloc(sizeof(LNode)); //为新节点分配内存空间
    s->data = e; //新节点的数据域赋值为e
    s->next = p->next; //① 新节点的指针域指向原第i个节点
    p->next = s; //② 前驱节点的指针域指向新节点
    return true; //返回true
    // ★ 注意：在表尾插入时，新节点成为尾节点，它的next自动指向头节点（因为p->next是头节点），循环依然成立，不需要特判
}

// ======7. 删除：删除第i个位置的元素e======
bool ListDelete(LinkList &L, int i, ElemType &e) {
    if(i < 1)
        return false; //位序i不合法，返回false
    LNode *p = GetElem(L, i - 1); //查找第i-1个节点（前驱节点）
    if(p->next == L)
        return false; //前驱的next指向头节点，说明第i个节点不存在（i超过表长），返回false
    LNode *q = p->next; //q指向第i个节点（被删节点）
    e = q->data; //将第i个节点的数据域赋值给e
    p->next = q->next; //前驱节点的指针域指向第i+1个节点
    free(q); //释放第i个节点的内存空间
    return true; //返回true
}

// ======8. 求循环单链表的长度======
int Length(LinkList L) {
    int len = 0; //定义一个计数器len，初始值为0
    LNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != L) { //★ 当p没有转回头节点时，循环计数
        len++; //计数器len加1
        p = p->next; //指针p指向下一个节点
    }
    return len; //返回循环单链表的长度len
}

// ======9. 判断循环单链表是否为空======
bool Empty(LinkList L) {
    return L->next == L; //★ 和单链表不同！头节点的指针域指向自己（L->next == L）才是空表
}

// ======10. 打印循环单链表======
void PrintList(LinkList L) {
    LNode *p = L->next; //定义一个指针p，指向第一个节点
    while(p != L) { //★ 当p没有转回头节点时，循环打印
        printf("%d ", p->data); //打印节点的数据域
        p = p->next; //指针p指向下一个节点
    }
    printf("\n"); //打印换行符
}

// ======11. 销毁循环单链表（★ 和单链表不同，要防止死循环）======
void DestroyList(LinkList &L) {
    LNode *p; //定义一个指针p，指向当前要释放的节点
    while(L->next != L) { //★ 当链表中还有数据节点时（头节点还没自己指向自己）
        p = L->next; //p指向第一个数据节点
        L->next = p->next; //头节点指向第二个数据节点
        free(p); //释放第一个数据节点
    }
    free(L); //最后释放头节点
    L = NULL; //头指针置空，防止野指针
}

// ======主函数：测试所有函数操作======
int main() {
    LinkList L, L1, L2; //定义循环单链表L、L1、L2
    InitList(L); //初始化循环单链表L

    //头插法测试
    printf("请输入循环单链表的元素（以9999结束）：\n");
    List_HeadInsert(L1); //头插法创建循环单链表L1
    printf("头插法创建的循环单链表为：");
    PrintList(L1); //打印循环单链表L1
    printf("判空测试（头插法创建的L1）：%s\n", Empty(L1) ? "为空" : "不为空");

    //尾插法测试
    printf("请输入循环单链表的元素（以9999结束）：\n");
    List_TailInsert(L2); //尾插法创建循环单链表L2
    printf("尾插法创建的循环单链表为：");
    PrintList(L2); //打印循环单链表L2
    printf("尾插法创建的L2的长度为：%d\n", Length(L2)); //打印长度

    //依次插入
    for(int i = 1; i <= 15; i++) {
        ListInsert(L, i, 100 + i); //在第i个位置插入元素100+i
    }
    printf("初始循环单链表为：");
    PrintList(L); //打印循环单链表L

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
    printf("插入后的循环单链表为：");
    PrintList(L); //打印插入后的循环单链表L

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
    printf("删除后的循环单链表为：");
    PrintList(L); //打印删除后的循环单链表L

    //按位查找元素
    int c;
    printf("请输入要查找的位序：");
    scanf("%d", &c); //输入要查找的位序
    if(c < 1 || c > Length(L)) {
        printf("位序%d不合法\n", c); //如果位序c不合法，打印错误信息
        printf("请重新输入位序：");
        scanf("%d", &c); //重新输入要查找的位序
    }
    LNode *p = GetElem(L, c); //查找第c个节点
    if(p == NULL || p->next == L && c > Length(L)) {
        printf("第%d个节点不存在\n", c); //如果未找到则打印错误信息
    } else {
        printf("第%d个节点的数据域为：%d\n", c, p->data); //如果找到则打印节点的数据域
    }

    //按值查找元素
    int d;
    printf("请输入要查找的元素值：");
    scanf("%d", &d); //输入要查找的元素值
    LNode *q = LocateElem(L, d); //查找值为d的节点
    if(q == NULL) {
        printf("值为%d的节点不存在\n", d); //如果未找到则打印错误信息
    } else {
        printf("值为%d的节点存在，数据域为：%d，位序为：%d\n", d, q->data, LocateElemPos(L, d)); //如果找到则打印节点信息
    }

    //打印循环单链表长度
    printf("循环单链表的长度为：%d\n", Length(L)); //打印循环单链表的长度
    //判断循环单链表是否为空
    if(Empty(L)) {
        printf("循环单链表为空\n"); //如果循环单链表为空，打印信息
    } else {
        printf("循环单链表不为空\n"); //如果循环单链表不为空，打印信息
    }

    //销毁循环单链表
    DestroyList(L); //销毁循环单链表L
    DestroyList(L1); //销毁循环单链表L1
    DestroyList(L2); //销毁循环单链表L2
    printf("循环单链表已销毁\n"); //打印销毁信息
    return 0; //返回0，表示程序正常结束
}
