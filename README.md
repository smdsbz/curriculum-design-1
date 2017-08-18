# curriculum-design-1
大一下课程设计 - 科研项目信息管理系统


## 须知
由于是课程设计项目，该项目遵循`GNU AGPL v3.0`许可。  
**借鉴之前请联系本人！！！**

-----------------------------------------------------------


## Features

### 数据维护
- [x] 院系基本信息维护
    - [x] 录入
    - [x] 修改
    - [x] 删除
- [x] 科研团队基本信息维护
    - ...
- [x] 科研项目基本信息维护
    - ...


### 数据查询

#### 关键字
_(院系负责人、科研团队不同名)_

- 院系
    - [x] 院系负责人
    - [x] 院系名称的全部或一部分

- 科研团队
    - [x] 团队名称的全部或一部分
    - [x] 教师人数满足条件（大于|小于|等于）

- 科研项目
    - [x] 项目编号
    - [x] 所属团队



### 数据统计

- [x] 人数统计
    - 教师总数
    - 研究生总数
    - 研究生与教师的人数比（保留两位小数）
        - [x] 降序排序，输出结果
- [x] 项目统计
    - 某年度各院系科研项目数、973项目数、863项目数、科研总经费
        - [x] 按科研项目数降序排序，输出结果

- [x] 历年来类别为国家自然科学基金的科研项目数最多的10个科研团队
    - [x] 按项目数降序排序，输出科研团队名称、国家自然科学基金项目数、项目总经费
- [ ] 项目数与教师人数的比值最高的5个科研团队
    - [ ] 按比值降序排序，输出团队名称、教师人数、科研项目数、项目数和教师人数比值

- *(自行设计)*

-------------------------------------------------------------

## User Manual

### Cookbook

```
${PROGRAM_ROOT} → ./the_programme.exe ${DATA_FOLDER}
========================================================
  \ +---------+--+---------+**+-------+**\ +---------+
  *\|  +------+  |  +---+  |\*|        \**\|  +------+
  **|  |   \**\--|  |-\ |  |*\|  ++  +  |**|  |       
  --|  +------+--|  +---+  |**|  ||  |  |--|  +------+
    +------+  |\ |  +------+**|  ||  |  |  +------+  |
  --------\|  |*\|  |      \**|  ||  |  |--------\|  |
  \*+------+  |\*|  |       \*|  ||  |  |\*+------+  |
   \+---------+ \+--+        \+--++--+--+ \+---------+    by smdsbz
========================================================
The Science Project Managing System
Version: 0.1
Author: smdsbz@GitHub.com (i.e. U201610136 朱晓光)
Enjoy your ride!

/* if data folder is unspecified */
Data folder unspecified!
Finding data files in program root...

/** if not found **/
Data not found!
Create data file in program root? [Y/N]:

/*** Y ***/
Creating data files...
Data files found, loading...
Successfully loaded all data!

/*** N ***/
No data available!
exit

/* if data found */
Data files found, loading...
Successfully loaded all data!

Operations Available:
     0) Quit
-->| 1) List out all records
     2) Query utilities
     3) Statistics utilities

========================================================
/* Query utils */
> 2
Query Objects:
     0) Go back
     1) Departments
     2) Teams
     3) Projects

/** Query departments **/
query > 1
Query Methods:
     0) Go back
     1) By name
     2) By manager

query/department >
========================================================
/* Statistics utils */
> 3
Statistics Catalogue:
     0) Go back
     1) Department
     2) Team
     3) Project
     4) Other

stat > 1
Department Statistics Catalogue:
     0) Go back
     1) Total teacher number
     2) Total master student number
     3) Master-teacher ratio
     4) Project number and funding
```

