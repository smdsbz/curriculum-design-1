/*  $PROJ_HOME/utils/data_structure.c
 *  所有的数据结构的定义都在这里
 */

#ifndef DATA_STRUCTURE
#define DATA_STRUCTURE  "data_structure.h"

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
// 正在开发标识 - 解注释启用单元测试
// #define BUILDING

// HACK: 双向链表
// #define DUAL_WAY_CHAIN
// PRO: 使用双向链表，查找的时候使用二分法，加快查找速度，加分项
// CON: 但是由于所有节点都是按录入时间排序的，除非对所有关键字hash转换成数字id，否则定义双向链表貌似意义不大。。。
// 但是现在我懒得写hash :triumph:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/******** Data ********/

    /**** Depart ****/

/* 院系数据 */
typedef struct _DepartData {
    // self.data
    char    name[20];       // 院系名称
    char    manager[12];    // 负责人
    char    mobile[15];     // 负责人电话
} DepartData;

/* 院系节点 */
typedef struct _Depart {
    struct _DepartData  *data;          // 数据域
    struct _Depart  *next;              // 下一个院系节点
    struct _Team    *child_team_head;   // 所有团队中的第一个
    struct _Team    *child_team_tail;   // '''''''中的最后一个
} Depart;

/* 院系搜索结果包装 */
typedef struct _DepartWrapper {
    struct _Depart          *depart;    // 符合条件的院系节点
    struct _DepartWrapper   *next;      // 下一个结果
} DepartWrapper;

/* 院系统计数据 */
typedef struct _DepartStatData {
    int     student_num;    // 学生总数
    int     teacher_num;    // 教师总数
    float   st_ratio;       // 学生/教师比例
    int     project_total;  // 项目总数
    int     project_973;    // 973项目数
    int     project_863;    // 863项目数
    float   funding;        // 科研总经费
    float   avg_funding;    // 项目平均经费
} DepartStatData;

/* 院系统计结果包装 */
typedef struct _DepartStatWrapper {
    struct _Depart              *depart;    // 源数据节点
    struct _DepartStatData      stat;       // 统计数据
    struct _DepartStatWrapper   *next;      // 下一个节点
} DepartStatWrapper;

    /**** Team ****/

/* 团队数据 */
typedef struct _TeamData {
    char    name[30];       // 团队名称
    char    manager[12];    // 负责人
    int     teacher_num;    // 教师人数
    int     student_num;    // 研究生人数
    char    faculty[20];    // 所属院系
} TeamData;

/* 团队节点 */
typedef struct _Team {
    struct _TeamData    *data;
    struct _Depart  *parent_depart; // 所属院系
    struct _Team    *next;
    struct _Project *child_project_head;
    struct _Project *child_project_tail;
} Team;

/* 团队搜索结果包装 */
typedef struct _TeamWrapper {
    struct _Team        *team;
    struct _TeamWrapper *next;
} TeamWrapper;

/* 团队统计数据 */
typedef struct _TeamStatData {
    int     project_total;  // 项目总数
    int     project_NSFC;   // NSFC项目数
    float   funding;        // 总经费
    float   pt_ratio;       // 项目数/教师人数
} TeamStatData;

/* 团队统计结果包装 */
typedef struct _TeamStatWrapper {
    struct _Team                *team;
    struct _TeamStatData        stat;
    struct _TeamStatWrapper     *next;
} TeamStatWrapper;

    /**** Project ****/

/* 项目数据 */
typedef struct _ProjectData {
    char    id[15];         // 项目编号
    char    type;           // 项目类别
    char    start_date[8];  // 起始时间
    float   funding;        // 项目经费
    char    manager[12];    // 项目负责人
    char    team[30];       // 所属团队
} ProjectData;

/* 项目节点 */
typedef struct _Project {
    struct _ProjectData *data;
    struct _Team        *parent_team;
    struct _Project     *next;
} Project;

/* 项目搜索结果包装 */
typedef struct _ProjectWrapper {
    struct _Project         *project;
    struct _ProjectWrapper  *next;
} ProjectWrapper;

/******** Search Condition ********/
/* 搜索条件 - 用于按教师人数查询团队功能 */
typedef struct _Where {
    char    direction[3];   // 查找条件 - ^[\>\<]=?|=$
    int     value;
} Where;

/******** Mounting Point Group ********/
/* 头节点挂载点组 - 用于返回一组指针 */
typedef struct _MountPoint {
    struct _Depart      *depart_head;   // 院系链表挂载点
    struct _Team        *team_head;     // 团队链表挂载点
    struct _Project     *project_head;  // 项目链表挂载点
} MountPoint;

/*********** Unit Test **************/

#if defined(BUILDING)
void main(void) {
    puts("Successfully constructed!");
}
#endif

#endif
