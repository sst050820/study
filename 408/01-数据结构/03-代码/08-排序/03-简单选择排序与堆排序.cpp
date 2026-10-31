/*
 * 文件名：03-简单选择排序与堆排序.cpp
 * 章节：第8章 排序
 * 内容：简单选择排序、堆排序（建堆+向下调整+排序）
 * 重要度：★★★★（堆排序手算建堆+调整必会，代码理解）
 * 考研考点：简单选择复杂度、堆的定义、建堆O(n)、向下调整、堆排序过程
 */

#include <stdio.h>
#include <stdlib.h>

typedef int ElemType; // 元素类型

// ======1. 打印数组（辅助函数）======
void PrintArray(ElemType A[], int n) {
    for(int i = 0; i < n; i++)
        printf("%d ", A[i]);
    printf("\n");
}

// ======2. 简单选择排序（★★ 理解）======
// 思想：每趟从未排序区选最小值，与未排序区第一个元素交换
void SelectSort(ElemType A[], int n) {
    for(int i = 0; i < n - 1; i++) {
        int minPos = i;                            // 假设第一个是最小
        for(int j = i + 1; j < n; j++)             // 在未排序区找最小值
            if(A[j] < A[minPos])
                minPos = j;
        if(minPos != i) {                          // 最小值不在首位才交换
            ElemType temp = A[i];
            A[i] = A[minPos];
            A[minPos] = temp;
        }
    }
}

/*
 * ★ 简单选择排序特点：
 * 比较次数恒为 n(n-1)/2（与初始序列无关）
 * 时间 O(n^2)，空间 O(1)，不稳定（交换可能改变相等元素次序）
 * 每趟结束后，前缀的最小值到达最终位置
 * 适合移动代价高、比较便宜的场景
 */

// ======3. 大根堆向下调整（★★★★ 能默写）======
// 将以root为根的子树调整为大根堆，end是堆的最后一个元素下标
void SiftDown(ElemType A[], int root, int end) {
    ElemType temp = A[root];                       // 暂存根节点
    int child = 2 * root + 1;                      // ★ 左孩子（下标从0开始）
    while(child <= end) {
        // ★ 选较大的孩子
        if(child + 1 <= end && A[child] < A[child + 1])
            child++;
        if(temp >= A[child])                       // 根已最大，停止
            break;
        A[root] = A[child];                        // 较大孩子上移
        root = child;                              // 继续向下调整
        child = 2 * root + 1;
    }
    A[root] = temp;                                // 原根放到最终位置
}

/*
 * ★ 向下调整要点：
 * 1. 数组下标从0开始，左孩子=2*i+1，右孩子=2*i+2，父节点=(i-1)/2
 * 2. 最后一个非叶节点 = n/2 - 1
 * 3. 每次选较大的孩子比较，大根堆要求根>=孩子
 * 4. 调整时间O(log n)（树高）
 */

// ======4. 建大根堆（★★★★ 手算必会）======
void BuildMaxHeap(ElemType A[], int n) {
    for(int i = n / 2 - 1; i >= 0; i--)            // ★ 从最后一个非叶节点向前
        SiftDown(A, i, n - 1);
}

/*
 * ★ 建堆要点：
 * 1. 从最后一个非叶节点 n/2-1 开始，向前逐个做向下调整
 * 2. 建堆时间 O(n)（不是O(n log n)！高频考点）
 * 3. 建堆完成后，A[0]是最大值
 */

// ======5. 堆排序（★★★★ 理解+会手算）======
// 升序排序：建大根堆，每趟堆顶与末尾交换，缩小堆调整
void HeapSort(ElemType A[], int n) {
    BuildMaxHeap(A, n);                            // 1. 建大根堆
    for(int i = n - 1; i > 0; i--) {
        // 2. 堆顶（最大值）与末尾交换
        ElemType temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        // 3. 缩小堆范围（0..i-1），调整堆顶
        SiftDown(A, 0, i - 1);
    }
}

/*
 * ★ 堆排序复杂度：
 * 建堆 O(n)，排序 O(n log n)
 * 总时间最好/平均/最坏均 O(n log n)
 * 空间 O(1)
 * 不稳定
 * 每趟结束后，当前堆顶（最大值）到达最终位置（末尾）
 * 升序建大根堆，降序建小根堆（别搞反）
 */

