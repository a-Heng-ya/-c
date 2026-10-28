#include <stdio.h>
#include <stdlib.h>

//限制
#define MAX_SIZE 100

//////////////////////////
//顺序表（数组）集合操作//
//////////////////////////


typedef struct {
    int data[MAX_SIZE];
    int length;
} SeqSet;

// 顺序表初始化
void initSeqSet(SeqSet* s) {
    s->length = 0;
}

// 顺序表判断元素重复
int seqContains(SeqSet* s, int val) {
    for (int i = 0; i < s->length; i++) {
        if (s->data[i] == val) { 
            return 1; 
        }
    }
    return 0; // 不存在
}

// 顺序表添加元素（调用元素去重判断）
void seqAdd(SeqSet* s, int val) {
    if (s->length >= MAX_SIZE) {
        printf("集合已满！\n");
        return;
    }
    if (seqContains(s, val)) {
        printf("元素 %d 已存在(重新输入)\n", val);
        return;
    }
    //上面都返回了实际不会添加元素
    //已存在的元素需要重新输入
    //下面是元素的输入与自增
    s->data[s->length] = val;
    s->length++;
}

// 顺序表展示的打印
void Show(SeqSet* s, const char* name) {
    printf("集合 %s = [ ", name);
    for (int i = 0; i < s->length; i++) {
        printf("%d ", s->data[i]);
    }
    printf("]\n");
}

// 顺序表并集 (返回一个新的集合)
//            (并集不可以重复)
SeqSet seqUnion(SeqSet* A, SeqSet* B) {
    SeqSet C;
    initSeqSet(&C);
    for (int i = 0; i < A->length; i++) seqAdd(&C, A->data[i]);
    for (int i = 0; i < B->length; i++) seqAdd(&C, B->data[i]);
    return C;
}

// 顺序表交集（依旧不可以重复）
SeqSet seqIntersect(SeqSet* A, SeqSet* B) {
    SeqSet C;
    initSeqSet(&C);
    for (int i = 0; i < A->length; i++) {

        if (seqContains(B, A->data[i])) {
            seqAdd(&C, A->data[i]);
        }

    }
    return C;

}

// 顺序表差集 (A + !B)
//先弄个C表A里有B的数据就进去
SeqSet seqDiff(SeqSet* A, SeqSet* B) {

    SeqSet C;
    initSeqSet(&C);

    for (int i = 0; i < A->length; i++) {
        if (!seqContains(B, A->data[i])) {//两个for
            seqAdd(&C, A->data[i]);
        }
    }

    return C;
}


////////////////
//链表集合操作//
////////////////

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} LinkSet;

//链表初始化
void initLinkSet(LinkSet* s) {
    s->head = NULL;
}

//链表判断元素是否存在
int linkContains(LinkSet* s, int val) {
    Node* curr = s->head;
    while (curr != nullptr) {
        if (curr->data == val) return 1;
        curr = curr->next;
    }
    return 0;
}

// 链表：添加元素（去重 + 尾插法）
void listAdd(LinkSet* s, int val) {
    if (linkContains(s, val)) {
        printf("元素 %d 已存在。\n", val);
        return;
    }
    // 申请新节点内存
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data=val;
    newNode->next = nullptr;

    if (s->head ==nullptr) {
        // 如果链表为空，新节点就是头节点
        s->head = newNode;
    }
    else {
        // 非空找尾部
        Node* curr = s->head;
        while (curr->next!=nullptr) {
            curr=curr->next;
        }
        curr->next=newNode; // 挂上去
    }
}

// 链表：显示集合
void linkShow(LinkSet* s, const char* name) {
    printf("集合 %s=[", name);
    Node* curr=s->head;
    while (curr != nullptr) {
        printf("%d ", curr->data);
        curr=curr->next;
    }
    printf("]\n");
}

// 链表：清空并释放内存（非常重要！）
void linkClear(LinkSet* s) {
    Node* curr=s->head;
    while (curr!=nullptr) {
        Node* temp = curr;
        curr=curr->next;
        free(temp); // 释放每一个节点
    }
    s->head = nullptr;
}

// 链表：并集
LinkSet linkUnion(LinkSet* A, LinkSet* B) {
    LinkSet C;
    initLinkSet(&C);
    Node* curr = A->head;
    while (curr !=nullptr) {
        listAdd(&C, curr->data);
        curr = curr->next;
    }
    curr = B->head;
    while (curr !=nullptr) {
        listAdd(&C, curr->data);
        curr=curr->next;
    }
    return C;
}

// 链表：交集
LinkSet linkIntersect(LinkSet* A,LinkSet* B) {
    LinkSet C;
    initLinkSet(&C);
    Node* curr=A->head;
    while (curr!=nullptr) {
        if (linkContains(B, curr->data)) {
            listAdd(&C, curr->data);
        }
        curr = curr->next;
    }
    return C;
}

