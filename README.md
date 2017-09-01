# curriculum-design-1
大一下课程设计 - 科研项目信息管理系统


## 须知
由于是课程设计项目，该项目遵循`GNU AGPL v3.0`许可。  
**借鉴之前请联系本人！！！**

-----------------------------------------------------------


## Features

*see `./Requirement_Documentation/`*

-------------------------------------------------------------

## File Structure

```
./PROGRAM_ROOT
    +-- utils/
    |   +-- data/                   // 测试用数据文件夹
    |   |   +-- DEPART.DAT
    |   |   +-- TEAM.DAT
    |   |   +-- PROJECT.DAT
    |   +-- __init__.h              // 所有数据处理函数的申明
    |   +-- data_structure.h        // 数据结构
    |   +-- faculty_functions.h     // 院系函数
    |   +-- faculty_functions.c
    |   +-- team_functions.h        // 团队函数
    |   +-- team_functions.c
    |   +-- project_functions.h     // 项目函数
    |   +-- project_functions.c
    |   +-- io_functions.h          // 数据载入、存储函数
    |   +-- io_functions.c
    |   +-- makefile                // 单元测试用
    +-- doc_strings.h               // 用户帮助提示
    +-- views.h                     // 视图函数申明
    +-- main.c                      // 编译起点
    +-- makefile                    // 完整编译用
    |
    +-- py-extended/                // black magic!
```

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

## User Manual (for CLI ver.)

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

-------------------------------------------------------------

## API

*see `.h` files*

-------------------------------------------------------------

## WebApp Interface

*step forward to `./py-extended`*