### Start With Data (GG)
```
${PROGRAM_ROOT} → ./the_programme.exe ${DATA_FOLDER}
========================================================
  \ +---------+--+---------+**+-------+**\ +---------+
  *\|  +------+  |  +---+  |\*|        \**\|  +------+
  **|  |   \**\--|  |-\ |  |*\|  ++  +  |**|  |       
  --|  +------+--|  +---+  |**|  ||  |  |--|  +------+
    +------+  |\ |  +------+**|  ||  |  |  +------+  |
  --------\|  |*\|  |      \**|  ||  |  |--------\|  |
  \*+------+  |\*|  |       \*|  ||  |  |\*+------+  |
   \+---------+ \+--+        \+--++--+--+ \+---------+    by smdsbz
========================================================
The Science Project Managing System
Version: 0.1
Author: smdsbz@GitHub.com (i.e. U201610136 朱晓光)
Enjoy your ride!

Loading data from ${DATA}...
Successfully loaded all data!
depart > help

Operations Available:                               /* depart level */
    ls      - list out all departments
    tree    - list out all departments, teams and projects
    select  - (sub-programme) focus on the node specified
              usage:                                /* NOTE: universal */
                select  -d | --depart    (-n --name | -m --manager)
                        -t | --team      (-n --name | -t --teacher-number)
                        -p | --project   (-i --id   | -t --team)
    s save  - save all changes
    q quit  - exit the system
========================================================
depart > ls

    Name      16|   Manager   15|   Tele      15
 ---------------|---------------|---------------
__计算机学院      |__张三         |__13322224444
__一个名字特别特别长的学院|__李四         |__13388883333

depart > tree

  计算机学院
    火箭队
      曼哈顿
      登月
    银河护卫队
      学习雷锋
  物理学院
    IBPE
      搞事情
    瞎编的队名
======================================================
depart > select

Do you mean one of these?
select  - (sub-programme) focus on the node specified
          usage:
            select  -d | --depart    (-n --name | -m --manager)
                    -t | --team      (-n --name | -t --teacher-number)
                    -p | --project   (-i --id   | -t --team)
======================================================
depart > select --team --teacher-number
select/team > help
USAGE:
    select/team > <= (direction)    4 (value)
select/team > > 5

____    Name          20|   Manager   15|   Tele      15
____ -------------------|---------------|---------------
   1__火箭队             |  张三         |  13322224444
   2__xxx               |  xxx          |  xxxxxx

select/team > 1
=======================================================
team/火箭队 >
```


-------------------------------------------------------------

## API

#### Department Info

name        |   description        |   type         |   sample
------------|----------------------|--------------- |--------------------
name        |   院系名称            |   char[20]     |   "Computer"
manager     |   负责人             |   char[12]     |   "张三"
mobile      |   联系电话            |   char[15]     |   "13312345678"
team_num*   |   团队总数            |   int         |   0
---         |   ---                |    ---         |    ---
next        |   下一个院系节点         |   Depart *    |   NULL
child_team_head |   团队链表中该院的第一个节点  |   Team *    |   NULL
child_team_tail |   团队链表中该院的最后一个节点  |   Team *  |   NULL