// 链表：差集 (A - B)
LinkSet linkDiff(LinkSet* A, LinkSet* B) {
    LinkSet C;
    initLinkSet(&C);
    Node* curr = A->head;
    while (curr != NULL) {
        if (!linkContains(B, curr->data)) {
            listAdd(&C, curr->data);
        }
        curr = curr->next;
    }
    return C;
}


///////////////////////////
// 第三部分：菜单与主逻辑
////////////////////////////



void printMenu() {
    printf("\n******************* 集合的基本运算 *******************\n");
    printf("                   1  集合A数据输入\n");
    printf("                   2  集合B数据输入\n");
    printf("                   3  集合A数据显示\n");
    printf("                   4  集合B数据显示\n");
    printf("                   5  集合A和B的并集\n");
    printf("                   6  集合A和B的交集\n");
    printf("                   7  集合A和B的差集(A-B)\n");
    printf("                   8  清空集合A和B (扩展功能)\n");
    printf("                   0  返回上一级\n");
    printf("******************************************************\n");
    printf("请选择功能：");
}


//顺序表模式
void runSeqMode() {
    SeqSet A, B;
    initSeqSet(&A);
    initSeqSet(&B);
    int choice, n, val;

    while (1) {
        printMenu();
        scanf("%d", &choice);

        if (choice == 0) break; // 返回上一级

        switch (choice) {
        case 1:
            printf("请输入集合A的元素个数：");
            scanf("%d", &n);
            printf("请依次输入 %d 个整数(空格隔开)：\n", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &val);
                seqAdd(&A, val);
            }
            break;
        case 2:
            printf("请输入集合B的元素个数：");
            scanf("%d", &n);
            printf("请依次输入 %d 个整数(空格隔开)：\n", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &val);
                seqAdd(&B, val);
            }
            break;
        case 3: Show(&A, "A"); break;
        case 4: Show(&B, "B"); break;
        case 5: { SeqSet C = seqUnion(&A, &B); Show(&C, "A并B"); break; }
        case 6: { SeqSet C = seqIntersect(&A, &B); Show(&C, "A交B"); break; }
        case 7: { SeqSet C = seqDiff(&A, &B); Show(&C, "A差B"); break; }
        case 8: initSeqSet(&A); initSeqSet(&B); printf("已清空A和B。\n"); break;
        default: printf("输入有误，请重新选择！\n");
        }
    }
}

//链表模式
void runLinkMode() {
    LinkSet A, B;
    initLinkSet(&A);
    initLinkSet(&B);
    int choice, n, val;

    while (1) {
        printMenu();
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {
        case 1:
            printf("请输入集合A的元素个数：");
            scanf("%d", &n);
            printf("请依次输入 %d 个整数(空格隔开)：\n", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &val);
                listAdd(&A, val);
            }
            break;
        case 2:
            printf("请输入集合B的元素个数：");
            scanf("%d", &n);
            printf("请依次输入 %d 个整数(空格隔开)：\n", n);
            for (int i = 0; i < n; i++) {
                scanf("%d", &val);
                listAdd(&B, val);
            }
            break;
        case 3: linkShow(&A, "A"); break;
        case 4: linkShow(&B, "B"); break;
        case 5: { LinkSet C = linkUnion(&A, &B); linkShow(&C, "A并B"); linkClear(&C); break; }
        case 6: { LinkSet C = linkIntersect(&A, &B); linkShow(&C, "A交B"); linkClear(&C); break; }
        case 7: { LinkSet C = linkDiff(&A, &B); linkShow(&C, "A差B"); linkClear(&C); break; }
        case 8: linkClear(&A); linkClear(&B); printf("已清空A和B。\n"); break;
        default: printf("输入有误，请重新选择！\n");
        }
    }
    //清理内存
    linkClear(&A);
    linkClear(&B);
}

int main() {
    int mainChoice;
    while (1) {
        printf("\n=== 请选择集合的底层存储结构 ===\n");
        printf("1. 顺序存储 (数组)\n");
        printf("2. 链式存储 (链表)\n");
        printf("0. 退出系统\n");
        printf("请选择：");
        scanf("%d", &mainChoice);

        if (mainChoice == 0) {
            printf("感谢使用，再见！\n");
            break;
        }
        else if (mainChoice == 1) {
            printf("\n>>> 进入顺序表模式 <<<\n");
            runSeqMode();
        }
        else if (mainChoice == 2) {
            printf("\n>>> 进入链表模式 <<<\n");
            runLinkMode();
        }
        else {
            printf("输入无效，请重新输入。\n");
        }
    }

    return 0;
}