---
layout: post
title:  "多线程安全的队列的思考"
date:   2020-04-29 09:30:51 +0800
categories: jekyll update 
desc: " 多线程消费模型，消息队列的设计"
keywords: "thread , MessageQueue"
categories: [LINUX]
tags: [技术,并发，多线程]
icon: icon-html
---





队列再熟悉不过了,使用队列也是为了多线程程序之间解除耦合性
一切工作任务交给队列.

列举集中实现方式
1.带锁的队列

2.一读一写的队列,提供原子变量

3.自旋锁队列

4.无锁队列

5.线程安全队列,基于c11语法新特性实现



锁的级别
无锁、偏向锁、轻量级锁、重量级锁
