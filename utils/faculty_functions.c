#include "data_structure.h"


#define BUILDING
#define DEBUG

#if defined(BUILDING)
#include <stdio.h>
#include <stdlib.h>
#endif


#include <string.h>


// Depart *Depart_HEAD = NULL;




/************ Declaration ************/

Depart *appendDepart(Depart *tail, Depart new_one);
/*  录入院系
 *  ARGS:   链表尾，已有数据的作为buffer的Depart节点
 *  RETN:   新增节点的地址
 */

int modifyDepart(Depart *target, Depart new_one);
/*  修改院系信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

int removeDepart(Depart *target);
/*  删除院系节点（也可以用作free方法？？）
 *  ARGS:   目标地址 | NULL
 *  RETN:   success code
 */

DepartWrapper *getDepartByManager(Depart *, const char *, DepartWrapper *);
/*  通过负责人姓名查找院系
 *  ARGS:   院系链表，院系负责人 char[12]，搜索结果挂载点
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   院系负责人不同名，返回的是一个院系的数据，或者也可以像下面的这个函数一样，返回结果链
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

void cleanupDepartWrapper(DepartWrapper *start, DepartWrapper *end);
/*  清空搜索结果序列
 *  ARGS:   头节点地址，尾节点地址
 *  RETN:   void
 *  NOTE:   每次搜完了记得调一次啊。。。
 */


/*************** Function Realizations ***************/

Depart initDepart(void) {
    /* TODO: 增加图形界面后记得修改数据来源 */
    Depart Computer;
    scanf("%s\n", Computer.name);
    scanf("%s\n", Computer.manager);
    scanf("%s\n", Computer.mobile);
    /* DON'T CHANGE ME */
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



int removeDepart(Depart *target) {

    /* 错误处理：同上 */
    if (target == NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in removeDepart():\n\ttarget is NULL");
        #endif
        return 0;
    }

    free(target);
    target = NULL;

    return 1;
}


void cleanupDepartWrapper(DepartWrapper *start, DepartWrapper *end) {
    // NOTE: end.next == NULL, u can point end to elsewhere
    while (1) {
        end = start->next;   // 保存下一个节点位置
        free(start);
        start = end;
        if (start == NULL) { break; }   // 全部清空
    }
    // start & end will be NULL hence
    return;
}




DepartWrapper *getDepartByManager(Depart *start,
                                  const char *manager,
                                  DepartWrapper *rtn) {
    /* NOTE: 这里还是按照有同名的情况来找，然后下面那个函数我就可以复制粘贴了 /doge */

    DepartWrapper *rtn_head = rtn;  // 保存搜索结果链的头指针
    rtn_head->depart = NULL; rtn_head->next = NULL;   // 再次初始化


    while (1) {     // using `break`

        if (!strcmp(start->manager, manager)) {  // BINGO!

            // 在搜索结果中登记
            if (rtn == rtn_head) {  // 第一个结果 - 头处理
                rtn->depart = start;
            } else {    // 不是第一个结果 - 扩展搜索结果
                // 申请空间
                rtn->next = (DepartWrapper *)malloc(sizeof(DepartWrapper));
                if (rtn->next == NULL) {     // 申请失败
                    #if defined(DEBUG)
                    puts("[LOG] Error in getDepartByManager():\n\tfailed to malloc for result container");
                    #endif
                    // 申请空间失败则中断操作，并清空搜索结果
                    cleanupDepartWrapper(rtn_head, rtn);
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
           if (rtn == rtn_head) {
               rtn->depart = start;
           } else {
               rtn->next = (DepartWrapper *)malloc(sizeof(DepartWrapper));
               if (rtn->next == NULL) {
                   #if defined(DEBUG)
                   // 好吧还有一个。。
                   puts("[LOG] Error in getDepartByName():\n\tfailed to malloc for result container");
                   #endif
                   cleanupDepartWrapper(rtn_head, rtn);
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





/*********** Unit Test **************/

#if defined(BUILDING)
void main(void) {
    // pass
}
#endif
