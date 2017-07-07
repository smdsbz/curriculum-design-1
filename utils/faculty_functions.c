#include "data_structure.h"

#ifdef BUILDING
#undef BUILDING
#endif
#define BUILDING

#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG

#if defined(BUILDING)
#include <stdio.h>
#include <stdlib.h>
#endif





/************ Declaration ************/

Depart *appendDepart(Depart *tail, DepartData new_one);
/*  录入院系
 *  ARGS:   链表尾，已有数据的作为buffer的DepartData实例
 *  RETN:   新增节点的地址
 */

int modifyDepart(Depart *target, DepartData new_one);
/*  修改院系信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

int removeDepart(Depart **phead, Depart *target);
/*  删除院系节点（也可以用作free方法？？）
 *  ARGS:   指向院系链表头节点地址的指针，目标地址 | NULL
 *  RETN:   success code
 */

DepartWrapper *getDepartByManager(Depart *, const char *, DepartWrapper *);
/*  通过负责人姓名查找院系
 *  ARGS:   院系链表，院系负责人 char[12]，搜索结果挂载点
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   院系负责人不同名，返回的是一个院系的数据，或者也可以像下面的这个函数一样，返回结果链
 *  NOTE:   只能在院系链有数据的情况下调用该函数！
 */

DepartWrapper *getDepartByName(Depart *, const char *, DepartWrapper *);
/*  通过院系名称查找院系
 *  ARGS:   名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   由于查找结果可能不只有一个，该操作会创建一个用于储存查询结果的新链表，返回链表的头节点地址
 */

Depart initDepart(void);
/*  创建一个院系数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */

void cleanupDepartWrapper(DepartWrapper *start);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 *  NOTE:   每次搜完了记得调一次啊。。。
 *  NOTE:   调用后传进来的那个节点也没了！
 */




#if defined(BUILDING)
/*********** Unit Test **************/

void printDepartToConsole(Depart *Comp) {
    /* 向终端打印院系信息 */
    printf("<Department @ 0x%p>\n", Comp);
    printf("\tthis.name = %s\n", Comp->data->name);
    printf("\tthis.manager = %s\n", Comp->data->manager);
    printf("\t\tthis.mobile = %s\n", Comp->data->mobile);
    printf("\tthis.team_num = %d\n", Comp->data->team_num);
    printf("\n\tnext_node @ 0x%p\n", Comp->next);
    printf("\tteam from 0x%p to 0x%p\n", Comp->child_team_head, Comp->child_team_tail);
    putchar('\n'); putchar('\n');
}

void printDepartWrapperToConsole(DepartWrapper *head) {
    printf("<DepartWrapper @ 0x%p>\n", head);
    int counter = 0;
    while (head->depart != NULL) {
        ++counter;
        printf("\tfound %s @ 0x%p\n", head->depart->name, head->depart);
        head = head->next;
        if (head == NULL) { break; }
    }
    if (!counter) { puts("no match!"); }
    putchar('\n'); putchar('\n');
}



void main(void) {
    Depart *HEAD = (Depart *)malloc(sizeof(Depart));
    // NOTE: 创建完头节点后记得初始化！！！
    HEAD->next = NULL, HEAD->child_team_head = NULL; HEAD->child_team_tail = NULL;

    // first node creation demo
    modifyDepart(HEAD, initDepart());   // 向第一节点写入数据用modify
    printDepartToConsole(HEAD);

    // second node creation demo
    appendDepart(HEAD, initDepart());   // 第一个节点的数据已经初始化了，则用添加节点
    printDepartToConsole(HEAD->next);

    // query demo
    DepartWrapper *RST_LIST = (DepartWrapper *)malloc(sizeof(DepartWrapper));
    auto char buf[40] = {'\0'};

    printf("search by manager: "); scanf("%s", buf);
    getDepartByManager(HEAD, buf, RST_LIST);
    printDepartWrapperToConsole(RST_LIST);
        // NOTE: DON'T FORGET THIS!!!
    cleanupDepartWrapper(RST_LIST);
        // NOTE: RST_LIST在清理之后也free掉了，需要重新申请搜索结果挂载点

    // search demo
    RST_LIST = (DepartWrapper *)malloc(sizeof(DepartWrapper));
    printf("search by name: "); scanf("%s", buf);
    getDepartByName(HEAD, buf, RST_LIST);
    printDepartWrapperToConsole(RST_LIST);
    cleanupDepartWrapper(RST_LIST);

    // remove demo
    removeDepart(&HEAD, HEAD->next);
    printDepartToConsole(HEAD);

    // simple cleanup
    // HACK: cleanupDepart() to simplify this process
    free(HEAD->next);
    free(HEAD);
}
#endif







/*************** Function Realizations ***************/

Depart initDepart(void) {
    // TODO: 增加图形界面后记得修改数据来源
    // NOTE: 载入内存
    Depart Computer;
    printf("depart.name = "); scanf("%s", Computer.name);
    printf("depart.manager = "); scanf("%s", Computer.manager);
    printf("depart.mobile = "); scanf("%s", Computer.mobile);
    /* DON'T CHANGE ME */
    Computer.team_num = 0;
    Computer.next = NULL;
    Computer.child_team_head = NULL;
    Computer.child_team_tail = NULL;
    /*******************/
    return Computer;
}




