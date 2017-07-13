# curriculum-design-1
大一下课程设计 - 科研项目信息管理系统


## 须知
由于是课程设计项目，该项目遵循`GNU AGPL v3.0`许可。  
**借鉴之前请联系本人！！！**

-----------------------------------------------------------


## Features

### 数据维护
- 院系基本信息维护
    - [ ] 录入
    - [ ] 修改
    - [ ] 删除
- 科研团队基本信息维护
    - ...
- 科研项目基本信息维护
    - ...


### 数据查询

#### 关键字
_(院系负责人、科研团队不同名)_

- 院系
    - [ ] 院系负责人
    - [ ] 院系名称的全部或一部分

- 科研团队
    - [ ] 团队名称的全部或一部分
    - [ ] 教师人数满足条件（大于|小于|等于）

- 科研项目
    - [ ] 项目编号
    - [ ] 所属团队



### 数据统计

- 人数统计
    - [ ] 教师总数
    - [ ] 研究生总数
    - [ ] 研究生与教师的人数比（保留两位小数）
        - [ ] 降序排序，输出结果
- 项目统计
    - [ ] 某年度各院系科研项目数、973项目数、863项目数、科研总经费
        - [ ] 按科研项目数降序排序，输出结果
- 历年来类别为国家自然科学基金的科研项目数最多的10个科研团队
    - [ ] 按项目数降序排序，输出科研团队名称、国家项目数、项目总经费
- 科研项目数最高的5个科研团队
    - [ ] 按比值降序排序，输出团队名称、教师人数、科研项目数、项目数和教师人数比值
- 教师人数的比值最高的5个科研团队
    - [ ] 按比值降序排序，输出团队名称、教师人数、科研项目数、项目数和教师人数比值
- 其他有关科研项目方面的数据统计


-------------------------------------------------------------

## API

#### Department Info

name        |   description        |   type         |   sample
------------|----------------------|--------------- |--------------------
name        |   院系名称            |   char[20]     |   "Computer"
manager     |   负责人             |   char[12]     |   "张三"
mobile      |   联系电话            |   char[15]     |   "13312345678"
team_num    |   团队总数            |   int         |   0
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

    /**** SELECT ****/

DepartWrapper *getDepartByManager(Depart *, const char *);
/*  通过负责人姓名查找院系
 *  ARGS:   院系链表，院系负责人 char[12]
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   院系负责人不同名，返回的是一个院系的数据，或者也可以像下面的这个函数一样，返回结果链
 *  NOTE:   只能在院系链有数据的情况下调用该函数！
 *  NOTE:   该函数会申请DepartWrapper占用空间，记得调用cleanupDepartWrapper()
 */

DepartWrapper *getDepartByName(Depart *, const char *);
/*  通过院系名称查找院系
 *  ARGS:   院系链表，名称线索（不一定是全称）
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   由于查找结果可能不只有一个，该操作会创建一个用于储存查询结果的新链表，返回链表的头节点地址
 *  NOTE:   该函数会申请DepartWrapper占用空间，记得调用cleanupDepartWrapper()
 */

DepartData initDepartData(void);
/*  创建一个院系数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
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
---         |   ---                 |   ---         |   ---
parent_depart   |   该团队所属院系的节点  |   Depart *    |   NULL
next        |   下一个团队节点         |   Team *      |   NULL
child_project_head  |   项目链表中该团队的第一个节点  |   Project *   |   NULL
child_project_tail  |   项目链表中该团队的最后一个节点 |   Project *   |   NULL

```C
    /**** POST | DELETE | PUT ****/

Team *appendTeam(Team *head, Team *tail, TeamData new_one);
/*  录入团队
 *  ARGS:   链表头，链表尾，已有数据的作为buffer的TeamData实例
 *  RETN:   新增节点的地址
 */

int modifyTeam(Team *target, TeamData new_one);
/*  修改团队信息
 *  ARGS:   目标地址，已经修改数据的buffer
 *  RETN:   success code
 */

int removeTeam(Team **phead, Team *target);
/*  删除团队节点
 *  ARGS:   指向团队链表头节点地址的指针，目标地址 | NULL
 *  RETN:   success code
 */


    /**** SELECT ****/

TeamWrapper *getTeamByManager(Team *, const char *, TeamWrapper *, Depart *depart_chain);
/*  通过负责人姓名查找团队
 *  ARGS:   团队链表，团队负责人 char[12]，搜索结果挂载点，团队链表头
 *  RETN:   搜索结果挂载点 | NULL （没有结果时也返回挂载点地址）
 *  NOTE:   调用过程中会为TeamWrapper申请内存空间，使用完搜索结果后记得cleanup
 */

TeamWrapper *getTeamByTeacherNumber(Team *, const char *, TeamWrapper *, Depart *depart_chain);
/*  通过团队名称查找团队
 *  ARGS:   团队名称线索（不一定是全称），团队链表头
 *  RETN:   搜索结果挂载点 | NULL
 *  NOTE:   调用过程中会为TeamWrapper申请内存空间，使用完搜索结果后记得cleanup
 */

TeamData initTeamData(void);
/*  创建一个团队数据的原型
 *  ARGS:   void
 *  RETN:   根据在该函数执行过程中输入的数据所创建出来的原型
 *  NOTE:   will trigger input action
 */
```




#### Project Info

name        |   description         |   type        |   sample
------------|-----------------------|---------------|--------------------
id          |   项目编号            |   char[15]    |   "0121210044"
type        |   项目类别            |   char        |   [1-5] (*[1])
start_date  |   起始时间            |   char[8]     |   "2015/03"
funding     |   项目经费            |   float           |   23.5 (x 1000 CNY)
manager     |   项目负责人           |   char[12]    |   "王五"
team        |   所属团队            |   char[30]    |   "网络安全"
---         |   ---                 |   ---         |   ---
parent_team |   该项目负责团队的节点  |   Team *      |   NULL
next        |   下一个项目节点         | Project *     |   NULL

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
