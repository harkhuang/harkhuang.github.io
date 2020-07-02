---
layout: post
title: "ZeroMq-(基于消息队列的网络库)"
date: 2019-06-14
tags: c++
---

如何快速的阅读掌握一门新的技术栈,
官方文档的阅读必不可???,当然也要配合自己的实践代???

官方文档过于刻板  还是源码中的用例 来得更加直接 部分函数只有在实际调试过用例才知道其真正的含???. 一点小的学习记???



高阶->基准测试,对功能和性能有深刻理???



// 建立复杂的消息路由
ZMQ_EXPORT int zmq_proxy (void *frontend, void *backend, void *capture);
ZMQ_EXPORT int zmq_proxy_steerable (void *frontend, void *backend, void *capture, void *control);

```
int zmq_proxy (const void *frontend, const void *backend, const void *capture);*
该api用于建立一个代理.在调用该api之前需要连接和建立 frontend, backend套接字

// capture 捕获函数回调指针  , 自定义路由函数

```
    void *ctx;

    void *fend = zmq_socket(ctx, ZMQ_ROUTER);
    zmq_bind(fend_sock, CCONFIG::GetInstance()->m_query_gateway_addr.c_str()) 
 

    void *bend = zmq_socket(ctx, ZMQ_DEALER);
    zmq_bind(bend, CCONFIG::GetInstance()->m_query_dealer_addr.c_str()
   
    zmq_proxy(fend, bend, nullptr);

```




zeromq 文章提到的模式 可按照通信种类  大致分为两种

1 . 异步    router  为异步提供了数据指向.(但是我觉得概括并不准确)
            router + req 也能提供服务   为多个req提供服务
            异步通信最重要的是记录线程id 和  发现线程的状态(内部实现通过掩码+信号方式)
            理解大部分是通过我对zmq猜测的.内部实现并没有参考实际代码.  阅读请谨慎,本文并非高清无码.

2.  同步   dealer 均衡分发事件 




// 建立代理后   ZMQ_ROUTER  或者使用ZMQ_REQ同样也是标记来源
// ZMQ_DEALER模式可以被均衡的分配给后面的执行函数进行回调


# 参考文???
* [zeromq-中文指南](https://github.com/anjuke/zguide-cn)
* [zeromq-官网](https://zeromq.org)
* [zeromq研究与分析](https://www.cnblogs.com/rainbowzc/p/3357594.html)

 
