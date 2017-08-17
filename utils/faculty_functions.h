#ifndef FACULTY_FUNCTIONS
#define FACULTY_FUNCTIONS

/************ Declaration ************/

    /**** POST | DELETE | PUT ****/

extern Depart *appendDepart(Depart *head, DepartData new_one);
/*  录入院系
 *  ARGS:   链表头，已有数据的作为buffer的DepartData实例
 *  RETN:   新增节点的地址
 */

extern int modifyDepart(Depart *target, DepartData new_one);
/*  修改院系信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

extern int removeDepart(Depart **phead, Depart *target);
/*  删除院系节点
 *  ARGS:   指向院系链表头节点地址的指针，目标地址 | NULL
 *  RETN:   success code
 */

extern DepartData initDepartData(void);
/*  创建一个院系数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */

extern Depart *createDepartHead(void);
/*  创建并初始化头节点
 *  ARGS:   void
 *  RETN:   头节点地址 || NULL
 */


    /**** SELECT ****/

extern DepartWrapper *getDepartByManager(Depart *, Depart *, const char *);
/*  通过负责人姓名查找院系
 *  ARGS:   院系链表，搜索结束点，院系负责人 char[12]
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   院系负责人不同名，返回的是一个院系的数据，或者也可以像下面的这个函数一样，返回结果链
 *  NOTE:   只能在院系链有数据的情况下调用该函数！
 *  NOTE:   该函数会申请DepartWrapper占用空间，记得调用cleanupDepartWrapper()
 */

extern DepartWrapper *getDepartByName(Depart *, Depart *, const char *);
/*  通过院系名称查找院系
 *  ARGS:   院系链表，搜索结束点，名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   由于查找结果可能不只有一个，该操作会创建一个用于储存查询结果的新链表，返回链表的头节点地址
 *  NOTE:   该函数会申请DepartWrapper占用空间，记得调用cleanupDepartWrapper()
 */

// extern DepartStatWrapper *getDepartOrderedByMasterTeacherRatio(Depart *, Depart *);
// /*  获取按照学生-老师比例排序的院系链
//  *  ARGS:   院系链表，搜索边界
//  *  RETN:   统计结果链挂载点
//  */

extern DepartStatWrapper *buildDepartStatChainUnordered(Depart *, Depart *);
extern DepartStatWrapper *orderDepartStatWrapperBySTRatio(DepartStatWrapper *);
extern DepartStatWrapper *orderDepartStatWrapperByProjectTotal(DepartStatWrapper *);

extern Depart *getPrevDepart(Depart *cur, Depart *head);
/*  获得当前院系节点的前一个节点
 *  ARGS:   当前节点，院系链表头
 *  RETN:   前一个节点 || NULL
 */


    /**** CLEANUPs ****/

extern void cleanupDepartWrapper(DepartWrapper *start);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 *  NOTE:   每次搜完了记得调一次啊。。。
 *  NOTE:   调用后传进来的那个节点也没了！
 */

extern void cleanupDepart(Depart *start);
/*  释放Depart链所占用的内存空间
 *  ARGS:   头节点地址
 *  RETN:   void
 */

extern void cleanupDepartStatWrapper(DepartStatWrapper *);


#endif
