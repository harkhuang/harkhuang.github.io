---
layout: post
title: "dll 和 lib 本质区别"
date: 2019-06-26
tags: c++
---


最近工作接触到到内容偏向windows内容比较多,由于很久没有构件windows工程了对于其内部库工作机制已经忘的差不多了,难免要在工作中走些弯路.


windows工程属性分成

|  选项| 类别 |
| --- | --- |
|debug|release|
|x64|x32|
|动态库|静态库|
|xx|xxxx|


64位环境不要运行32位库
32位环境不要运行64位库
debug版本不要依赖release库
boost库windows下位只是编译出来了release库




通常到动态库链接都依赖一个lib文件里面包含了 动态库路径



但是运行起来又依赖动态库是什么鬼,动态库文件必须和可执行文件放到一起么.  有没有别都方法?




2019年7月15日 15:41:53

成功的被windows的宏  预定义宏  坑了一次 
之前在编译 使用pb的时候  使用宏来定义是否导入库 


这次在使用log4z中因为定义了WIN32  我就在预定义宏中加了这个代码  然后编译过程中 因为使用的boost是64位的  所以  一顿报错  嗷嗷报错 

用u版进行测试 没有出问题 已换到windows 中 使用  asio  socket   connect  就报错  <错误内容是  句柄为空>   **句柄为空**

为什么会是句柄为空?  而在编译过程中



````
#ifdef WIN32
#include <io.h>
#include <shlwapi.h>
#include <process.h>
#pragma comment(lib, "shlwapi")
#pragma comment(lib, "User32.lib")
#pragma warning(disable:4996)

#else
...
#endif
`````


我在使用这部分代码时候在win64 release vs2015编译报错
于是我在预定义宏加入了WIN32  导致在运行时  boost库报错  
#pragma comment(lib, "shlwapi")
#pragma comment(lib, "User32.lib")
#pragma warning(disable:4996)
这段代码  具体起到什么作用?
32位系统?
目前 我把宏 #ifdef WIN32改成了#ifdef _WIN32  都没有问题了
 

# 参考文章

