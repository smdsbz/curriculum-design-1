/*  $PROGRAM_ROOT/doc_strings.h
 *  用户界面字符串
 */

#ifndef _DOC_STRING_H
#define _DOC_STRING_H

// 欢迎界面
char *START = "========================================================\n\
  \\ +---------+  +---------+**+-------+**\\ +---------+\n\
  *\\|  +------+  |  +---+  |\\*|        \\**\\|  +------+\n\
  **|  |   \\**\\--|  |-\\ |  |*\\|  ++  +  |**|  |\n\
  --|  +------+--|  +---+  |**|  ||  |  |--|  +------+\n\
    +------+  |\\ |  +------+**|  ||  |  |  +------+  |\n\
  --------\\|  |*\\|  |      \\**|  ||  |  |--------\\|  |\n\
  \\*+------+  |\\*|  |       \\*|  ||  |  |\\*+------+  |\n\
   \\+---------+ \\+--+        \\+--++--+--+ \\+---------+    by smdsbz\n\
========================================================\n\
The Science Project Managing System\n\
Version: 0.1\n\
Author: smdsbz@GitHub.com (i.e. U201610136 朱晓光)\n\
Enjoy your ride!\n\
";

// 顶层
char *DOC_ROOT = "Operations Available:\n\
     0) Save and quit\n\
     1) List out all records\n\
     2) Query utilities\n\
     3) Statistics utilities\n\
     4) Add a new record\n\
     5) Back-up data\n\
";

// 查询对象选择
char *DOC_QUERY_OBJ = "Query Objects:\n\
     0) Go back\n\
     1) Departments\n\
     2) Teams\n\
     3) Projects\n\
";

// 查询院系方法
char *DOC_QUERY_DEPART_METHOD = "Query Methods:\n\
     0) Go back\n\
     1) By name\n\
     2) By manager\n\
";

// 查询团队方法
char *DOC_QUERY_TEAM_METHOD = "Query Methods:\n\
     0) Go back\n\
     1) By name\n\
     2) By teacher number\n\
";

// 添加记录类型选择
char *DOC_ADD_TYPE = "Add object:\n\
     0) Go back\n\
     1) Department\n\
     2) Team\n\
     3) Project\n\
";

// 院系操作
char *DOC_DEPART = "Operations Available:\n\
     0) Unfocus\n\
     1) List info\n\
     2) Modify\n\
     3) Delete\n\
     4) List teams\n\
";

// 选择修改院系属性
char *DOC_DEPART_MODIFY = "Modify Attribute:\n\
     0) Go back\n\
     1) Manager\n\
     2) Telephone\n\
";

// 团队操作
char *DOC_TEAM = "Operations Available:\n\
     0) Unfocus\n\
     1) List info\n\
     2) Modify\n\
     3) Delete\n\
     4) List projects\n\
     5) Focus on parent department\n\
";

// 选择修改团队属性
char *DOC_TEAM_MODIFY = "Modify Attribute:\n\
     0) Go back\n\
     1) Manager\n\
     2) Teacher number\n\
     3) Student number\n\
";

// 项目操作
char *DOC_PROJECT = "Operations Available:\n\
     0) Unfocus\n\
     1) List info\n\
     2) Modify\n\
     3) Delete\n\
     4) Focus on parent team\n\
";

// 选择修改项目属性
char *DOC_PROJECT_MODIFY = "Modify Attribute:\n\
     0) Go back\n\
     1) Type\n\
     2) Funding\n\
     3) Manager\n\
";

// 选择统计
char *DOC_STAT = "Statistics Tools Available:\n\
     0) Go back\n\
     1) Human Resource\n\
     2) Project Overview\n\
     3) Top 10 NSFC Teams\n\
     4) Top 5 Project/Teacher Ratio Teams\n\
     5) Top 5 Average Project Funding Departments\n\
";

#endif
