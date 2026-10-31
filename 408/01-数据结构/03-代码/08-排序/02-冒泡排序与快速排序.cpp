/*
 * 文件名：02-冒泡排序与快速排序.cpp
 * 章节：第8章 排序
 * 内容：冒泡排序、快速排序（划分+递归）
 * 重要度：★★★★★（快速排序是全章最高频，必会代码+手算划分）
 * 考研考点：冒泡提前结束标志、快速排序划分过程、复杂度、稳定性
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

// ======2. 冒泡排序（★★★ 理解+会算）======
// 思想：两两比较相邻元素，逆序则交换，每趟把最大值送到末尾
void BubbleSort(ElemType A[], int n) {
    for(int i = 0; i < n - 1; i++) {
        bool changed = false;                  // ★ 提前结束标志
        for(int j = 0; j < n - 1 - i; j++) {   // 每趟比较到未排序区末尾
            if(A[j] > A[j + 1]) {               // 相邻逆序则交换
                ElemType temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
                changed = true;
            }
        }
        if(!changed) break;                     // ★ 本趟无交换，已有序，提前结束
    }
}

/*
 * ★ 冒泡排序复杂度：
 * 最好（已有序+提前结束标志）：O(n)，第一趟无交换直接结束
 * 平均/最坏：O(n^2)
 * 空间：O(1)
 * 稳定性：稳定（相邻比较，相等不交换）
 * 特点：每趟结束后，末尾的极值到达最终位置
 * 注意：没有提前结束标志时，最好情况也是O(n^2)
 */

// ======3. 快速排序划分函数（★★★★★ 能默写，挖坑法）======
// 选第一个元素为枢轴，返回枢轴的最终位置
int Partition(ElemType A[], int low, int high) {
    ElemType pivot = A[low];                    // ★ 选第一个元素为枢轴，挖坑
    while(low < high) {
        // 从右往左找 < pivot 的元素
        while(low < high && A[high] >= pivot)
            high--;
        if(low < high)
            A[low++] = A[high];                 // 填坑，low右移
        // 从左往右找 > pivot 的元素
        while(low < high && A[low] <= pivot)
            low++;
        if(low < high)
            A[high--] = A[low];                 // 填坑，high左移
    }
    A[low] = pivot;                             // ★ 枢轴归位
    return low;                                 // 返回枢轴最终位置
}

/*
 * ★ 划分函数要点：
 * 1. 枢轴选第一个元素A[low]，暂存后该位置成为"坑"
 * 2. 从右往左找小于枢轴的，填到左边的坑
 * 3. 从左往右找大于枢轴的，填到右边的坑
 * 4. 循环结束时low=high，枢轴归位
 * 5. 相等元素的处理：A[high]>=pivot和A[low]<=pivot，相等时不停止
 * 6. 划分时间O(high-low+1)，空间O(1)
 */

// ======4. 快速排序（★★★★★ 能默写）======
void QuickSort(ElemType A[], int low, int high) {
    if(low < high) {
        int p = Partition(A, low, high);        // 划分，枢轴归位
        QuickSort(A, low, p - 1);               // 递归排序左半
        QuickSort(A, p + 1, high);              // 递归排序右半
    }
}

// 封装：从0到n-1排序
void QuickSortWrapper(ElemType A[], int n) {
    QuickSort(A, 0, n - 1);
}

/*
 * ★ 快速排序复杂度：
 * 最好/平均：O(n log n)，划分均衡
 * 最坏：O(n^2)，枢轴极不均衡（如已有序且固定取端点）
 * 递归栈空间：平均O(log n)，最坏O(n)
 * 稳定性：不稳定（划分可能改变相等元素相对次序）
 * 特点：每趟划分后枢轴到达最终位置
 * 改善最坏情况：随机枢轴、三数取中、三向划分
 */

// ======5. 带过程演示的划分（用于教学展示）======
int PartitionWithTrace(ElemType A[], int low, int high) {
    ElemType pivot = A[low];
    printf("       枢轴=%d，初始：", pivot);
    // 打印当前区间
    for(int i = low; i <= high; i++) printf("%d ", A[i]);
    printf("\n");
    int origLow = low, origHigh = high;
    while(low < high) {
        while(low < high && A[high] >= pivot) high--;
        if(low < high) {
            A[low++] = A[high];
            printf("       从右找%d，放到左端：", A[low-1]);
            for(int i = origLow; i <= origHigh; i++) printf("%d ", A[i]);
            printf("\n");
        }
        while(low < high && A[low] <= pivot) low++;
        if(low < high) {
            A[high--] = A[low];
            printf("       从左找%d，放到右端：", A[high+1]);
            for(int i = origLow; i <= origHigh; i++) printf("%d ", A[i]);
            printf("\n");
        }
    }
    A[low] = pivot;
    printf("       枢轴归位：");
    for(int i = origLow; i <= origHigh; i++) printf("%d ", A[i]);
    printf("（枢轴在位置%d）\n", low);
    return low;
}

