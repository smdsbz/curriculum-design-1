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
---         |   ---                |    ---         |    ---
next        |   下一个院系节点         |   Depart *    |   NULL
child_team_head |   团队链表中该院的第一个节点  |   Team *    |   NULL
child_team_tail |   团队链表中该院的最后一个节点  |   Team *  |   NULL

```C
Depart *appendDepart(Depart *tail, Depart new_one);
int modifyDepart(Depart *target, Depart new_one);
int removeDepart(Depart *target);
Depart *getDepartByManager(const char *);
Depart *getDepartByName(const char *);
Depart initDepart(void);
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