```C
    /**** POST | DELETE | PUT ****/

Depart *appendDepart(Depart *head, DepartData new_one);
/*  录入院系
 *  ARGS:   链表头，已有数据的作为buffer的DepartData实例
 *  RETN:   新增节点的地址
 */

int modifyDepart(Depart *target, DepartData new_one);
/*  修改院系信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

int removeDepart(Depart **phead, Depart *target);
/*  删除院系节点
 *  ARGS:   指向院系链表头节点地址的指针，目标地址 | NULL
 *  RETN:   success code
 */

DepartData initDepartData(void);
/*  创建一个院系数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */

Depart *createDepartHead(void);
/*  创建并初始化头节点
 *  ARGS:   void
 *  RETN:   头节点地址 || NULL
 */


    /**** SELECT ****/

DepartWrapper *getDepartByManager(Depart *, Depart *, const char *);
/*  通过负责人姓名查找院系
 *  ARGS:   院系链表，搜索结束点，院系负责人 char[12]
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   院系负责人不同名，返回的是一个院系的数据，或者也可以像下面的这个函数一样，返回结果链
 *  NOTE:   只能在院系链有数据的情况下调用该函数！
 *  NOTE:   该函数会申请DepartWrapper占用空间，记得调用cleanupDepartWrapper()
 */

DepartWrapper *getDepartByName(Depart *, Depart *, const char *);
/*  通过院系名称查找院系
 *  ARGS:   院系链表，搜索结束点，名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   由于查找结果可能不只有一个，该操作会创建一个用于储存查询结果的新链表，返回链表的头节点地址
 *  NOTE:   该函数会申请DepartWrapper占用空间，记得调用cleanupDepartWrapper()
 */

Depart *getPrevDepart(Depart *cur, Depart *head);
/*  获得当前院系节点的前一个节点
 *  ARGS:   当前节点，院系链表头
 *  RETN:   前一个节点 || NULL
 */

    /**** CLEANUPs ****/

void cleanupDepartWrapper(DepartWrapper *start);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 *  NOTE:   每次搜完了记得调一次啊。。。
 *  NOTE:   调用后传进来的那个节点也没了！
 */

void cleanupDepart(Depart *start);
/*  释放Depart链所占用的内存空间
 *  ARGS:   头节点地址
 *  RETN:   void
 */
```


#### Team Info

name        |   description         |   type        |   sample
------------|-----------------------|---------------|--------------------
name        |   团队名称            |   char[30]    |   "网络安全"
manager     |   负责人             |   char[12]    |   "李四"
teacher_num |   教师人数            |   int         |   5
student_num |   研究生人数           |   int         |   20
faculty     |   所属院系            |   char[20]    |   "Computer"
project_num*|   该团队所管理项目的总数 |   int         |   123
---         |   ---                 |   ---         |   ---
parent_depart   |   该团队所属院系的节点  |   Depart *    |   NULL
next        |   下一个团队节点         |   Team *      |   NULL
child_project_head  |   项目链表中该团队的第一个节点  |   Project *   |   NULL
child_project_tail  |   项目链表中该团队的最后一个节点 |   Project *   |   NULL


```C
    /**** POST | DELETE | PUT ****/

TeamData initTeamData(void);
/*  创建一个团队数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */


Team *appendTeam(Team *head, TeamData new_one, Depart *depart_chain);
/*  录入团队
 *  ARGS:   链表头，已有数据的作为buffer的TeamData实例，母结点链（院系链）
 *  RETN:   新增节点的地址
 */

int modifyTeam(Team *target, TeamData new_one);
/*  修改团队信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

int removeTeam(Team **phead, Team *tgt);
/*  删除团队节点
 *  ARGS:   指向团队链表头节点地址的指针，目标地址 | NULL
 *  RETN:   success code
 */

Team *createTeamHead(void);
/*  创建并初始化头节点
 *  ARGS:   void
 *  RETN:   头节点地址 || NULL
 */



    /**** SELECT ****/

TeamWrapper *getTeamByTeacherNum(Team *, Team *, const Where cond);
/*  通过教师数量查找团队
 *  ARGS:   团队链表，搜索结束点，查找条件
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   调用过程中会为TeamWrapper申请内存空间，使用完搜索结果后记得cleanup
 */

TeamWrapper *getTeamByName(Team *, Team *, const char *);
/*  通过团队名称查找团队
 *  ARGS:   团队链表，搜索结束点，团队名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   调用过程中会为TeamWrapper申请内存空间，使用完搜索结果后记得cleanup
 */


    /**** CLEANUPs ****/

void cleanupTeamWrapper(TeamWrapper *start);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 *  NOTE:   每次搜完了记得调一次啊。。。
 *  NOTE:   调用后传进来的那个节点也没了！
 */

void cleanupTeam(Team *start);
/*  释放Team链所占用的内存空间
 *  ARGS:   头节点地址
 *  RETN:   void
 */
```




#### Project Info

