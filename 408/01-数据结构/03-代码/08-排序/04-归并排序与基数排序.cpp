/*
 * 文件名：04-归并排序与基数排序.cpp
 * 章节：第8章 排序
 * 内容：二路归并排序、基数排序（LSD最低位优先）
 * 重要度：★★★★（归并合并过程必会，基数排序理解分配收集）
 * 考研考点：归并合并过程、复杂度、稳定性；基数排序LSD过程、复杂度
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

// ======2. 归并函数（★★★★ 能默写）======
// 合并 A[left..mid] 和 A[mid+1..right]（两段都已有序）
void Merge(ElemType A[], int left, int mid, int right, ElemType temp[]) {
    int i = left;                                  // 左段指针
    int j = mid + 1;                               // 右段指针
    int k = left;                                  // temp指针
    while(i <= mid && j <= right) {
        if(A[i] <= A[j])                           // ★ 相等时取左边，保持稳定
            temp[k++] = A[i++];
        else
            temp[k++] = A[j++];
    }
    while(i <= mid)                                // 复制左段剩余
        temp[k++] = A[i++];
    while(j <= right)                              // 复制右段剩余
        temp[k++] = A[j++];
    for(int t = left; t <= right; t++)             // ★ 写回原数组
        A[t] = temp[t];
}

/*
 * ★ 归并函数要点：
 * 1. 前提：A[left..mid] 和 A[mid+1..right] 都已非降序
 * 2. 用辅助数组temp暂存合并结果
 * 3. 相等时取左段元素（A[i] <= A[j]），保持稳定性
 * 4. 合并时间 O(right-left+1)
 * 5. 最后必须写回原数组
 */

// ======3. 归并排序（★★★★ 理解）======
void MergeSort(ElemType A[], int left, int right, ElemType temp[]) {
    if(left < right) {
        int mid = left + (right - left) / 2;       // 二分
        MergeSort(A, left, mid, temp);              // 递归排序左半
        MergeSort(A, mid + 1, right, temp);         // 递归排序右半
        Merge(A, left, mid, right, temp);           // 合并
    }
}

// 封装
void MergeSortWrapper(ElemType A[], int n) {
    ElemType *temp = (ElemType *)malloc(sizeof(ElemType) * n);
    if(temp == NULL) return;
    MergeSort(A, 0, n - 1, temp);
    free(temp);
}

/*
 * ★ 归并排序复杂度：
 * 时间：最好/平均/最坏均 O(n log n)（与初始序列无关）
 * 空间：O(n)（辅助数组）+ O(log n)（递归栈）
 * 稳定性：稳定（相等时取左段）
 * 特点：每趟合并后形成更长的有序段，但元素不一定到最终位置
 * 是外部排序的基础（多路归并）
 */

// ======4. 带过程演示的归并（用于教学）======
void MergeWithTrace(ElemType A[], int left, int mid, int right, ElemType temp[]) {
    printf("       合并 A[%d..%d]={", left, mid);
    for(int i = left; i <= mid; i++) printf("%d ", A[i]);
    printf("} 和 A[%d..%d]={", mid+1, right);
    for(int i = mid+1; i <= right; i++) printf("%d ", A[i]);
    printf("} → ");
    Merge(A, left, mid, right, temp);
    for(int i = left; i <= right; i++) printf("%d ", A[i]);
    printf("\n");
}

void MergeSortWithTrace(ElemType A[], int left, int right, ElemType temp[]) {
    if(left < right) {
        int mid = left + (right - left) / 2;
        MergeSortWithTrace(A, left, mid, temp);
        MergeSortWithTrace(A, mid + 1, right, temp);
        MergeWithTrace(A, left, mid, right, temp);
    }
}

// ======5. 基数排序（LSD最低位优先，★★ 理解过程）======
// 对非负整数，按十进制位从低到高进行分配和收集
void RadixSortLSD(ElemType A[], int n, int digits) {
    ElemType *output = (ElemType *)malloc(sizeof(ElemType) * n);
    int *count = (int *)malloc(sizeof(int) * 10);   // 基数r=10，10个桶
    if(output == NULL || count == NULL) {
        free(output); free(count);
        return;
    }

    int factor = 1;                                 // 当前位的权值（1, 10, 100, ...）
    for(int pass = 1; pass <= digits; pass++) {     // ★ 共digits趟
        // 第一步：统计每个桶的元素个数
        for(int i = 0; i < 10; i++) count[i] = 0;
        for(int i = 0; i < n; i++) {
            int digit = (A[i] / factor) % 10;       // 取当前位数字
            count[digit]++;
        }
        // 第二步：求前缀和（确定每个桶在output中的结束位置）
        for(int i = 1; i < 10; i++)
            count[i] += count[i - 1];
        // 第三步：从后往前放入output（保持稳定）
        for(int i = n - 1; i >= 0; i--) {
            int digit = (A[i] / factor) % 10;
            output[count[digit] - 1] = A[i];
            count[digit]--;
        }
        // 第四步：写回原数组
        for(int i = 0; i < n; i++)
            A[i] = output[i];

        printf("       第%d趟（%s位）：", pass,
               pass == 1 ? "个" : pass == 2 ? "十" : pass == 3 ? "百" : "更高");
        PrintArray(A, n);

        factor *= 10;
    }
    free(output);
    free(count);
}

