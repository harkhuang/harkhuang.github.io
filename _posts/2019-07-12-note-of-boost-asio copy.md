
### boost asio 官方文档 1.7 最新
### 包含 入门 简介  用例  简单有效
https://www.boost.org/doc/libs/1_70_0/doc/html/boost_asio.html


### 用例
https://www.boost.org/doc/libs/1_70_0/doc/html/boost_asio/examples.html

包含知识点比较多

异步   lambda表达式 事件封装  多线程io切换 


### 横向比较libuv和asio区别,类比方式非常值得学习
https://stackoverflow.com/questions/11423426/how-does-libuv-compare-to-boost-asio


一句话Call Chains vs. Watcher Loops 概括区分的区分事件触发内部机制 也是StackOverflow高赞的回答




### c11里面有非常多的特性  
rvalue 是最核心的概念  整个后面的改革  和 声明周期的重整都是基于右值声明周期来做的定义和改善

比如 我们可以通过move 来延长对象的声明周期

比如 我们可以返回多种数据类型 而不是仅仅的int

比如 在lambda表达式中使用捕获对参数带入和传出参数


顺便思考个问题  boost::asio中异步事件中的数据返回 如何做到通知的效果呢?
回调再回调吗,由于对象声明周期并没有那么灵活  不同于c语言 只要地址给你了 你就可以基于这块地址构建自己程序需要的内存布局,精心构建的布局对于程序运行非常必要, 既要节省内存,又要使得程序灵活调用.

c++采用生命周期也就是常说的raii来管理整个声明周期和内存的分配.这是个简单有效的原则.我们虽然可以有效的利用内存,