name        |   description         |   type        |   sample
------------|-----------------------|---------------|--------------------
id          |   项目编号            |   char[15]    |   "0121210044"
type        |   项目类别            |   char        |   ['1'-'5'] (*[1])
start_date  |   起始时间            |   char[8]     |   "2015/03"
funding     |   项目经费            |   float           |   23.5 (x 1000 CNY)
manager     |   项目负责人           |   char[12]    |   "王五"
team        |   所属团队            |   char[30]    |   "网络安全"
---         |   ---                 |   ---         |   ---
parent_team |   该项目负责团队的节点  |   Team *      |   NULL
next        |   下一个项目节点         | Project *     |   NULL

```C
    /**** POST | DELETE | PUT ****/

ProjectData initProjectData(void);
/*  创建一个项目数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */

Project *appendProject(Project *head, ProjectData new_one, Team *team_chain);
/*  录入项目
 *  ARGS:   项目链表头，新增节点数据域模板，母结点链（团队链）
 *  RETN:   新增节点的地址
 */

int modifyProject(Project *tgt, ProjectData new_one);
/*  修改项目信息
 *  ARGS:   目标地址，已经修改的数据域
 *  RETN:   success code
 */

int removeProject(Project **phead, Project *tgt);
/*  删除项目节点
 *  ARGS:   指向团队链表头节点的指针，目标地址 | NULL
 *  RETN:   success code
 */

Project *createProjectHead(void);
/*  创建并初始化头节点
 *  ARGS:   void
 *  RETN:   头节点地址 || NULL
 */


    /**** SELECT ****/

ProjectWrapper *getProjectById(Project *head, Project*, const char *id);
/*  通过id查找项目
 *  ARGS:   项目链表，搜索结束点，目标id
 *  RETN:   搜索结果挂载点 | NULL （没有搜索结果时也返回挂载点地址）
 */

ProjectWrapper *getProjectByTeam(Team *parent_team);
/*  通过团队查找项目
 *  ARGS:   目标团队节点
 *  RETN:   搜索结果挂载点 || NULL （没有搜索结果也返回挂载点地址）
 */


    /**** CLEANUPs ****/

void cleanupProjectWrapper(ProjectWrapper *start);
/*  清空搜索结果序列
 *  ARGS:   头节点地址
 *  RETN:   void
 */

void cleanupProject(Project *start);
/*  释放Project链所占用的空间
 *  ARGS:   头节点地址
 *  RETN:   void
 */
```


##### \*[1] project.type

code (char)   |   name
--------------|------------------
'1'           |   973计划项目
'2'           |   国家自然科学基金项目
'3'           |   863计划项目
'4'           |   国际合作项目
'5'           |   横向项目


#### 具体实现的简单展示
详见`./utils/data_structure.c`
```
Department_HEAD --> ... <--> Department <--> next_Department <--> ...
                             |
                             +---------------------------------+-----------------------+
                                                               |                       |
Team_HEAD --------> ... <--> Team_from_another_Department <--> Team_1 <--> Team_2 <--> ...
                                                               |
                             +-----------------------+---------+
                             |                       |
Project_HEAD -----> ... <--> Proj_1 <--> Proj_2 <--> ...
```

-------------------------------------------------------------



#### 不定长搜索结果序列实现方式
```C
typedef struct _DepartWrapper {
    /* 用来储存搜索结果 */
    struct _Depart          *depart;    // 指向符合条件的节点
    struct _DepartWrapper   *next;      // 指向下一个结果wrapper
} DepartWrapper;
```

### 搜索条件

name        |   description         |   type        |   sample
------------|-----------------------|---------------|--------------------
direction   |   查询方向             |   char[3]     |   /[\\<\\>]=?/ or /=/
value       |   查询阈值             |   int         |   1234

#### 用于加速统计过程的数据结构
```C
typedef struct _DepartStatWrapper {
    /* 用来储存人数统计信息 */
    struct _Depart              *deapart;
    int                         student_num;
    int                         teacher_num;
    struct _DepartStatWrapper   *next;
} DepartStatWrapper;
```
