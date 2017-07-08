# curriculum-design-1

## What I've learnt

#### 多文件编译

- 防止重复引入头文件的方法

```C
/* header_file.h */
#ifndef HEADER_FILE
#define HEADER_FILE

// everything you need

#endif
```


- 返回函数的函数的申明方式

```C
/* $PROJ_ROOT/utils/data_structure.c:174 */
int (*setJudger(const char *))(int, int);
/*  接收 const char * 型参数
 *  返回 int (*)(int, int) 型函数
 */
```