Depart *appendDepart(Depart *tail, Depart new_one) {

    /* 错误处理：传入的节点地址不是最后一个节点的地址，防止数据丢失 */
    if (tail->next != NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in appendDepart():\n\ttail.next not pointing to NULL");
        #endif
        return NULL;
    }

    tail->next = (Depart *)malloc(sizeof(Depart));   // 在尾部链接新节点
    if (tail->next == NULL) {    // 内存空间申请失败
        #if defined(DEBUG)
        puts("[LOG] Error in appendDepart():\n\tfailed to alloc memory");
        #endif
        return NULL;
    }

    *(tail->next) = new_one;     // 直接用new_one作为模板，向新节点中写入数据

    // new_one中所有指针都指向NULL

    return tail->next;   // 返回新节点地址
}




int modifyDepart(Depart *target, Depart new_one) {

    /* 错误处理：链式调用时，防止前一级调用中产生的“错误”传递，如查找节点方法返回的NULL */
    if (target == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in modifyDepart():\n\ttarget is NULL");
        #endif
        return 0;   // error code
    }

    *target = new_one;

    return 1;
}




int removeDepart(Depart **phead, Depart *target) {

    /* 错误处理：同上 */
    if (target == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in removeDepart():\n\ttarget is NULL");
        #endif
        return 0;
    }

    /* problem code
    free(target);
    target = NULL;  // target will be deleted out of the function, no need to redirect
    */

    if (*phead == target) {     // 要删除的节点是头节点
        *phead = target->next;  // 头节点重新赋值
        (*phead)->next = target->next->next;   // 链表重新链接
    } else {    // 要删除的节点不是头节点
        Depart *phead_safe = *phead;
        for (; phead_safe->next != target; phead_safe = phead_safe->next) ;
        phead_safe->next = phead_safe->next->next;
    }

    // after all the routing things are done
    free(target);

    return 1;
}




void cleanupDepartWrapper(DepartWrapper *start) {
    DepartWrapper *end = start;
    while (1) {
        end = start->next;   // 保存下一个节点位置
        #if defined(BUILDING)
        printf("[LOG] cleanupDepartWrapper(): freed 0x%p\n", start);
        #endif
        free(start);
        start = end;
        if (start == NULL) { break; }   // 全部清空
    }
    // start & end will be NULL hence
    return;
    // NOTE: 此时，你传进来的DepartWrapper头节点也没了。。。
}




DepartWrapper *getDepartByManager(Depart *start,
    const char *manager,
    DepartWrapper *rtn) {
        /* NOTE: 这里还是按照有同名的情况来找，以后我就可以复制粘贴了 :doge: */

        DepartWrapper *rtn_head = rtn;  // 保存搜索结果链的头指针
        rtn_head->depart = NULL; rtn_head->next = NULL;   // 再次初始化


        while (1) {     // using `break`
            // NOTE: 只能在有数据的情况下调用该函数

            if (!strcmp(start->manager, manager)) {  // BINGO!
                #if defined(BUILDING)
                printf("[LOG] getDepartByManager(): found %s @ 0x%p\n", start->name, start);
                #endif
                // 在搜索结果中登记
                if (rtn_head->depart == NULL) {  // 第一个结果 - 头处理
                    rtn->depart = start;
                } else {    // 不是第一个结果 - 扩展搜索结果
                    // 申请空间
                    rtn->next = (DepartWrapper *)malloc(sizeof(DepartWrapper));
                    if (rtn->next == NULL) {     // 申请失败
                        #if defined(DEBUG)
                        puts("[LOG] Error in getDepartByManager():\n\tfailed to malloc for result container");
                        #endif
                        // 申请空间失败则中断操作，并清空搜索结果
                        cleanupDepartWrapper(rtn_head);
                        return NULL;
                    }
                    // 申请成功，注册搜索结果
                    rtn = rtn->next; rtn->next = NULL;
                    rtn->depart = start;
                }

            }

            start = start->next;     // 下一个院系（可以用for循环，懒得改了）
            if (start == NULL) { break; }   // 完整遍历，收工了
        }
        return rtn_head;    // 不管有没有找到，都返回搜索结果链表头
    }




DepartWrapper *getDepartByName(Depart *start,
                               const char *name,
                               DepartWrapper *rtn) {
    DepartWrapper *rtn_head = rtn;
    rtn_head->depart = NULL; rtn_head->next = NULL;
    while (1) {
        // NOTE: 和上一个函数唯一的不同
        if (strstr(start->name, name)) {  // 搜索内容是已保存内容的子串
            #if defined(BUILDING)
            printf("[LOG] getDepartByName(): found %s @ 0x%p\n", start->name, start);
            #endif
            if (rtn_head->depart == NULL) {
                rtn->depart = start;
            } else {
                rtn->next = (DepartWrapper *)malloc(sizeof(DepartWrapper));
                if (rtn->next == NULL) {
                    #if defined(DEBUG)
                    puts("[LOG] Error in getDepartByName():\n\tfailed to malloc for result container");  // 好吧还有一个。。
                    #endif
                    cleanupDepartWrapper(rtn_head);
                    return NULL;
                }
                rtn = rtn->next; rtn->next = NULL;
                rtn->depart = start;
            }

        }
        start = start->next;
        if (start == NULL) { break; }
    }
    return rtn_head;
}
