/*
 * 文件名：01-插入排序.cpp
 * 章节：第8章 排序
 * 内容：直接插入排序、折半插入排序、希尔排序
 * 重要度：★★★（直接插入必会代码，折半和希尔理解过程）
 * 考研考点：插入排序思想、复杂度、稳定性、希尔增量序列
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

// ======2. 直接插入排序（★★★ 能默写）======
// 思想：将A[i]插入到已排序区A[0..i-1]的合适位置
void InsertSort(ElemType A[], int n) {
    for(int i = 1; i < n; i++) {               // 从第2个元素开始，依次插入
        if(A[i] < A[i - 1]) {                   // ★ 只有当前元素比前一个小才需要插入
            ElemType temp = A[i];               // 暂存待插入元素
            int j = i - 1;
            while(j >= 0 && A[j] > temp) {      // 从后往前找插入位置
                A[j + 1] = A[j];                 // 元素后移
                j--;
            }
            A[j + 1] = temp;                     // ★ 插入到正确位置
        }
        // 若A[i] >= A[i-1]，说明已经在正确位置，不需要操作
    }
}

/*
 * ★ 直接插入排序复杂度：
 * 最好（已有序）：O(n)，每趟只需比较1次，不移动
 * 平均/最坏（逆序）：O(n^2)
 * 空间：O(1)
 * 稳定性：稳定（相等时不后移，保持原次序）
 * 特点：每趟结束后前缀A[0..i]有序，但元素不一定到最终位置
 */

// ======3. 折半插入排序（★★ 了解）======
// 思想：用折半查找定位插入位置，减少比较次数，但移动次数不变
void BinaryInsertSort(ElemType A[], int n) {
    for(int i = 1; i < n; i++) {
        ElemType temp = A[i];                   // 暂存待插入元素
        int low = 0, high = i - 1;
        while(low <= high) {                     // ★ 折半查找插入位置
            int mid = (low + high) / 2;
            if(A[mid] > temp)
                high = mid - 1;                  // 插入位置在左半
            else
                low = mid + 1;                   // 插入位置在右半（相等时往右，保持稳定）
        }
        // 循环结束时 low = high + 1，插入位置为 low（即 high+1）
        for(int j = i - 1; j >= low; j--)        // 元素后移
            A[j + 1] = A[j];
        A[low] = temp;                            // ★ 插入
    }
}

/*
 * ★ 折半插入排序特点：
 * 比较次数降为 O(n log n)，但移动次数仍为 O(n^2)
 * 总时间仍为 O(n^2)
 * 稳定（相等时插到已有相等元素之后）
 * 适合比较操作昂贵、移动操作便宜的场景
 */

// ======4. 希尔排序（★★ 理解过程）======
// 思想：按递减增量分组，每组内直接插入排序，最后增量为1
void ShellSort(ElemType A[], int n) {
    // 增量序列：n/2, n/4, ..., 1（常用的Shell增量）
    for(int gap = n / 2; gap >= 1; gap /= 2) {   // ★ 增量递减
        for(int i = gap; i < n; i++) {            // 对每个分组做插入排序
            ElemType temp = A[i];
            int j = i - gap;
            while(j >= 0 && A[j] > temp) {        // 组内从后往前找
                A[j + gap] = A[j];                 // 元素后移（步长为gap）
                j -= gap;
            }
            A[j + gap] = temp;                     // 插入
        }
    }
}

/*
 * ★ 希尔排序特点：
 * 性能依赖增量序列，最后一个增量必须为1
 * 空间 O(1)，不稳定（分组插入可能改变相等元素相对次序）
 * 适合中等规模数据
 * 统考通常不给统一精确平均界，最坏常记 O(n^2)
 */

// ======5. 主函数测试======
int main() {
    printf("====== 插入排序测试开始 ======\n\n");

    // 【测试1】直接插入排序
    printf("【测试1】直接插入排序\n");
    ElemType arr1[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n1 = 8;
    printf("       原数组：");
    PrintArray(arr1, n1);
    InsertSort(arr1, n1);
    printf("       排序后：");
    PrintArray(arr1, n1);
    printf("       期望：13 27 38 49 49 65 76 97\n");

    // 【测试2】直接插入排序（已有序，最好情况）
    printf("\n【测试2】直接插入排序（已有序，最好情况O(n)）\n");
    ElemType arr2[] = {1, 2, 3, 4, 5};
    int n2 = 5;
    printf("       原数组：");
    PrintArray(arr2, n2);
    InsertSort(arr2, n2);
    printf("       排序后：");
    PrintArray(arr2, n2);
    printf("       已有序时每趟只需比较1次，不移动\n");

    // 【测试3】直接插入排序（逆序，最坏情况）
    printf("\n【测试3】直接插入排序（逆序，最坏情况O(n^2)）\n");
    ElemType arr3[] = {5, 4, 3, 2, 1};
    int n3 = 5;
    printf("       原数组：");
    PrintArray(arr3, n3);
    InsertSort(arr3, n3);
    printf("       排序后：");
    PrintArray(arr3, n3);
    printf("       逆序时每趟比较和移动都最多\n");

    // 【测试4】折半插入排序
    printf("\n【测试4】折半插入排序\n");
    ElemType arr4[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n4 = 8;
    printf("       原数组：");
    PrintArray(arr4, n4);
    BinaryInsertSort(arr4, n4);
    printf("       排序后：");
    PrintArray(arr4, n4);
    printf("       比较次数减少，但移动次数不变\n");

    // 【测试5】希尔排序
    printf("\n【测试5】希尔排序（增量序列：4, 2, 1）\n");
    ElemType arr5[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n5 = 8;
    printf("       原数组：");
    PrintArray(arr5, n5);
    ShellSort(arr5, n5);
    printf("       排序后：");
    PrintArray(arr5, n5);
    printf("       增量gap=4：分组[49,76],[38,13],[65,27],[97,49]，组内插入\n");
    printf("       增量gap=2：分组[49,65,76,27],[38,97,13,49]，组内插入\n");
    printf("       增量gap=1：整体直接插入排序\n");

    // 【测试6】三种插入排序对比
    printf("\n【测试6】三种插入排序对比\n");
    printf("       | 算法     | 最好   | 平均   | 最坏   | 空间 | 稳定 |\n");
    printf("       |----------|--------|--------|--------|------|------|\n");
    printf("       | 直接插入 | O(n)   | O(n^2) | O(n^2) | O(1) | 是   |\n");
    printf("       | 折半插入 | O(nlogn)比较 | O(n^2) | O(n^2) | O(1) | 是   |\n");
    printf("       | 希尔     | 依增量 | 依增量 | O(n^2) | O(1) | 否   |\n");

    // 【测试7】稳定性验证
    printf("\n【测试7】稳定性验证（两个49的相对次序）\n");
    ElemType arr7[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n7 = 8;
    printf("       原数组中两个49：第1个在位置0，第2个在位置7\n");
    InsertSort(arr7, n7);
    printf("       直接插入排序后：");
    PrintArray(arr7, n7);
    printf("       两个49的相对次序保持不变（稳定）\n");

    printf("\n====== 插入排序测试结束 ======\n");
    return 0;
}
