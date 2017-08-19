#ifndef DATA_STRUCTURE
#define DATA_STRUCTURE  "data_structure.h"
// #endif

/*  $PROJ_HOME/utils/data_structure.c
 *
 *  @author smdsbz@github.com
 *          U201610136 CSIE1601 朱晓光
 *
 *  所有的数据结构的定义都在这里
 */


/**** 简单展示 *****

Department_HEAD --> ... <--> Department <--> next_Department <--> ...
                             |
                             +---------------------------------+-----------------------+
                                                               |                       |
Team_HEAD --------> ... <--> Team_from_another_Department <--> Team_1 <--> Team_2 <--> ...
                                                               |
                             +-----------------------+---------+
                             |                       |
Project_HEAD -----> ... <--> Proj_1 <--> Proj_2 <--> ...


采用了这样比较分立的设计，主要是考虑到在查找项目的时候，传统的树形结构不能直接获得所有的底层节点，造成很多麻烦

现在的新操作同时支持了树形操作和链式操作

* 所有节点以录入系统的时间顺序存放 FILO顺序

*/



#ifdef BUILDING
#undef BUILDING
#endif
// #define BUILDING        // 正在开发，包含main函数，单元测试

// #if defined(BUILDING)
// #include <stdio.h>
// #endif


// HACK: 双向链表
// #define DUAL_WAY_CHAIN
// PRO: 使用双向链表，查找的时候使用二分法，加快查找速度，加分项
// CON: 但是由于所有节点都是按录入时间排序的，除非对所有关键字hash转换成数字id，否则定义双向链表貌似意义不大。。。
// 但是现在我懒得写hash，所以我拒绝，单向链表你慢慢找吧:triumph:

// 链表结构辅助标识 - 子节点计数器
// #define CHILD_COUNTER



#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // will be used in .c files


/*********** Definitions ************/

    /**** Depart ****/

typedef struct _DepartData {
    // self.data
    char    name[20];       // 院系名称
    char    manager[12];    // 负责人
    char    mobile[15];     // 负责人电话
    // added
    #if defined(CHILD_COUNTER)
    int     team_num;       // 该院系团队总数
    #endif
    // stat
    // int     student_num;
    // int     teacher_num;
    // int     project_total;
    // int     project_973;
    // int     project_863;
    // float   funding;
} DepartData;


typedef struct _Depart {
    // data
    struct _DepartData  *data;  // 指向数据域
    // navigation
    #if defined(DUAL_WAY_CHAIN)
    struct _Depart  *prev;  // 上一个院系节点
    #endif
    struct _Depart  *next;  // 下一个院系节点
    // navigation - child node range
    struct _Team    *child_team_head;       // 该院系所有团队中的第一个
    struct _Team    *child_team_tail;       // ''''''''''''中的最后一个
} Depart;


typedef struct _DepartWrapper {
    /* 用来储存搜索结果 */
    struct _Depart          *depart;    // 指向符合条件的节点
    struct _DepartWrapper   *next;      // 指向下一个结果wrapper
} DepartWrapper;

typedef struct _DepartStatData {
    int                         student_num;    // 该院系下学生总数
    int                         teacher_num;    // 该院系下教师总数
    // float                       st_ratio;       // 该院系学生/教师比例
    int                         project_total;
    int                         project_973;
    int                         project_863;
    float                       funding;
} DepartStatData;

typedef struct _DepartStatWrapper {
    /* 用来储存人数统计信息 */
    struct _Depart              *depart;       // 指向源数据节点
    struct _DepartStatData      stat;
    struct _DepartStatWrapper   *next;          // 下一个节点
} DepartStatWrapper;

    /**** Team ****/

typedef struct _TeamData {
    // self.data
    char    name[30];       // 团队名称
    char    manager[12];    // 负责人
    int     teacher_num;    // 教师人数
    int     student_num;    // 研究生人数
    char    faculty[20];    // 所属院系
    #if defined(CHILD_COUNTER)
    int     project_num;    // 该团队所管理的项目总数
    #endif
} TeamData;


typedef struct _Team {
    // data
    struct _TeamData    *data;
    // navigation
    struct _Depart  *parent_depart; // 所属院系节点
    #if defined(DUAL_WAY_CHAIN)
    struct _Team    *prev;      // 上一个同级节点
    #endif
    struct _Team    *next;      // 下一个同级节点
    // navigation - child node range
    struct _Project *child_project_head;    // 该团队所有项目中的第一个
    struct _Project *child_project_tail;    // ''''''''''''中的最后一个
} Team;


typedef struct _TeamWrapper {
    struct _Team        *team;
    struct _TeamWrapper *next;
} TeamWrapper;


typedef struct _TeamStatData {
    int     project_total;
    int     project_NSFC;
    float   funding;
} TeamStatData;

typedef struct _TeamStatWrapper {
    struct _Team                *team;
    struct _TeamStatData        stat;
    struct _TeamStatWrapper     *next;
} TeamStatWrapper;

    /**** Project ****/

typedef struct _ProjectData {
    // self.data
    char    id[15];         // 项目编号
    char    type;           // 项目类别
    char    start_date[8];  // 起始时间
    float   funding;        // 项目经费
    char    manager[12];    // 项目负责人
    char    team[30];       // 所属团队
} ProjectData;


typedef struct _Project {
    // data
    struct _ProjectData *data;
    // navigation
    struct _Team    *parent_team;   // 负责团队的节点
    #if defined(DUAL_WAY_CHAIN)
    struct _Project *prev;      // 上一个同级节点
    #endif
    struct _Project *next;      // 下一个同级节点
} Project;


typedef struct _ProjectWrapper {
    struct _Project         *project;
    struct _ProjectWrapper  *next;
} ProjectWrapper;



/**** Additional typdef: Search Condition ****/

typedef struct _Where {
    char    direction[3];   // 查找条件 - ^[\>\<]=?|=$
    int     value;
} Where;



/**** Additional typedef: Mounting Points ****/

typedef struct _MountPoint {    // 用于返回一组指针
    struct _Depart      *depart_head;       // 院系链表挂载点
    struct _Team        *team_head;         // 团队链表挂载点
    struct _Project     *project_head;      // 项目链表挂载点
} MountPoint;


/*********** Unit Test **************/

#if defined(BUILDING)
void main(void) {
    puts("Successfully constructed!");
}
#endif


#endif
