/*  $PROGRAM_ROOT/utils/faculty_functions.h
 *  院系方法头文件
 */

#ifndef FACULTY_FUNCTIONS
#define FACULTY_FUNCTIONS

/************ Declaration ************/

    /**** POST | DELETE | PUT ****/

extern Depart *appendDepart(Depart *head, DepartData new_one);
/*  录入院系
 *  ARGS:   链表头，院系数据模板
 *  RETN:   新增节点的地址
 */

extern int modifyDepart(Depart *target, DepartData new_one);
/*  覆盖院系信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 *  NOTE:   没有调用！（clang可能报错）
 */

extern int removeDepart(Depart **phead, Depart *target);
/*  删除院系节点
 *  ARGS:   指向院系链表头节点地址的指针，目标地址 | NULL
 *  RETN:   success code
 */

extern DepartData initDepartData(void);
/*  创建院系数据模板
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的模板
 *  NOTE:   包含用户输入前端
 */

extern Depart *createDepartHead(void);
/*  创建并初始化头节点
 *  ARGS:   void
 *  RETN:   头节点地址 || NULL
 *  NOTE:   头节点此时没有数据域
 */

    /**** SELECT ****/

// NOTE: 会申请DepartWrapper空间，记得调用cleanupDepartWrapper()释放内存

extern DepartWrapper *getDepartByManager(Depart *, Depart *, const char *);
/*  通过负责人姓名查找院系
 *  ARGS:   院系链表，搜索结束点，院系负责人 char[12]
 *  RETN:   搜索结果挂载点 || NULL （没有结果时也返回挂载点地址）
 */

extern DepartWrapper *getDepartByName(Depart *, Depart *, const char *);
/*  通过院系名称查找院系
 *  ARGS:   院系链表，搜索结束点，名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 || NULL
 */

    /**** STAT ****/

extern DepartStatWrapper *buildDepartStatChainUnordered(Depart *, Depart *, int);
/*  统计所有院系数据
 *  ARGS:   院系链表头，遍历区域边界，查询目标年度
 *  RETN:   统计结果挂载点 || NULL
 */

extern DepartStatWrapper *orderDepartStatWrapperBySTRatio(DepartStatWrapper *);
/*  将统计结果按学生/教师人数比排序
 *  ARGS:   统计结果挂载点
 *  RETN:   排序后统计结果链的挂载点
 */

extern DepartStatWrapper *orderDepartStatWrapperByProjectTotal(DepartStatWrapper *);
/*  将统计结果按项目总数降序排序
 */

extern DepartStatWrapper *orderDepartStatWrapperByAvgFunding(DepartStatWrapper *);
/*  将统计结果按项目平均经费降序排序
 */

extern Depart *getPrevDepart(Depart *cur, Depart *head);
/*  获得当前院系节点的前一个节点
 *  ARGS:   当前节点，院系链表头
 *  RETN:   前一个节点 || NULL
 */

    /**** CLEANUPs ****/

extern void cleanupDepartWrapper(DepartWrapper *start);
/*  清除搜索结果链表
 *  ARGS:   搜索结果链表头节点地址
 *  RETN:   void
 *  NOTE:   每次搜完了记得调用！
 *  NOTE:   调用后搜索结果会被完全清空，包括头节点本身
 */

extern void cleanupDepart(Depart *start);
/*  释放院系链所占用的内存空间
 *  ARGS:   院系链表挂载点
 *  RETN:   void
 */

extern void cleanupDepartStatWrapper(DepartStatWrapper *);
/*  清除统计结果链表
 *  ARGS:   统计结果链表头节点地址
 *  RETN:   void
 */

#endif