// ======6. 带过程演示的建堆（用于教学）======
void BuildMaxHeapWithTrace(ElemType A[], int n) {
    printf("       初始数组（完全二叉树层序）：");
    PrintArray(A, n);
    printf("       最后一个非叶节点下标 = %d（值=%d）\n", n/2-1, A[n/2-1]);
    for(int i = n / 2 - 1; i >= 0; i--) {
        printf("       调整节点%d（值=%d）：", i, A[i]);
        SiftDown(A, i, n - 1);
        PrintArray(A, n);
    }
    printf("       建堆完成，堆顶（最大值）=%d\n", A[0]);
}

// ======7. 主函数测试======
int main() {
    printf("====== 简单选择排序与堆排序测试开始 ======\n\n");

    // 【测试1】简单选择排序
    printf("【测试1】简单选择排序\n");
    ElemType arr1[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n1 = 8;
    printf("       原数组：");
    PrintArray(arr1, n1);
    SelectSort(arr1, n1);
    printf("       排序后：");
    PrintArray(arr1, n1);
    printf("       期望：13 27 38 49 49 65 76 97\n");
    printf("       比较次数恒为 n(n-1)/2 = %d（与初始序列无关）\n", n1*(n1-1)/2);

    // 【测试2】建大根堆过程演示（★ 手算必会）
    printf("\n【测试2】建大根堆过程演示\n");
    ElemType arr2[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n2 = 8;
    BuildMaxHeapWithTrace(arr2, n2);
    printf("       建堆时间 O(n)，不是 O(n log n)！\n");

    // 【测试3】堆排序
    printf("\n【测试3】堆排序（升序，建大根堆）\n");
    ElemType arr3[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n3 = 8;
    printf("       原数组：");
    PrintArray(arr3, n3);
    HeapSort(arr3, n3);
    printf("       排序后：");
    PrintArray(arr3, n3);
    printf("       期望：13 27 38 49 49 65 76 97\n");

    // 【测试4】堆排序每趟过程
    printf("\n【测试4】堆排序每趟过程（建堆后逐趟交换+调整）\n");
    ElemType arr4[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n4 = 8;
    BuildMaxHeap(arr4, n4);
    printf("       建堆后：");
    PrintArray(arr4, n4);
    for(int i = n4 - 1; i > 0; i--) {
        ElemType temp = arr4[0]; arr4[0] = arr4[i]; arr4[i] = temp;
        printf("       第%d趟交换后（堆顶%d与末尾交换）：", n4-i, arr4[i]);
        PrintArray(arr4, n4);
        SiftDown(arr4, 0, i - 1);
        printf("       调整后（堆范围0~%d）：", i-1);
        PrintArray(arr4, n4);
    }

    // 【测试5】数组下标关系
    printf("\n【测试5】堆的数组下标关系（从0开始）\n");
    printf("       节点i的左孩子 = 2*i + 1\n");
    printf("       节点i的右孩子 = 2*i + 2\n");
    printf("       节点i的父节点 = (i-1) / 2\n");
    printf("       最后一个非叶节点 = n/2 - 1\n");
    printf("       n=8时，最后一个非叶节点 = 8/2-1 = 3（值=%d）\n", arr4[3]);

    // 【测试6】两种选择排序对比
    printf("\n【测试6】简单选择 vs 堆排序\n");
    printf("       | 指标     | 简单选择 | 堆排序           |\n");
    printf("       |----------|----------|------------------|\n");
    printf("       | 时间     | O(n^2)   | O(n log n)       |\n");
    printf("       | 建堆     | 无       | O(n)             |\n");
    printf("       | 空间     | O(1)     | O(1)             |\n");
    printf("       | 稳定性   | 不稳定   | 不稳定           |\n");
    printf("       | 每趟特征 | 最小值归位 | 堆顶（最大值）归位 |\n");

    // 【测试7】大根堆 vs 小根堆
    printf("\n【测试7】大根堆 vs 小根堆（★ 别搞反）\n");
    printf("       升序排序 → 建大根堆（每次取最大值放末尾）\n");
    printf("       降序排序 → 建小根堆（每次取最小值放末尾）\n");
    printf("       大根堆：每个节点 >= 孩子，根是最大值\n");
    printf("       小根堆：每个节点 <= 孩子，根是最小值\n");

    printf("\n====== 简单选择排序与堆排序测试结束 ======\n");
    return 0;
}
