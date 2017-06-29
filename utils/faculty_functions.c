#define BUILDING
#define DEBUG

#if defined(BUILDING)
#include <stdio.h>
#endif



#include <data_structure.h>


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

Depart *getDepartByManager(const char *);
/*  通过负责人姓名查找院系
 *  ARGS:   院系负责人 char[12]
 *  RETN:   目标地址 | NULL
 *  NOTE:   院系负责人不同名，返回的是一个院系的数据，或者也可以像下面的这个函数一样，返回结果链
 */

Depart *getDepartByName(const char *);
/*  通过院系名称查找院系
 *  ARGS:   名称线索（不一定是全称）
 *  RETN:   目标链地址 | NULL
 *  NOTE:   由于查找结果可能不只有一个，该操作会创建一个用于储存查询结果的新链表，返回链表的头节点地址
 */

Depart initDepart(void);
/*  创建一个院系数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */



/*********** Unit Test **************/

#if defined(BUILDING)
void main(void) {
    // pass
}
#endif


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
    if (tail.next != NULL) {
        #if defined(DEBUG)
        puts("[LOG] Error in appendDepart():\n\ttail.next not pointing to NULL");
        #endif
        return NULL;
    }

    tail.next = (Depart *)malloc(sizeof(Depart));   // 在尾部链接新节点

    *(tail.next) = new_one;     // 直接用new_one作为模板，向新节点中写入数据

    // new_one中所有指针都指向NULL

    return tail.next;   // 返回新节点地址
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

Depart *getDepartByManager(const char *);

Depart *getDepartByName(const char *);