// ======6. 主函数测试======
int main() {
    printf("====== 冒泡排序与快速排序测试开始 ======\n\n");

    // 【测试1】冒泡排序
    printf("【测试1】冒泡排序\n");
    ElemType arr1[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n1 = 8;
    printf("       原数组：");
    PrintArray(arr1, n1);
    BubbleSort(arr1, n1);
    printf("       排序后：");
    PrintArray(arr1, n1);
    printf("       期望：13 27 38 49 49 65 76 97\n");

    // 【测试2】冒泡排序（已有序，最好情况）
    printf("\n【测试2】冒泡排序（已有序，最好情况O(n)）\n");
    ElemType arr2[] = {1, 2, 3, 4, 5};
    int n2 = 5;
    printf("       原数组：");
    PrintArray(arr2, n2);
    BubbleSort(arr2, n2);
    printf("       排序后：");
    PrintArray(arr2, n2);
    printf("       第一趟无交换，提前结束（changed=false）\n");

    // 【测试3】快速排序
    printf("\n【测试3】快速排序\n");
    ElemType arr3[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n3 = 8;
    printf("       原数组：");
    PrintArray(arr3, n3);
    QuickSortWrapper(arr3, n3);
    printf("       排序后：");
    PrintArray(arr3, n3);
    printf("       期望：13 27 38 49 49 65 76 97\n");

    // 【测试4】快速排序划分过程演示（★ 手算必会）
    printf("\n【测试4】快速排序划分过程演示（选第一个元素为枢轴）\n");
    ElemType arr4[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n4 = 8;
    printf("       原数组：");
    PrintArray(arr4, n4);
    int p = PartitionWithTrace(arr4, 0, n4 - 1);
    printf("       划分结果：枢轴49在位置%d，左侧都<=49，右侧都>=49\n", p);
    printf("       划分后数组：");
    PrintArray(arr4, n4);

    // 【测试5】快速排序最坏情况（已有序）
    printf("\n【测试5】快速排序最坏情况（已有序，固定取端点为枢轴）\n");
    ElemType arr5[] = {1, 2, 3, 4, 5};
    int n5 = 5;
    printf("       原数组（已有序）：");
    PrintArray(arr5, n5);
    printf("       选1为枢轴：划分后左半空，右半[2,3,4,5]，极不均衡\n");
    printf("       递归深度=n，时间退化为O(n^2)\n");
    printf("       改善方法：随机枢轴、三数取中、三向划分\n");
    QuickSortWrapper(arr5, n5);
    printf("       排序后：");
    PrintArray(arr5, n5);

    // 【测试6】两种排序对比
    printf("\n【测试6】冒泡排序 vs 快速排序\n");
    printf("       | 指标     | 冒泡排序       | 快速排序           |\n");
    printf("       |----------|----------------|--------------------|\n");
    printf("       | 最好     | O(n)（有标志） | O(n log n)         |\n");
    printf("       | 平均     | O(n^2)         | O(n log n)         |\n");
    printf("       | 最坏     | O(n^2)         | O(n^2)（枢轴不均） |\n");
    printf("       | 空间     | O(1)           | 平均O(log n)       |\n");
    printf("       | 稳定性   | 稳定           | 不稳定             |\n");
    printf("       | 每趟特征 | 极值归位       | 枢轴归位           |\n");

    // 【测试7】稳定性验证
    printf("\n【测试7】稳定性验证（两个49的相对次序）\n");
    ElemType arr7[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n7 = 8;
    printf("       原数组：第一个49在位置0，第二个49在位置7\n");
    BubbleSort(arr7, n7);
    printf("       冒泡排序后：");
    PrintArray(arr7, n7);
    printf("       冒泡：两个49相对次序不变（稳定）\n");

    ElemType arr8[] = {49, 38, 65, 97, 76, 13, 27, 49};
    QuickSortWrapper(arr8, n7);
    printf("       快速排序后：");
    PrintArray(arr8, n7);
    printf("       快排：相等元素可能交换位置（不稳定，取决于具体划分）\n");

    printf("\n====== 冒泡排序与快速排序测试结束 ======\n");
    return 0;
}