/*
 * ★ 基数排序要点：
 * 1. LSD（最低位优先）：从个位开始，逐趟按位分配收集
 * 2. 每趟的分配和收集必须是稳定的（从后往前放入）
 * 3. 时间 O(d(n+r))，d=位数，r=基数（十进制r=10）
 * 4. 空间 O(n+r)
 * 5. 稳定
 * 6. 不是基于比较的排序，不受 n log n 下界限制
 * 7. 适合位数有限的整数或定长字符串
 */

// ======6. 主函数测试======
int main() {
    printf("====== 归并排序与基数排序测试开始 ======\n\n");

    // 【测试1】归并排序
    printf("【测试1】归并排序\n");
    ElemType arr1[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n1 = 8;
    printf("       原数组：");
    PrintArray(arr1, n1);
    MergeSortWrapper(arr1, n1);
    printf("       排序后：");
    PrintArray(arr1, n1);
    printf("       期望：13 27 38 49 49 65 76 97\n");

    // 【测试2】归并排序过程演示（★ 手算必会）
    printf("\n【测试2】归并排序过程演示\n");
    ElemType arr2[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n2 = 8;
    ElemType *temp2 = (ElemType *)malloc(sizeof(ElemType) * n2);
    printf("       原数组：");
    PrintArray(arr2, n2);
    MergeSortWithTrace(arr2, 0, n2 - 1, temp2);
    printf("       最终结果：");
    PrintArray(arr2, n2);
    free(temp2);

    // 【测试3】归并排序复杂度
    printf("\n【测试3】归并排序复杂度\n");
    printf("       时间：O(n log n)（最好/平均/最坏均如此，与初始序列无关）\n");
    printf("       空间：O(n)（辅助数组）+ O(log n)（递归栈）\n");
    printf("       稳定性：稳定（相等时取左段元素）\n");
    printf("       特点：每趟形成更长有序段，元素不一定到最终位置\n");
    printf("       是外部排序多路归并的基础\n");

    // 【测试4】基数排序（LSD）
    printf("\n【测试4】基数排序（LSD最低位优先，3位数）\n");
    ElemType arr4[] = {179, 208, 306, 93, 859, 984, 55, 9, 271, 33};
    int n4 = 10;
    printf("       原数组：");
    PrintArray(arr4, n4);
    RadixSortLSD(arr4, n4, 3);
    printf("       排序后：");
    PrintArray(arr4, n4);
    printf("       期望：9 33 55 93 179 208 271 306 859 984\n");

    // 【测试5】基数排序过程说明
    printf("\n【测试5】基数排序LSD过程说明\n");
    printf("       第1趟（个位）：按个位0~9分配到桶，再按桶顺序收集\n");
    printf("       第2趟（十位）：按十位0~9分配到桶，再按桶顺序收集\n");
    printf("       第3趟（百位）：按百位0~9分配到桶，再按桶顺序收集\n");
    printf("       每趟分配收集必须稳定（从后往前放入output）\n");
    printf("       时间 O(d(n+r)) = O(3*(10+10)) = O(60)，d=3, r=10\n");
    printf("       空间 O(n+r) = O(10+10) = O(20)\n");

    // 【测试6】归并 vs 基数对比
    printf("\n【测试6】归并排序 vs 基数排序\n");
    printf("       | 指标     | 归并排序       | 基数排序         |\n");
    printf("       |----------|----------------|------------------|\n");
    printf("       | 时间     | O(n log n)     | O(d(n+r))        |\n");
    printf("       | 空间     | O(n)           | O(n+r)           |\n");
    printf("       | 稳定性   | 稳定           | 稳定             |\n");
    printf("       | 基于比较 | 是             | 否（非比较排序） |\n");
    printf("       | 适用     | 通用、链表、外排 | 位数有限的整数   |\n");

    // 【测试7】稳定性验证
    printf("\n【测试7】归并排序稳定性验证（两个49）\n");
    ElemType arr7[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n7 = 8;
    printf("       原数组：第一个49在位置0，第二个49在位置7\n");
    MergeSortWrapper(arr7, n7);
    printf("       归并排序后：");
    PrintArray(arr7, n7);
    printf("       两个49相对次序保持不变（稳定）\n");

    printf("\n====== 归并排序与基数排序测试结束 ======\n");
    return 0;
}
