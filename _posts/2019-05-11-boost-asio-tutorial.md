---
layout: post
title:  " boost aiso tutorial "
date:   2019-05-08 15:16:51 +0800
categories: jekyll update
---

boost :: asio入门指南

条目由Drew_Benton发布 ·2011年1月31日

 

boost :: asio 是“用于网络和低级I / O编程的跨平台C ++库，它使用现代C ++方法为开发人员提供一致的异步模型。” 它目前拥有许多用户，是图书馆推动系列的一部分。

在开始之前，我们将要阅读boost :: asio 概述。它涵盖了许多有用的信息，我们应该首先了解它的基础知识。本指南并不是要教授boost :: asio库的完整细节; 这就是文档的用途！本指南将指导我们学习图书馆的实用方向，并开始在我们自己的应用程序中使用它。

另外，我们可能还想查看一下 boost :: asio作者的博客也是如此。这里有大量有用的信息以及作者的许多有意识的帖子。另一个很好的参考网站就是这个。它实际上是一本已在网上发布的书，所以考虑支持该作者的辛勤工作！

此时，需要解释本指南的目的。本指南不会教任何编程语言。读者必须已经熟悉C ++才能充分利用boost库，特别是boost :: asio。本指南一般不会教授网络编程。读者应该已经理解了这个话题。本指南将帮助读者开始使用boost :: asio库进行实际的网络编程。换句话说，本指南的目标是引导读者沿着学习最有用的学习boost :: asio的学习路径。读者仍然需要花时间阅读官方文档，并在为项目使用新库时进行其他必要的研究。

另外需要注意的是，这些示例仅在Windows上编译。虽然已经努力使它们大部分都是可移植的，但是有一些可能需要一些逻辑更改才能在其他平台上编译或使用其他编译器。所有源示例都已压缩并附加。没有包含项目文件，只包含实际的源文件。如果我们不想复制和粘贴示例，这是为了使查看代码更方便。必须正确设置和安装Boost才能使用它们！我们可以参考主要增强网站获取此类说明。

最后，本指南是根据我个人的观点和经验编写的。虽然我努力确保没有传播任何错误信息，但有时也会犯错误。如果您在此域中也有经验，请指出任何错误或不一致，我们将很乐意纠正它们。

如果没有进一步的说明，让我们开始吧！

1. io_service的基础知识
2. 了解boost :: bind
3. 为io_service做一些工作
4. 使用strand
5. 序列化我们的工作负载。错误处理
6. 定时器
7. 网络基础：连接器和接受器（TCP）
8. 网络基础：二进制协议发送和接收（TCP）
9. 一个boost :: asio网络包装器（TCP）
10. 前面的道路

[页面]

1. io_service

的基础知识 boost :: asio的核心对象是io_service。这个对象就像大脑和图书馆的核心。我们将从一个简单的例子开始，以熟悉它。在这个例子中，我们将调用run 成员函数。如果我们检查函数的文档，“run（）函数会阻塞，直到所有工作都完成，并且没有更多的处理程序要调度，或者直到io_service被停止。”

例1a

```
int main( int argc, char * argv[] )
{
	boost::asio::io_service io_service;
	io_service.run();
	std::cout << "Do you reckon this line displays?" << std::endl; //??
	return 0;
}
```

根据文档的说法，我们应该期待显示文本行，对吧？我的意思是我们并没有真正明确地给它做任何事情，所以除非在我们不知道的幕后发生什么事情，否则该功能不应该阻止。如果我们运行该程序，我们会得到预期的结果; 我们看到了文字。

此示例可能已为某些读者设置了警报。如果我们的计划失效怎么办？这根本没用; boost :: asio绝对不适合我！ 不是那么快的伙伴，让我们不要这么快就超越自己。boost :: asio的开发人员也考虑过这一点。为了解决这个问题，他们创作了一部作品 类。工作类是“在工作时要通知io_service的类”。换句话说，只要io_service有一个与之关联的工作对象，它就永远不会用完。要测试这一点，请考虑下一个示例。

示例1b
[扰流器] [/扰流器] 如果我们运行该示例，我们将再次获得预期结果。我们没有看到文本和程序没有退出。不幸的是，我们现在无法使用我们现在知道的工具执行优雅的退出。有很多方法可以解决这个问题，但是我们暂时不会介绍它们，因为我们现在只是弄湿了脚。


```
int main( int argc, char * argv[] )
{
	boost::asio::io_service io_service;
	boost::asio::io_service::work work( io_service );
	io_service.run();
	std::cout << "Do you reckon this line displays?" << std::endl;
	return 0;
}

```




从这些示例中，我们已经可以看到使用boost :: asio可以实现的两种不同的设计方法。还有很多很多！如果我们不喜欢这种必须阻止线程做工作的想法怎么办？如果我们希望能够随时随地工作，该怎么办？这甚至可能吗？答案是肯定的！

在下一个示例中，我们将简单地模拟循环并调用io_service 的poll函数。poll函数“运行io_service对象的事件处理循环以执行就绪处理程序。”

示例1c
[扰流器] [/扰流器] 如果我们运行该示例，我们将看到输出到控制台的42行文本，然后程序退出。如果我们将工作对象分配给io_service怎么办？这种行为会改变吗？

```
int main( int argc, char * argv[] )
{
	boost::asio::io_service io_service;
	for( int x = 0; x < 42; ++x )
	{
		io_service.poll();
		std::cout << "Counter: " << x << std::endl;
	}

	return 0;
}

```



示例1d
[扰流器] [/扰流器] 当我们运行此程序时，我们得到与以前完全相同的输出和结果。这是因为当有更多工作要做时，poll函数不会阻塞。它只是执行当前的工作集然后返回。在实际程序中，循环将基于其他一些事件，但为了简单起见，我们只使用固定的一个。

```
int main( int argc, char * argv[] )
{
	boost::asio::io_service io_service;
	boost::asio::io_service::work work( io_service );
	for( int x = 0; x < 42; ++x )
	{
		io_service.poll();
		std::cout << "Counter: " << x << std::endl;
	}
	return 0;
}

```



这个例子讲述了工人阶级如何在幕后操作。想象一下，如果工作对象以io_service调用的工作处理程序内部添加新工作的方式为io_service对象提供工作。在这种情况下，由于总是会添加新工作，因此民意调查永远不会耗尽。然而，事实显然并非如此。工作被添加到处理程序之外，因此一切都将按预期工作。

大！我们现在可以根据我们需要的程序设置选择使用run和poll函数。要添加更多灵活性，请运行run_one和poll_one 功能已创建。这些允许程序员根据需要微调他们的程序。在这一点上，我们需要退后一步，考虑到目前为止我们所知道的。为了让io_service为我们工作，我们必须使用run或poll系列函数。如果我们为poll工作对象分配工作对象，则run将阻塞并等待工作。实质上，函数的名称与其功能相匹配。

还有一个小松散的结局，我们需要绑定。如果我们想从io_service中删除工作对象怎么办？通过文档查看，似乎没有提供这样做的功能。为了实现此功能，我们必须使用指向工作对象的指针。保持boost库，我们将使用shared_ptr，一个智能指针类。

例1e
[剧透] [/剧透] 如果我们运行程序，我们将看到显示的文本行。这有效地向我们展示了如何从io_service中删除工作对象。在我们希望优雅地完成所有待处理工作但不能过早停止工作的情况下，此类功能非常重要。虽然有一点需要注意，但有一点需要注意。

```
int main( int argc, char * argv[] )
{
	boost::asio::io_service io_service;
	boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( io_service ));

	work.reset();

	io_service.run();

	std::cout << "Do you reckon this line displays?" << std::endl;

	return 0;
}

```



现在我们知道如何用一个线程驱动io_service，我们需要弄清楚更多线程需要什么。io_service文档页面告诉我们“多个线程可以调用run（）函数来设置一个线程池，io_service可以从中执行处理程序。在池中等待的所有线程都是等效的，io_service可以选择任何一个他们调用一个处理程序。“ 听起来很容易吧？由于boost库还提供了一个线程库，我们将在下一个示例中使用它。

例1f
[扰流器] [/扰流器] 这个例子介绍了停止

```
boost::asio::io_service io_service;

void WorkerThread()
{
std::cout << "Thread Start\n";
io_service.run();
std::cout << "Thread Finish\n";
}

int main( int argc, char * argv[] )
{
    boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( io_service ));
    std::cout << "Press [return] to exit." << std::endl;
    boost::thread_group worker_threads;
    for( int x = 0; x < 4; ++x )
    {
        worker_threads.create_thread( WorkerThread );
    }

    std::cin.get();

    // 停止asio集成服务
    io_service.stop();


    // 异步回收线程资源
    worker_threads.join_all();

    return 0;
}

```

会员功能。停止功能将向io_service发出应该停止所有工作的信号，因此在当前一批工作完成后，将不再进行任何工作。此示例中的另一个更改是io_service对象现在已成为全局对象。这只是为了保持简单，因为必须使用更复杂的机制。如果我们运行程序，我们在控制台上获得4个线程启动消息，在我们点击返回之后，我们按预期获得四个线程完成消息。

真正应该突出的是使我们的线程程序扩展是多么简单和容易。通过简单地添加更多工作线程，我们可以通过io_service对象支持越来越多的并发处理工作。如前所述，如果我们将工作对象与io_service相关联并希望让所有排队的工作完成，我们就不会调用stop而是销毁工作对象。但是必须要小心。如果我们希望所有工作都能完成但是继续为io_service提供更多的事情，那么它永远不会退出！在这种情况下，在某些时候，我们希望调用stop函数来确保系统实际停止。

现在我们已经了解了驱动io_service对象的不同方法，我们可以继续学习下一组我们必须学习的主题，然后才能真正完成实际工作。



2.了解boost :: bind

在我们看看如何让io_service真正开展工作之前，我们需要建立对另一个boost库boost :: bind的基本理解。我第一次看到boost :: bind时我不知道它是怎么回事，并且真的不知道为什么需要这样的库。在使用boost :: asio之后，我可以很容易地看到并欣赏库的有用性。在这个时候，我建议读者稍微阅读一下boost :: bind文档，以便在继续之前了解库的功能。

使用boost :: bind包装函数调用会创建一个对象。请考虑以下示例。

示例2a
[扰流器] [/扰流器] 如果我们运行程序，我们将看不到输出。这是因为我们创建了一个函数调用对象，但实际上并没有调用它。要调用它，我们只需使用对象的（）运算符。示例2b [扰流器] [/扰流器] 现在，当我们运行示例时，我们看到输出！如果我们有参数通过怎么办？添加它们也很容易。例2c [剧透] [/剧透]

```
void F1()
{
std::cout << __FUNCTION__ << std::endl;
}

int main( int argc, char * argv[] )
{
boost::bind( &F1 );
return 0;
}
```

```
void F1()
{
std::cout << __FUNCTION__ << std::endl;
}

int main( int argc, char * argv[] )
{
boost::bind( &F1 )();
return 0;
}
```

```
void F2( int i, float f )
{
std::cout << "i: " << i << std::endl;
std::cout << "f: " << f << std::endl;
}
int main( int argc, char * argv[] )
{
boost::bind( &F2, 42, 3.14f )();
return 0;
}
```

如果我们运行程序，我们将看到预期的输出。我们也可以轻松地将变量值替换为变量。这里还有一些重要的事情需要注意。参数属于函数对象，不通过调用操作符传递！当我们将参数与函数绑定在一起时，我们必须完全匹配签名，否则我们将获得大量难以阅读的错误，这些错误最初难以查看。当我们使用boost :: bind获取错误时，我们需要比较函数声明和用于检查任何类型不匹配的参数。
我们在boost :: bind的崩溃过程中的最后一个例子将使用类成员函数的bind来显示。此示例与之前类似，但有一个重要区别。

例2d
[剧透] [/剧透] 我们必须传递类对象的地址才能在类函数之后调用！如果我们从类中调用bind，那么我们可以使用this指针或者随后使用shared_from_this（）如果我们的类支持它。请注意，在所有这些示例中，我们只是使用（）运算符来调用对象。实际上，如果我们接收一个boost :: bind对象来实际调用，我们只会这样做。否则，我们只需使用普通语义来调用该函数！请务必参考boost :: bind文档以获取更多信息和参考。

```
class MyClass
{
public:
   void F3( int i, float f )
   {
        out << "i: " << i << std::endl;
        std::cout << "f: " << f << std::endl;
   }
};

int main( int argc, char * argv[] )
{
    MyClass c;
    boost::bind( &MyClass::F3, &c, 42, 3.14f )();
    return 0;
}

```


现在已经快速引入了boost :: bind，我们还必须讨论另一个重要的概念。在线程boost :: asio示例中，io_service对象变为全局并移动到程序的顶部。对于任何模块化和可重复使用的代码，这是不希望的。但是，如果我们尝试将io_service与boost :: bind一起使用，我们将得到一个不可复制的错误，因为无法复制io_service，这就是boost :: bind在幕后为我们所做的。为了解决这个问题，我们必须再次使用shared_ptr。

我们必须使用io_service的shared_ptr对象并传递它，而不是使用常规的io_service对象。
shared_ptr是一个引用计数的智能指针，因此它是可复制的，因此与boost :: bind兼容。这同样适用于许多其他不可复制的对象; 如果需要，我们必须将它们包装在shared_ptrs中以传递它们。让我们使用我们新学到的概念重新审视线程示例。

例2e
[剧透]

[/剧透]

```
void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
    std::cout << "Thread Start\n";
    io_service->run();
    std::cout << "Thread Finish\n";
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));

std::cout << "Press [return] to exit." << std::endl;

boost::thread_group worker_threads;
for( int x = 0; x < 4; ++x )
{

// boost::bind 绑定过程中可以传递默认打参数 
// 而且这个参数是被正确打传递的
// 在传递过程中涉及到线程池资源共享 所以我们用的智能指针
// io_service作为WorkerThread打参数
// 在线程中调用异步阻塞
worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
}

std::cin.get();

io_service->stop();

// 异步阻塞回收资源
// 此处不回收资源那么 我们面临打就是
worker_threads.join_all();

return 0;
}
```

很酷，对吧？我们可以在io_service上使用shared_ptr使其可复制，这样我们就可以将它绑定到我们用作线程处理程序的工作线程函数。当我们运行程序时，我们应该得到与以前完全相同的行为。在这一点上，强烈建议阅读更多关于boost :: bind，shared_ptr，甚至已经涵盖的boost :: asio主题，如果他们感觉不太清楚。我们很快就会大量使用它们！

使用线程程序时，我们必须确保同步对任何全局数据或共享数据的访问。我们之前的线程示例有一个瑕疵，这个漏洞已被破解。它突出了吗？
std :: cout对象是一个全局对象。一次从不同的线程写入它可能会导致输出格式问题。为了确保我们不会遇到这些问题，因为它们阻碍了调试工作，我们将希望使用全局互斥锁。boost :: thread库为我们提供了完成此任务所需的类。

下一个示例将使用互斥对象。我们还应该阅读同步 主题也是。我们将简单地更正前面的示例以处理输出，因为它现在应该处理。另外，我们还将利用boost :: thread的线程id功能来识别我们的线程。此外，还可以在此处找到有关该功能的更多信息。

例2f

```
boost::mutex global_stream_lock;
void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
// 由于std::out 是全局唯一打竞争资源 所以此处应该加锁
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id() <<"] Thread Start" << std::endl;
global_stream_lock.unlock();

io_service->run();

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id() <<"] Thread Finish" << std::endl;
global_stream_lock.unlock();
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] Press [return] to exit." << std::endl;
global_stream_lock.unlock();

boost::thread_group worker_threads;
for( int x = 0; x < 4; ++x )
{
worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
}

std::cin.get();

io_service->stop();

worker_threads.join_all();

return 0;
}
```

了解互斥对象的基础知识也很重要。如果我们锁定一次，我们必须在完成后立即解锁。我们不能递归锁定这种特定类型的互斥锁，尽管还有其他类型允许这种情况。如果我们这样做，线程就会死锁，这是我们永远不想发生的事情。当我们必须等待全局输出锁定时，并发性的许多好处都会减少，但是为了使用正确的多线程代码，它现在是必须的。最终，我们将希望实现我们自己的自定义日志记录方案，以避免此类问题，但我们可以稍后解决该问题。

现在已经涵盖了所有先决条件。我们将来会大量使用boost :: bind库，所以在继续之前要确保概念很舒服。有关更有趣的阅读，
请查看Boost Bind库如何改进您的C ++程序和快速C ++代理：Boost.Function'drop-in'替换和多播。
Boost :: bind肯定提供了很大的灵活性，但是在使用生产代码之前，用户应该知道这个代价。

[page]

3.为io_service提供一些工作要做

现在我们终于可以做真正的工作了！我们将重用前面的示例作为基础，因此我们的示例是多线程就绪的。如果io_service是boost :: asio库的大脑和心脏，那么io_service成员函数的post和dispatch 将是arm和legs。post函数“用于请求io_service执行给定的处理程序，但不允许io_service从此函数内部调用处理程序。” 调度函数“保证只在当前调用run（），run_one（），poll（）或poll_one（）成员函数的线程中调用处理程序。如果处理程序可以在此函数内执行。保证可以得到满足。“

因此，根本区别在于，如果可以的话，调度将立即执行工作，并在排队工作时将其排队，无论如何。这两个功能都非常重要，因为我们将使用的功能将取决于它所使用的上下文。请记住前面关于工作类内部如何工作的评论？如果工作类一次又一次地使用调度，那么调查调用的工作可能永远不会完成，但如果工作调用post，它可以。

让我们开始吧！我们将从简单的Fibonacci计算开始。为了使事情变得更有趣，我们将添加一些时间延迟来显示boost :: asio的真正本质。我们还将工作线程的数量减少到2个。我们想要在多线程程序中使用的实际值将根据许多因素而变化，但稍后将对此进行讨论。

例3a
[扰流板] [/扰流板]

```
boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] Thread Start" << std::endl;
global_stream_lock.unlock();

io_service->run();

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] Thread Finish" << std::endl;
global_stream_lock.unlock();
}

size_t fib( size_t n )
{
if ( n <= 1 )
{
return n;
}
boost::this_thread::sleep(
boost::posix_time::milliseconds( 1000 )
);
return fib( n - 1 ) + fib( n - 2);
}

void CalculateFib( size_t n )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] Now calculating fib( " << n << " ) " << std::endl;
global_stream_lock.unlock();

size_t f = fib( n );

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] fib( " << n << " ) = " << f << std::endl;
global_stream_lock.unlock();
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] The program will exit when all work has finished."<< std::endl;
global_stream_lock.unlock();

boost::thread_group worker_threads;
for( int x = 0; x < 2; ++x )
{
worker_threads.create_thread(boost::bind( &WorkerThread, io_service )
);

}

//  投掷工作事件 
//  交代做什么事情就好了 
//  服务器模型已经建立好了  只要把任务清单递交(post)给服务器  服务器就会按照顺序执行您打清单 
io_service->post( boost::bind( CalculateFib, 3 ) );
io_service->post( boost::bind( CalculateFib, 4 ) );
io_service->post( boost::bind( CalculateFib, 5 ) );



// 完成清空工作集
work.reset();

//完成清理线程池资源
worker_threads.join_all();

return 0;
}
```

在这个例子中，从main开始，我们通过post函数将3个函数对象发布到io_service。
在这种特殊情况下，由于当前线程没有调用io_service run或poll函数，dispatch也会调用post函数而不立即执行代码。
在我们通过post给出io_service工作之后，我们重置工作对象，以便在我们希望退出的工作完成后发出信号。最后，我们等待所有线程完成，就像我们使用join_all函数一样。
我们的fib函数只是计算序列，但我们添加一个时间延迟来减慢速度，看看我们的工作线程在起作用！我们最终必须使用CalculateFib包装调用，因为我们关心返回值，并且我们希望看到有关函数实际启动和完成的额外调试信息。
运行程序，我们应该看到前两个工作线程从前两个工作单元开始，一旦一个工作线程完成，它将占用第三个工作单元。完成所有工作后，程序退出。
恭喜！我们现在已经完成了第一份工作！那不是很糟糕吗？我们使用boost :: asio的程序结构总体来说非常通用。我们可以设置可重用的工作线程来构建一个工作池，当我们将工作发送到io_service时，它就会尽可能地完成它。这是我们的基本例子，让我们考虑另一个。
在这个例子中，我们展示了post和dispatch之间的区别，以及如果我们不小心我们的工作，它会如何让我们陷入困境！这次我们只使用一个工作线程。

例3b
[扰流板] [/扰流板]

```
boost::mutex global_stream_lock;
void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] Thread Start" << std::endl;
global_stream_lock.unlock();
io_service->run();
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] Thread Finish" << std::endl;
global_stream_lock.unlock();
}



// 分发
void Dispatch( int x )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id() << "] "<< __FUNCTION__ << " x = " << x << std::endl;
global_stream_lock.unlock();
}

void Post( int x )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id() << "] "<< __FUNCTION__ << " x = " << x << std::endl;
global_stream_lock.unlock();
}

void Run3( boost::shared_ptr< boost::asio::io_service > io_service )
{
for( int x = 0; x < 3; ++x )
{
io_service->dispatch( boost::bind( &Dispatch, x * 2 ) );
io_service->post( boost::bind( &Post, x * 2 + 1 ) );
boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 ) );
}
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] The program will exit when all work has finished." << std::endl;
global_stream_lock.unlock();

boost::thread_group worker_threads;
for( int x = 0; x < 1; ++x )
{
worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
}

io_service->post( boost::bind( &Run3, io_service ) );

work.reset();

worker_threads.join_all();

return 0;
}
```

如果我们运行该程序，我们应该在这里看到问题。我们想要一个按顺序显示事件，但它却出了问题。这是因为dispatch用于某些事件并发布给其他事件。即使排队的其他待处理事件，也可以从当前工作线程执行调度事件。发布的事件必须等到处理程序完成才允许执行。编程时请记住这一点，如果我们依赖于此类事件的顺序，我们可以轻松地将自己编码为严重的错误！

还应该注意的是，如果我们有多个工作线程，由于睡眠调用，我们实际上会获得预期的结果，但问题仍然存在。如果我们删除了睡眠，我们可能会获得任何输出顺序，具体取决于谁首先抓住了锁互斥锁。例如，在没有睡眠的情况下运行程序的一个示例输出是0,2,1,4,3,5。在这个级别进行编程时我们必须注意这些事情，这样我们就不会被“正确”的输出所迷惑这只是以可能的方式设置我们的程序的结果。这些类型的错误一旦发生就难以追踪，因此我们必须充分了解我们在深入潜水之前首先使用的API。

这几乎是包装，没有双关语意图，我们将如何将我们的工作传递给io_service对象。我们只是设置我们的程序来通过民意调查处理工作或运行我们需要的方式，然后我们可以根据需要调用派遣或发布。现在我们可以使用这些概念做很多很酷的事情！此时，我们现在可以进入boost :: asio库的其他有用方面。

[page]

4.使用strand

来序列化我们的工作负载有时我们会想要完成工作的排队，但是完成它的顺序很重要。这股 为这种情况创建了类。strand类“提供序列化处理程序执行”。这意味着如果我们通过一个链发布work1 - > work2 - > work3，无论我们有多少个工作线程，它都将按此顺序执行。整齐！

尽管如此，强大的力量也带来了巨大 我们必须理解strand类的处理程序调用的顺序！
[quote]
处理程序调用顺序
给定：
	* 
一个链对象
	* 
对象是会议完成处理程序的要求
	* 
对象a1，它是由实现产生的任意副本
	* 
满足完成处理程序要求的对象b
	* 
如果满足以下任一条件，则对象b1是由实现生成的b的任意副本：

		* 
s.post（a）发生在s.post之前（b）
		* 
s.post（a）发生在s.dispatch（b）之前，其中后者在the strand之外执行
		* 
s.dispatch（a）发生在s.post（b）之前，其中前者在the strand之外执行
		* 
s.dispatch（a）发生在s.dispatch（b）之前，其中两者都在strand之外执行，然后asio_handler_invoke（a1，＆a1）发生在asio_handler_invoke（b1，＆b1）之前。

请注意，在以下情况中：

async_op_1（...，s.wrap（a））;
async_op_2（...，s.wrap（b））;
第一个异步操作的完成将执行s.dispatch（a），第二个将执行s.dispatch（b），但未指定执行这些操作的顺序。也就是说，你无法说明一个是否发生在另一个之前。因此，上述条件均不符合，且未作出订购保证。[/ quote]

在使用strand类时，我们必须了解这些条件。如果我们不这样做，我们可以编写一个具有未定义行为的解决方案，该解决方案可能在大多数情况下都可以工作，但是每隔一段时间，它就会崩溃并且很难找出原因！我自己做了这个，结果从中学到了很多东西。

现在我们可以考虑一个我们不使用strand的例子。我们将删除std :: cout对象上的输出锁。

例4a
[扰流板] [/扰流器] 我电脑上的输出如下：[扰流板] [/扰流板]

```
boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] Thread Start" << std::endl;
    global_stream_lock.unlock();

    io_service->run();

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] Thread Finish" << std::endl;
    global_stream_lock.unlock();
}

void PrintNum( int x )
{
    std::cout << "[" << boost::this_thread::get_id()<< "] x: " << x << std::endl;
}

int main( int argc, char * argv[] )
{
    boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
    boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));
    boost::asio::io_service::strand strand( *io_service );

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] The program will exit when all work has finished." << std::endl;
    global_stream_lock.unlock();

    boost::thread_group worker_threads;
    for( int x = 0; x < 2; ++x )
    {
        worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
    }

    boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 ) );

    //strand.post( boost::bind( &PrintNum, 1 ) );
    //strand.post( boost::bind( &PrintNum, 2 ) );
    //strand.post( boost::bind( &PrintNum, 3 ) );
    //strand.post( boost::bind( &PrintNum, 4 ) );
    //strand.post( boost::bind( &PrintNum, 5 ) );

    io_service->post( boost::bind( &PrintNum, 1 ) );
    io_service->post( boost::bind( &PrintNum, 2 ) );
    io_service->post( boost::bind( &PrintNum, 3 ) );
    io_service->post( boost::bind( &PrintNum, 4 ) );
    io_service->post( boost::bind( &PrintNum, 5 ) );

    work.reset();

    worker_threads.join_all();

    return 0;
}

```

[00154F88] The program will exit when all work has finished.
[001532B0] Thread Start
[00154FB0] Thread Start
[[001532B000154FB0] x: ] x: 21

[[001532B000154FB0] x: ] x: 34

[001532B0] x: 5
[00154FB0] Thread Finish
[001532B0] Thread Finish
Press any key to continue . . .


这是非常期待的。由于我们不再锁定std :: cout对象并且有多个线程写入它，因此最终输出结合起来。根据我们拥有多少工作线程以及多少PC核心，输出可能看起来有点不同甚至可能显示正确！从概念上讲，我们知道正确的输出并不意味着什么，因为我们没有正确地同步对全局共享对象的访问！

现在，让我们看看下一个例子，只需注释掉所有的io_service-> post并取消注释strand.post函数调用。这是strand程序的一个输出。

[扰流板] [/扰流板]

[00154F88] The program will exit when all work has finished.
[001532B0] Thread Start
[00154FB0] Thread Start
[001532B0] x: 1
[00154FB0] x: 2
[001532B0] x: 3
[00154FB0] x: 4
[001532B0] x: 5
[00154FB0] Thread Finish
[001532B0] Thread Finish
Press any key to continue . . .


无论我们运行程序多少次，我们都应该每次都看到x输出的干净输出。这是因为strand对象正在将事件处理正确地序列化为一次只有一个线程。非常重要的是，我们注意到，strand不会仅通过一个线程序列化工作。如果我们再次检查先前的输出，则使用多个线程。因此，工作仍将按顺序执行，但它将通过当时可用的任何工作线程执行。我们无法使用不正确的假设编程，相同的线程将实际处理所有工作！如果我们这样做，我们将会有虫子回来咬我们。

如前所述，在过去，我曾以错误的方式使用过绞线而没有意识到它，并且它导致了各种难以发现的问题。现在让我们看看这样一个语法正确的例子，但根据我们的期望在逻辑上是不正确的。

例4b
[扰流板] [/扰流板]


boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id() << "] Thread Start" << std::endl;
    global_stream_lock.unlock();

    io_service->run();

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] Thread Finish" << std::endl;
    global_stream_lock.unlock();
}

void PrintNum( int x )
{
    std::cout << "[" << boost::this_thread::get_id()<< "] x: " << x << std::endl;
}

int main( int argc, char * argv[] )
{
    boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
    boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));
    boost::asio::io_service::strand strand( *io_service );

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] The program will exit when all work has finished." << std::endl;
    global_stream_lock.unlock();

    boost::thread_group worker_threads;
    for( int x = 0; x < 4; ++x )
    {
        worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
    }

    boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
    io_service->post( strand.wrap( boost::bind( &PrintNum, 1 ) ) );
    io_service->post( strand.wrap( boost::bind( &PrintNum, 2 ) ) );

    boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
    io_service->post( strand.wrap( boost::bind( &PrintNum, 3 ) ) );
    io_service->post( strand.wrap( boost::bind( &PrintNum, 4 ) ) );

    boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
    io_service->post( strand.wrap( boost::bind( &PrintNum, 5 ) ) );
    io_service->post( strand.wrap( boost::bind( &PrintNum, 6 ) ) );

    work.reset();

    worker_threads.join_all();

    return 0;
}


如果我们运行这个程序很多次，我们应该看到预期的1,2,3,4,5,6输出。然而，每隔一段时间，我们可能会看到事件被切换的2,1,3,4,5,6或其他变化。有时我们必须运行很多才能实现这一点，而有时则可能更频繁地发生。虽然输出仍然很干净，但订单并不像预期的那样。这是因为我们传递的工作保证是连续执行的，但是由于我们正在使用的API函数，无法保证工作的顺序实际发生！

因此，如果顺序很重要，我们必须通过strand对象API本身。如果顺序不重要，那么我们可以在io_service对象上发布并通过strand包装我们的处理程序。现在看起来似乎很明显，但如果我们自己开始使用这些东西，就很容易误解这些基本概念。我们发布的工作类型将最终决定我们想要使用哪个界面，因为两者都非常有用。我们将看到更多将来使用的strand wrap成员函数的例子。

这几乎涵盖了strand对象。它非常强大，因为它允许我们在没有显式锁定的情况下进行同步。在使用多线程系统和全面保持效率时，这绝对是必备功能。

我们几乎有足够的核心概念可以进入boost :: asio库的网络方面。boost :: asio库非常庞大，有很多很棒的功能！

[page]

5.错误处理

我们需要注意的下一个概念是错误处理。换句话说，当我们的工作函数抛出异常时会发生什么？Boost :: asio为用户提供了两种处理此案例的方法。错误通过处理程序传播到线程调用run或poll系列函数的点。用户可以通过try / switch语句处理异常，也可以选择通过错误变量接收异常。有关boost的更多信息，请查看错误和异常处理。另外，这个错误处理文章还介绍了一些更有用的要点。

首先，我们将看一下异常方法。

示例5a
[扰流器] [/扰流器] 在此示例中，我们将工作发布到io_service，导致异常反复出现。工作对象也不会被销毁，因此io_service应该保持忙碌状态。但是，当我们运行程序时，我们会看到它退出。原因是因为异常通过run函数传播，所以工作线程退出。由于退出了所有工作线程，因此程序在join_all返回后完成。我们可以立即看到，如果我们不小心，这可能会导致问题，因为工作线程可以一个接一个地取出，直到系统没有剩下。现在让我们看看也可能的错误变量方法。


boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] Thread Start" << std::endl;
global_stream_lock.unlock();

try
{
    io_service->run();
}
catch( std::exception & ex )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] Exception: " << ex.what() << std::endl;
    global_stream_lock.unlock();
}

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] Thread Finish" << std::endl;
global_stream_lock.unlock();
}

void RaiseAnException( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] " << __FUNCTION__ << std::endl;
global_stream_lock.unlock();

io_service->post( boost::bind( &RaiseAnException, io_service ) );

throw( std::runtime_error( "Oops!" ) );
}

int main( int argc, char * argv[] )
{
    boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
    boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] The program will exit when all work has finished." << std::endl;
    global_stream_lock.unlock();

    boost::thread_group worker_threads;
    for( int x = 0; x < 2; ++x )
    {
        worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
    }

    io_service->post( boost::bind( &RaiseAnException, io_service ) );

    worker_threads.join_all();

    return 0;
}






例5b
[扰流板] [/扰流板] 哦哦！当我们运行程序时，我们遇到了崩溃。通过调试，我们可以看到它是因为未捕获异常。这是因为错误变量方法不会将用户异常转换为错误，而是将boost :: asio异常转换。记住这一点非常重要！如果我们通过io_service传递我们自己的工作，我们必须遵守C ++异常编程概念。如果boost :: asio库生成错误，如果没有使用错误变量或者它将转换为错误变量，它将作为异常。根据我们的应用，我们会选择最适合我们需要的应用。

```
boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] Thread Start" << std::endl;
    global_stream_lock.unlock();

    boost::system::error_code ec;
    io_service->run( ec );

    if( ec )
    {
        global_stream_lock.lock();
        std::cout << "[" << boost::this_thread::get_id()<< "] Exception: " << ec << std::endl;
        global_stream_lock.unlock();
    }

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] Thread Finish" << std::endl;
    global_stream_lock.unlock();
}

void RaiseAnException( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] " << __FUNCTION__ << std::endl;
    global_stream_lock.unlock();

    io_service->post( boost::bind( &RaiseAnException, io_service ) );

    throw( std::runtime_error( "Oops!" ) );
}

int main( int argc, char * argv[] )
{
    boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
    boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] The program will exit when all work has finished." << std::endl;
    global_stream_lock.unlock();

    boost::thread_group worker_threads;
    for( int x = 0; x < 2; ++x )
    {
        worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
    }

    io_service->post( boost::bind( &RaiseAnException, io_service ) );

    worker_threads.join_all();

    return 0;
}

```


为了进一步澄清我们是否正在使用io_service进行用户工作，如果工作可以生成异常，我们必须使用异常处理。如果我们仅将io_service用于boost :: asio函数，那么我们可以使用异常处理或错误变量。如果我们将io_service用于boost :: asio函数和用户工作，那么我们既可以使用两种方法，也可以只使用异常处理方法，但如果工作可以生成异常，则不仅可以使用错误变量。这应该非常简单。

现在我们知道了这个小细节，我们必须考虑如果实际生成异常会发生什么。我们想要做的还取决于我们正在开发的应用程序的类型。换句话说，是系统故障或上下文故障的例外吗？如果它们是系统故障，那么我们将要调用io_service的stop成员函数并确保工作对象被销毁，以便我们的程序正常退出。如果异常只是上下文失败，那么我们将要设置工作线程函数再次调用run函数，这样工作线程就不会死。以下是修改前的示例。

例5c
[扰流板] [/扰流板]

```
boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] Thread Start" << std::endl;
    global_stream_lock.unlock();

    while( true )
    {
        try
        {
            boost::system::error_code ec;
            io_service->run( ec );
            if( ec )
            {
                global_stream_lock.lock();
                std::cout << "[" << boost::this_thread::get_id()<< "] Error: " << ec << std::endl;
                global_stream_lock.unlock();
            }
            break;
        }
        catch( std::exception & ex )
        {
            global_stream_lock.lock();
            std::cout << "[" << boost::this_thread::get_id()<< "] Exception: " << ex.what() << std::endl;
            global_stream_lock.unlock();
        }
    }

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] Thread Finish" << std::endl;
    global_stream_lock.unlock();
}

void RaiseAnException( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()<< "] " << __FUNCTION__ << std::endl;
global_stream_lock.unlock();

io_service->post( boost::bind( &RaiseAnException, io_service ) );

throw( std::runtime_error( "Oops!" ) );
}

int main( int argc, char * argv[] )
{
    boost::shared_ptr< boost::asio::io_service > io_service(new boost::asio::io_service);
    boost::shared_ptr< boost::asio::io_service::work > work(new boost::asio::io_service::work( *io_service ));

    global_stream_lock.lock();
    std::cout << "[" << boost::this_thread::get_id()<< "] The program will exit when all work has finished." << std::endl;
    global_stream_lock.unlock();

    boost::thread_group worker_threads;
    for( int x = 0; x < 2; ++x )
    {
        worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
    }

    io_service->post( boost::bind( &RaiseAnException, io_service ) );

    worker_threads.join_all();

    return 0;
}
```

现在，当发生异常时，输出它并且工作线程返回处理工作。当调用stop成员函数或销毁工作对象时，run函数不再像我们之前看到的那样阻塞，因此循环退出然后线程结束。如果我们在异常示例中使用此概念，我们将看到事件的无限输出，因为我们总是将新事件发布到队列。显然，我们永远不会希望在真实的程序中出现这种情况。

我们将从boost :: asio库中遇到的大多数错误都来自实际的I / O接口，例如套接字。我们还没有准备好深入研究那些。我们首先需要暴露的boost :: asio库还有更多有用的功能。

[页]

6.计时器

boost :: asio提供了deadline_timer 类，它提供同步和异步接口。文档页面有几个很好的例子，所以我们可以使用我们已经了解的boost :: asio库，立即开始使用更高级的用法。

在我们的第一个例子中，我们将创建一个在5秒内到期的简单计时器。这里应该没有任何意外，因为文档会忽略这个简单的行为。

例6a
[扰流板] [/扰流板]


boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Start" << std::endl;
global_stream_lock.unlock();

while( true )
{
try
{
boost::system::error_code ec;
io_service->run( ec );
if( ec )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << ec << std::endl;
global_stream_lock.unlock();
}
break;
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}
}

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Finish" << std::endl;
global_stream_lock.unlock();
}

void TimerHandler( const boost::system::error_code & error )
{
if( error )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << error << std::endl;
global_stream_lock.unlock();
}
else
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] TimerHandler " << std::endl;
global_stream_lock.unlock();
}
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(
new boost::asio::io_service
);
boost::shared_ptr< boost::asio::io_service::work > work(
new boost::asio::io_service::work( *io_service )
);

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Press [return] to exit." << std::endl;
global_stream_lock.unlock();

boost::thread_group worker_threads;
for( int x = 0; x < 2; ++x )
{
worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
}

boost::asio::deadline_timer timer( *io_service );
timer.expires_from_now( boost::posix_time::seconds( 5 ) );
timer.async_wait( TimerHandler );

std::cin.get();

io_service->stop();

worker_threads.join_all();

return 0;
}


如果我们想要一个定期计时器怎么办？我们可以将计时器对象设置为全局，但这可能会引发一些问题，因为共享对象不是线程安全的。这就是boost :: bind再次拯救的地方！通过将shared_ptr设置为timer对象，我们可以使用boost :: bind并将计时器传递给它自己的处理程序，这样我们就可以让它重复出现。这是一个例子。

例6b
[剧透] [/剧透] 正如我们所看到的，boost :: bind允许我们做一些非常漂亮的事情。_1参数是参数占位符。由于TimerHandler函数需要一个回调参数，我们必须在bind调用中引用它。总而言之，_1表示“第一个参数，稍后将提供”。这个博客有一个很好的帖子解释这个。


boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Start" << std::endl;
global_stream_lock.unlock();

while( true )
{
try
{
boost::system::error_code ec;
io_service->run( ec );
if( ec )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << ec << std::endl;
global_stream_lock.unlock();
}
break;
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}
}

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Finish" << std::endl;
global_stream_lock.unlock();
}

void TimerHandler(
const boost::system::error_code & error,
boost::shared_ptr< boost::asio::deadline_timer > timer
)
{
if( error )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << error << std::endl;
global_stream_lock.unlock();
}
else
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] TimerHandler " << std::endl;
global_stream_lock.unlock();

timer->expires_from_now( boost::posix_time::seconds( 5 ) );
timer->async_wait( boost::bind( &TimerHandler, _1, timer ) );
}
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(
new boost::asio::io_service
);
boost::shared_ptr< boost::asio::io_service::work > work(
new boost::asio::io_service::work( *io_service )
);

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Press [return] to exit." << std::endl;
global_stream_lock.unlock();

boost::thread_group worker_threads;
for( int x = 0; x < 2; ++x )
{
worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
}

boost::shared_ptr< boost::asio::deadline_timer > timer(
new boost::asio::deadline_timer( *io_service )
);
timer->expires_from_now( boost::posix_time::seconds( 5 ) );
timer->async_wait( boost::bind( &TimerHandler, _1, timer ) );

std::cin.get();

io_service->stop();

worker_threads.join_all();

return 0;
}




运行程序后，我们将看到一个每5秒触发一次的计时器。大！现在我们知道除了火灾类型之外如何制作重复计时器。此外，我们可以根据需要利用boost :: bind将更多参数传递给处理程序。但是，我们的定时器将异步执行，因此如果我们有多个工作线程，我们可能在一个线程中执行一个定时器，而另一个线程在另一个线程中执行。让我们假设计时器处理程序和工作处理程序使用相同的共享对象，因此我们现在具有非线程安全设计。我们如何确保计时器不与工作处理程序同时执行？

答案是我们的朋友。通过使用strand对象，我们可以通过strand进行工作，并包装计时器处理程序以通过它进行调度。因此，我们将获得序列化输出，而不必显式同步对共享对象的访问。这是一个显示的例子。

例6c
[扰流板] [/扰流板]


boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Start" << std::endl;
global_stream_lock.unlock();

while( true )
{
try
{
boost::system::error_code ec;
io_service->run( ec );
if( ec )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << ec << std::endl;
global_stream_lock.unlock();
}
break;
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}
}

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Finish" << std::endl;
global_stream_lock.unlock();
}

void TimerHandler(
const boost::system::error_code & error,
boost::shared_ptr< boost::asio::deadline_timer > timer,
boost::shared_ptr< boost::asio::io_service::strand > strand
)
{
if( error )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << error << std::endl;
global_stream_lock.unlock();
}
else
{
std::cout << "[" << boost::this_thread::get_id()
<< "] TimerHandler " << std::endl;

timer->expires_from_now( boost::posix_time::seconds( 1 ) );
timer->async_wait(
strand->wrap( boost::bind( &TimerHandler, _1, timer, strand ) )
);
}
}

void PrintNum( int x )
{
std::cout << "[" << boost::this_thread::get_id()
<< "] x: " << x << std::endl;
boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 ) );
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(
new boost::asio::io_service
);
boost::shared_ptr< boost::asio::io_service::work > work(
new boost::asio::io_service::work( *io_service )
);
boost::shared_ptr< boost::asio::io_service::strand > strand(
new boost::asio::io_service::strand( *io_service )
);

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Press [return] to exit." << std::endl;
global_stream_lock.unlock();

boost::thread_group worker_threads;
for( int x = 0; x < 2; ++x )
{
worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
}

boost::this_thread::sleep( boost::posix_time::seconds( 1 ) );

strand->post( boost::bind( &PrintNum, 1 ) );
strand->post( boost::bind( &PrintNum, 2 ) );
strand->post( boost::bind( &PrintNum, 3 ) );
strand->post( boost::bind( &PrintNum, 4 ) );
strand->post( boost::bind( &PrintNum, 5 ) );

boost::shared_ptr< boost::asio::deadline_timer > timer(
new boost::asio::deadline_timer( *io_service )
);
timer->expires_from_now( boost::posix_time::seconds( 1 ) );
timer->async_wait(
strand->wrap( boost::bind( &TimerHandler, _1, timer, strand ) )
);

std::cin.get();

io_service->stop();

worker_threads.join_all();

return 0;
}


至关重要的是要注意我们必须如何将计时器处理程序包装到各处，而不仅仅是第一次。如果我们忘记了，那么计时器将不再通过strand对象执行，并且可能导致错误。运行程序，我们应该看到前五个工作对象执行，然后是计时器线程。由于所有内容都是序列化的，因此工作对象必须在计时器事件触发之前先按顺序完成。如果我们要删除strand wrap调用，那么我们会看到定时器正常执行，但输出会搞乱，因为我们没有锁定std :: cout对象，因此这表明我们会有多线程错误！有关计时器的更多有用信息，请查看boost :: asio作者撰写的Time Travel文章。

在这一点上，我们可以看到bind，strand和shared_ptr在与boost :: asio库配合使用时如何为我们提供编程所需的功能和灵活性是无价的组件。我们将利用所有这些功能来介绍boost :: asio库的下一个方面，即网络系统。

[page]

7.网络基础：连接器和接受器（TCP）

我们将介绍的boost :: asio网络系统的第一个概念是TCP编程。boost :: asio库的好处是一致性。介绍了boost :: asio库的其他方面，我们已经了解并理解了网络系统使用的框架。我们只需要学习特定的网络API函数！

首先，我们将了解如何同步连接到主机。由于我们的程序将充当客户端，我们现在将使用tcp :: socket 对象。可用的不同协议有不同的套接字类型。因此，我们必须确保从正确的命名空间中使用正确的对象和函数。在我们连接到远程主机之前，我们必须能够获取远程主机的地址。为此，我们将使用tcp :: resolver。

例7a
[扰流板] [/扰流板]


boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Start" << std::endl;
global_stream_lock.unlock();

while( true )
{
try
{
boost::system::error_code ec;
io_service->run( ec );
if( ec )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << ec << std::endl;
global_stream_lock.unlock();
}
break;
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}
}

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Finish" << std::endl;
global_stream_lock.unlock();
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(
new boost::asio::io_service
);
boost::shared_ptr< boost::asio::io_service::work > work(
new boost::asio::io_service::work( *io_service )
);
boost::shared_ptr< boost::asio::io_service::strand > strand(
new boost::asio::io_service::strand( *io_service )
);

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Press [return] to exit." << std::endl;
global_stream_lock.unlock();

boost::thread_group worker_threads;
for( int x = 0; x < 2; ++x )
{
worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
}

boost::asio::ip::tcp::socket sock( *io_service );

try
{
boost::asio::ip::tcp::resolver resolver( *io_service );
boost::asio::ip::tcp::resolver::query query(
"www.google.com",
boost::lexical_cast< std::string >( 80 )
);
boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
boost::asio::ip::tcp::endpoint endpoint = *iterator;

global_stream_lock.lock();
std::cout << "Connecting to: " << endpoint << std::endl;
global_stream_lock.unlock();

sock.connect( endpoint );
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}

std::cin.get();

boost::system::error_code ec;
sock.shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
sock.close( ec );

io_service->stop();

worker_threads.join_all();

return 0;
}


此示例只会打开与Google的连接。该程序将告诉我们它尝试连接的实际IP和端口。如果我们打开命令提示符并运行命令“netstat -n”，我们应该看到该程序的TCP连接。在此示例中，我们使用查询对象的格式来获得可重用的代码。端口可能是整数而不是字符串，因此我们使用lexical_cast 将其转换为字符串。虽然还有其他方法可以做到这一点，但是使用它可以快速轻松地完成。再举一个例子，请务必查看Daytime.1 -在boost站点上的同步TCP白天客户端教程。

有时我们可能不希望同步连接到远程主机。想象一下我们希望通过按钮启动连接的GUI应用程序，但我们不希望GUI在操作完成时冻结。Boost :: asio提供了一种通过套接字异步连接的方法。使用我们已经学习的技术，例如使用boost :: bind和boost :: shared_ptr，我们可以设置我们自己的将被调用的连接处理程序。

例7b
[扰流板] [/扰流板]


boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Start" << std::endl;
global_stream_lock.unlock();

while( true )
{
try
{
boost::system::error_code ec;
io_service->run( ec );
if( ec )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << ec << std::endl;
global_stream_lock.unlock();
}
break;
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}
}

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Finish" << std::endl;
global_stream_lock.unlock();
}

void OnConnect( const boost::system::error_code & ec, boost::shared_ptr< boost::asio::ip::tcp::socket > sock )
{
if( ec )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << ec << std::endl;
global_stream_lock.unlock();
}
else
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Connected!" << std::endl;
global_stream_lock.unlock();
}
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(
new boost::asio::io_service
);
boost::shared_ptr< boost::asio::io_service::work > work(
new boost::asio::io_service::work( *io_service )
);
boost::shared_ptr< boost::asio::io_service::strand > strand(
new boost::asio::io_service::strand( *io_service )
);

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Press [return] to exit." << std::endl;
global_stream_lock.unlock();

boost::thread_group worker_threads;
for( int x = 0; x < 2; ++x )
{
worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
}

boost::shared_ptr< boost::asio::ip::tcp::socket > sock(
new boost::asio::ip::tcp::socket( *io_service )
);

try
{
boost::asio::ip::tcp::resolver resolver( *io_service );
boost::asio::ip::tcp::resolver::query query(
"www.google.com",
boost::lexical_cast< std::string >( 80 )
);
boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
boost::asio::ip::tcp::endpoint endpoint = *iterator;

global_stream_lock.lock();
std::cout << "Connecting to: " << endpoint << std::endl;
global_stream_lock.unlock();

sock->async_connect( endpoint, boost::bind( OnConnect, _1, sock ) );
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}

std::cin.get();

boost::system::error_code ec;
sock->shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
sock->close( ec );

io_service->stop();

worker_threads.join_all();

return 0;
}


为了确保我们仍然在同一页面，比喻说，如果我们希望传递它们，我们必须使用boost :: shared_ptr和大多数boost :: asio对象。这是因为对象本身是不可复制的，我们必须确保在处理程序等待调用时对象仍然有效。我们使用boost :: bind来设置我们自己的自定义处理程序。除了必须具有的默认编号外，此处理程序还可以包含任意数量的参数。在这种情况下，async_connect处理程序是一个ConnectHandler，它只是一个模板参数，但是参考文档我们将看到它必须具有最小的签名。
[quote]
* @param handler连接操作时要调用的处理程序
*完成。将根据需要复制处理程序。
处理程序的函数*签名必须是：
* @code void handler（
* const boost :: system :: error_code＆error //操作结果
*）; @endcode
*无论异步操作是立即完成还是
不完成，都不会从此函数中调用处理程序。
处理程序的调用*将以与使用
* boost :: asio :: io_service :: post（）相当的方式执行。
[/引用]

需要注意的一件很酷的事情是，使用boost :: bind，我们可以根据需要重新排列参数的顺序！重要的是参数最终在物理上存在。希望现在，这个例子对我们为什么做某些事情有意义。在这个例子中，我们仍然同步解析远程地址。如果我们愿意，我们可以通过参考文档将其更改为异步方法。我更喜欢保持简单，只是同步查看。

现在我们有两种不同的方法可以用来连接远程主机。让远程主机连接到我们怎么样？要设置这样的“服务器”，我们将使用tcp :: acceptor 宾语。虽然我们介绍了用于连接的同步和异步方法，但我们将简要介绍服务器的异步方法。原因是，使用boost :: asio库的主要目标是异步性，即使它确实提供了同步方法。我们稍后会看到更多变化的示例，但现在我们将介绍最常用的内容。我们来看看服务器的开始。

例7c
[扰流板] [/扰流板]


boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Start" << std::endl;
global_stream_lock.unlock();

while( true )
{
try
{
boost::system::error_code ec;
io_service->run( ec );
if( ec )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << ec << std::endl;
global_stream_lock.unlock();
}
break;
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}
}

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Finish" << std::endl;
global_stream_lock.unlock();
}

void OnAccept( const boost::system::error_code & ec, boost::shared_ptr< boost::asio::ip::tcp::socket > sock )
{
if( ec )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << ec << std::endl;
global_stream_lock.unlock();
}
else
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Accepted!" << std::endl;
global_stream_lock.unlock();
}
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(
new boost::asio::io_service
);
boost::shared_ptr< boost::asio::io_service::work > work(
new boost::asio::io_service::work( *io_service )
);
boost::shared_ptr< boost::asio::io_service::strand > strand(
new boost::asio::io_service::strand( *io_service )
);

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Press [return] to exit." << std::endl;
global_stream_lock.unlock();

boost::thread_group worker_threads;
for( int x = 0; x < 2; ++x )
{
worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
}

boost::shared_ptr< boost::asio::ip::tcp::acceptor > acceptor(
new boost::asio::ip::tcp::acceptor( *io_service )
);
boost::shared_ptr< boost::asio::ip::tcp::socket > sock(
new boost::asio::ip::tcp::socket( *io_service )
);

try
{
boost::asio::ip::tcp::resolver resolver( *io_service );
boost::asio::ip::tcp::resolver::query query(
"127.0.0.1",
boost::lexical_cast< std::string >( 7777 )
);
boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );
acceptor->open( endpoint.protocol() );
acceptor->set_option( boost::asio::ip::tcp::acceptor::reuse_address( false ) );
acceptor->bind( endpoint );
acceptor->listen( boost::asio::socket_base::max_connections );
acceptor->async_accept( *sock, boost::bind( OnAccept, _1, sock ) );

global_stream_lock.lock();
std::cout << "Listening on: " << endpoint << std::endl;
global_stream_lock.unlock();
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}

std::cin.get();

boost::system::error_code ec;
acceptor->close( ec );

sock->shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
sock->close( ec );

io_service->stop();

worker_threads.join_all();

return 0;
}


这个例子看起来非常接近前一个。事实上，变化很小！如前所述，这是boost :: asio库的一个好处。当我们花时间学习不同的组件时，由于组件之间的依赖性，我们开始能够轻松地理解其他组件。一旦我们运行程序，我们就有一个服务器在端口7777上运行。我们可以运行命令“telnet localhost 7777”来启动与服务器的连接并触发OnAccept功能。

但是，服务器将不再接受任何连接。这是因为我们只调用一次async_accept，只有一个套接字对象。我们稍后将讨论服务器的设计策略，因为我们现在才开始使用所需的核心API。此时，我们还可以简要提及我们看到的关闭和关闭功能。在我们的示例中，我们选择使用error_code变量版本以确保不会抛出异常。这是因为有时关闭可能在套接字上无效（如果没有连接或接受），则关闭时。如果我们使用异常处理，则可能不会调用其中一个函数。因此，我们将简单地尝试为套接字调用这两个函数并忽略错误。

现在已经涵盖了连接和接受的基础知识。我们现在知道如何连接到远程主机以及接受传入连接。但是，我们还没有涵盖对套接字的读写。这将是我们关注的下一个话题。

[page]

8.网络基础：二进制协议发送和接收（TCP）

为了真正完成我们的套接字所做的任何有用的事情，我们必须能够读取和写入它们。有很多不同的方法可以解决这个问题。有些协议依赖于文本（ascii / unicode），而其他协议则依赖于二进制文件。本章将重点介绍较小的二进制示例。对于更广泛的示例集，boost :: asio 示例页面包含许多有用的参考资料。

发送和接收有许多类型的功能。根据我们的协议，我们希望根据我们的应用程序的最佳套件在它们之间进行选择。以下是我们需要注意的相关函数列表：

basic_stream_socket：
[
spoiler ] async_read_some - “此函数用于异步读取流套接字中的数据。函数调用始终立即返回。”
async_receive - “此函数用于异步接收来自流套接字的数据。函数调用始终立即返回。”
async_send - “此函数用于在流套接字上异步发送数据。函数调用总是立即返回。”
async_write_some - “此函数用于异步将数据写入流套接字。函数调用始终立即返回。”
receive - “此函数用于接收流套接字上的数据。函数调用将阻塞，直到成功接收到一个或多个字节的数据，或者直到发生错误。”
read_some - “此函数用于从流套接字读取数据。函数调用将阻塞，直到成功读取一个或多个字节的数据，或者直到发生错误。”
send - “此函数用于在流套接字上发送数据。函数调用将阻塞，直到数据的一个或多个字节已成功发送，或者直到发生错误。”
write_some - “此函数用于将数据写入流套接字。函数调用将阻塞，直到数据的一个或多个字节成功写入，或直到发生错误。”
[/ spoiler ]

boost :: asio Free Functions
[
spoiler ] async_read - “启动异步操作以从流中读取一定数量的数据。”
async_read_at - “启动异步操作以读取指定偏移量的一定数量的数据。”
async_read_until - “启动异步操作以将数据读入streambuf，直到它包含分隔符，匹配正则表达式或函数对象表示匹配。”
ASYNC_WRITE - “启动异步操作以将一定量的数据写入流中。”
async_write_at - “启动异步操作以在指定的偏移处写入一定量的数据。”
read - “在返回之前尝试从流中读取一定数量的数据。”
read_at - “在返回之前尝试读取指定偏移量的一定数量的数据。”
read_until - “将数据读入streambuf，直到它包含分隔符，匹配正则表达式，或者函数对象表示匹配。”
write - “在返回之前将一定数量的数据写入流中。”
write_at - “ 在返回之前在指定的偏移量处写入一定量的数据。
“ [/剧透]

哇！这是很多功能。我们应该记住boost :: asio不仅仅是网络功能，所以这就是为什么它们有这么多。当我们花费更多时间使用boost :: asio库时，我们将能够很容易地知道应用程序应该使用哪些函数。对于本指南，我们将使用async_write和async_read_some。我们将使用async_write的原因是因为该函数将为我们写入所有数据，因此我们不必担心部分发送。同样，我们使用async_read_some作为通用函数来读取一些数据，因为我们没有用于接收的特定协议。我们稍后会讨论这个概念。

现在，让我们看一下使用IO函数的完整示例。我们将使用示例7C中的修改后的服务器示例。

示例8a
[扰流器] [/扰流器] 在此示例中，我们添加一个ClientContext类，其中包含传入连接的所有上下文特定内容。该类包含了发送和接收所需的IO功能。在此示例中，服务器将以（消息大小）[xx xx]（消息有效负载）[xx ... xx]的格式向传入连接发送消息。但是，我们还没有客户端，所以要测试我们可以简单地telnet到服务器来接收数据。我们发送到服务器的任何数据都将被转储到控制台上。


boost::mutex global_stream_lock;

void WorkerThread( boost::shared_ptr< boost::asio::io_service > io_service )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Start" << std::endl;
global_stream_lock.unlock();

while( true )
{
try
{
boost::system::error_code ec;
io_service->run( ec );
if( ec )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << ec << std::endl;
global_stream_lock.unlock();
}
break;
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}
}

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Thread Finish" << std::endl;
global_stream_lock.unlock();
}

void OnAccept( const boost::system::error_code & ec, boost::shared_ptr< boost::asio::ip::tcp::socket > sock )
{
if( ec )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Error: " << ec << std::endl;
global_stream_lock.unlock();
}
else
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Accepted!" << std::endl;
global_stream_lock.unlock();
}
}

int main( int argc, char * argv[] )
{
boost::shared_ptr< boost::asio::io_service > io_service(
new boost::asio::io_service
);
boost::shared_ptr< boost::asio::io_service::work > work(
new boost::asio::io_service::work( *io_service )
);
boost::shared_ptr< boost::asio::io_service::strand > strand(
new boost::asio::io_service::strand( *io_service )
);

global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Press [return] to exit." << std::endl;
global_stream_lock.unlock();

boost::thread_group worker_threads;
for( int x = 0; x < 2; ++x )
{
worker_threads.create_thread( boost::bind( &WorkerThread, io_service ) );
}

boost::shared_ptr< boost::asio::ip::tcp::acceptor > acceptor(
new boost::asio::ip::tcp::acceptor( *io_service )
);
boost::shared_ptr< boost::asio::ip::tcp::socket > sock(
new boost::asio::ip::tcp::socket( *io_service )
);

try
{
boost::asio::ip::tcp::resolver resolver( *io_service );
boost::asio::ip::tcp::resolver::query query(
"127.0.0.1",
boost::lexical_cast< std::string >( 7777 )
);
boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );
acceptor->open( endpoint.protocol() );
acceptor->set_option( boost::asio::ip::tcp::acceptor::reuse_address( false ) );
acceptor->bind( endpoint );
acceptor->listen( boost::asio::socket_base::max_connections );
acceptor->async_accept( *sock, boost::bind( OnAccept, _1, sock ) );

global_stream_lock.lock();
std::cout << "Listening on: " << endpoint << std::endl;
global_stream_lock.unlock();
}
catch( std::exception & ex )
{
global_stream_lock.lock();
std::cout << "[" << boost::this_thread::get_id()
<< "] Exception: " << ex.what() << std::endl;
global_stream_lock.unlock();
}

std::cin.get();

boost::system::error_code ec;
acceptor->close( ec );

sock->shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
sock->close( ec );

io_service->stop();

worker_threads.join_all();

return 0;
}




我们需要从前面的例子中拿走一些东西。首先，我们希望服务的每个连接都需要自己的上下文。此上下文应包含套接字以及send / recv缓冲区以及任何其他用户数据。接下来，该示例本身并不特别是线程安全，因此我们仅将其限制为一个工作线程。我们稍后会讨论这个问题。但是现在，应该明确读取和写入套接字的概念。我们只需为特定任务选择合适的API函数，并确保正确使用它们。

为了正确使用它们，我们必须确保上下文和缓冲区在函数的整个持续时间内保持有效。在这种情况下，我们使用发送的向量列表和一个向量作为公共接收缓冲区。根据我们必须实现的协议，我们可能希望在此处和那里更改一些内容。例如，假设我们想要在流中处理数据包。在这种情况下，我们希望使用带有标头大小的async_read，然后再使用有效负载的确切大小调用async_read。这样，我们不必在使用async_read_some时跟踪流的位置。

在流中一次处理一个分组的缺点是它在较大规模的程序中是低效的。想象一下，我们在流中有100个3字节的消息。我们必须执行100次“读取一个数据包”逻辑！相比之下，如果我们使用async_read_some，我们很可能会立即获取所有数据并从逻辑上解析它。这是首选方法，但也更复杂。根据我们的应用需求，我们必须决定我们走哪条路线。在某些情况下，我们既不能使用，也不能使用。例如，如果我们有一个流协议，例如HTTP，那么我们不能简单地等待一次确切的字节数，而是尽可能多地接收然后处理它。

客户端的代码看起来也很相似。唯一的区别是连接上下文将连接到远程主机而不是被接受。因此，我们可以重用很多代码。但是，我们不会深入将前一个示例修改为客户端。原因是因为在本指南的这一点上，我们已经了解了使用boost :: asio库进行TCP编程时需要注意的所有概念。这意味着我们可以查看完整的网络包装器，并且我们应该能够理解所使用的所有概念。

我们想要转换到网络包装器的原因是因为我们需要可重用的代码来工作，而不是像过去那样一遍又一遍地重新编码相同的逻辑。包装器的另一个好处是它已经过测试，应该相对没有bug。在我们继续编写自己的代码时，拥有可以在将来引用的代码可以提供很大的帮助。话虽如此，我们接下来将看一下网络包装器。

[page]

1. boost :: asio网络包装器（TCP）

现在我们已经了解了使用boost :: asio库和一些简单的TCP网络方面的基础知识，我们可以看一下处理低级别内容的网络包装器。通过使用包装器，我们可以重用它并始终专注于应用程序逻辑，而不是重写网络代码。

[color =“＃ff0000”] 重要说明： [/ color]此代码纯粹用于教育目的。不要在生产系统中使用它，因为可能存在错误。代码旨在以特定方式工作，如果您不需要此类行为，则可能导致执行不当。虽然我已经测试并部署了许多项目的代码，但我总是发现要修复的小问题。

此外，必须考虑使用这种网络包装器的开销。例如，矢量和列表背后有大量的分配。此外，shared_ptr和boost :: bind处理程序的司法使用确实增加了一些在某些环境中可能无法接受的开销。这就是为什么这段代码仅供教育使用！

network.h
[spoiler] [/ spoiler] network.cpp [spoiler] [/ spoiler] 网络库试图提供一个线程安全的可伸缩包装器，以便轻松实现客户端和服务器应用程序。用户将根据需要从基本Connection，Acceptor或Hive类派生自定义类。以下示例将显示使用包装器的基础知识。

#pragma once

#ifndef NETWORK_H_
#define NETWORK_H_

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

using boost::uint64_t;
using boost::uint32_t;
using boost::uint16_t;
using boost::uint8_t;

using boost::int64_t;
using boost::int32_t;
using boost::int16_t;
using boost::int8_t;

//-----------------------------------------------------------------------------

class Hive;
class Acceptor;
class Connection;

//-----------------------------------------------------------------------------

class Connection : public boost::enable_shared_from_this< Connection >
{
friend class Acceptor;
friend class Hive;

private:
boost::shared_ptr< Hive > m_hive;
boost::asio::ip::tcp::socket m_socket;
boost::asio::strand m_io_strand;
boost::asio::deadline_timer m_timer;
boost::posix_time::ptime m_last_time;
std::vector< uint8_t > m_recv_buffer;
std::list< int32_t > m_pending_recvs;
std::list< std::vector< uint8_t > > m_pending_sends;
int32_t m_receive_buffer_size;
int32_t m_timer_interval;
volatile uint32_t m_error_state;

protected:
Connection( boost::shared_ptr< Hive > hive );
virtual ~Connection();

private:
Connection( const Connection & rhs );
Connection & operator =( const Connection & rhs );
void StartSend();
void StartRecv( int32_t total_bytes );
void StartTimer();
void StartError( const boost::system::error_code & error );
void DispatchSend( std::vector< uint8_t > buffer );
void DispatchRecv( int32_t total_bytes );
void DispatchTimer( const boost::system::error_code & error );
void HandleConnect( const boost::system::error_code & error );
void HandleSend( const boost::system::error_code & error, std::list< std::vector< uint8_t > >::iterator itr );
void HandleRecv( const boost::system::error_code & error, int32_t actual_bytes );
void HandleTimer( const boost::system::error_code & error );

private:
// Called when the connection has successfully connected to the local
// host.
virtual void OnAccept( const std::string & host, uint16_t port ) = 0;

// Called when the connection has successfully connected to the remote
// host.
virtual void OnConnect( const std::string & host, uint16_t port ) = 0;

// Called when data has been sent by the connection.
virtual void OnSend( const std::vector< uint8_t > & buffer ) = 0;

// Called when data has been received by the connection.
virtual void OnRecv( std::vector< uint8_t > & buffer ) = 0;

// Called on each timer event.
virtual void OnTimer( const boost::posix_time::time_duration & delta ) = 0;

// Called when an error is encountered.
virtual void OnError( const boost::system::error_code & error ) = 0;

public:
// Returns the Hive object.
boost::shared_ptr< Hive > GetHive();

// Returns the socket object.
boost::asio::ip::tcp::socket & GetSocket();

// Returns the strand object.
boost::asio::strand & GetStrand();

// Sets the application specific receive buffer size used. For stream
// based protocols such as HTTP, you want this to be pretty large, like
// 64kb. For packet based protocols, then it will be much smaller,
// usually 512b - 8kb depending on the protocol. The default value is
// 4kb.
void SetReceiveBufferSize( int32_t size );

// Returns the size of the receive buffer size of the current object.
int32_t GetReceiveBufferSize() const;

// Sets the timer interval of the object. The interval is changed after
// the next update is called.
void SetTimerInterval( int32_t timer_interval_ms );

// Returns the timer interval of the object.
int32_t GetTimerInterval() const;

// Returns true if this object has an error associated with it.
bool HasError();

// Binds the socket to the specified interface.
void Bind( const std::string & ip, uint16_t port );

// Starts an a/synchronous connect.
void Connect( const std::string & host, uint16_t port );

// Posts data to be sent to the connection.
void Send( const std::vector< uint8_t > & buffer );

// Posts a recv for the connection to process. If total_bytes is 0, then
// as many bytes as possible up to GetReceiveBufferSize() will be
// waited for. If Recv is not 0, then the connection will wait for exactly
// total_bytes before invoking OnRecv.
void Recv( int32_t total_bytes = 0 );

// Posts an asynchronous disconnect event for the object to process.
void Disconnect();
};

//-----------------------------------------------------------------------------

class Acceptor : public boost::enable_shared_from_this< Acceptor >
{
friend class Hive;

private:
boost::shared_ptr< Hive > m_hive;
boost::asio::ip::tcp::acceptor m_acceptor;
boost::asio::strand m_io_strand;
boost::asio::deadline_timer m_timer;
boost::posix_time::ptime m_last_time;
int32_t m_timer_interval;
volatile uint32_t m_error_state;

private:
Acceptor( const Acceptor & rhs );
Acceptor & operator =( const Acceptor & rhs );
void StartTimer();
void StartError( const boost::system::error_code & error );
void DispatchAccept( boost::shared_ptr< Connection > connection );
void HandleTimer( const boost::system::error_code & error );
void HandleAccept( const boost::system::error_code & error, boost::shared_ptr< Connection > connection );

protected:
Acceptor( boost::shared_ptr< Hive > hive );
virtual ~Acceptor();

private:
// Called when a connection has connected to the server. This function
// should return true to invoke the connection's OnAccept function if the
// connection will be kept. If the connection will not be kept, the
// connection's Disconnect function should be called and the function
// should return false.
virtual bool OnAccept( boost::shared_ptr< Connection > connection, const std::string & host, uint16_t port ) = 0;

// Called on each timer event.
virtual void OnTimer( const boost::posix_time::time_duration & delta ) = 0;

// Called when an error is encountered. Most typically, this is when the
// acceptor is being closed via the Stop function or if the Listen is
// called on an address that is not available.
virtual void OnError( const boost::system::error_code & error ) = 0;

public:
// Returns the Hive object.
boost::shared_ptr< Hive > GetHive();

// Returns the acceptor object.
boost::asio::ip::tcp::acceptor & GetAcceptor();

// Returns the strand object.
boost::asio::strand & GetStrand();

// Sets the timer interval of the object. The interval is changed after
// the next update is called. The default value is 1000 ms.
void SetTimerInterval( int32_t timer_interval_ms );

// Returns the timer interval of the object.
int32_t GetTimerInterval() const;

// Returns true if this object has an error associated with it.
bool HasError();

public:
// Begin listening on the specific network interface.
void Listen( const std::string & host, const uint16_t & port );

// Posts the connection to the listening interface. The next client that
// connections will be given this connection. If multiple calls to Accept
// are called at a time, then they are accepted in a FIFO order.
void Accept( boost::shared_ptr< Connection > connection );

// Stop the Acceptor from listening.
void Stop();
};

//-----------------------------------------------------------------------------

class Hive : public boost::enable_shared_from_this< Hive >
{
private:
boost::asio::io_service m_io_service;
boost::shared_ptr< boost::asio::io_service::work > m_work_ptr;
volatile uint32_t m_shutdown;

private:
Hive( const Hive & rhs );
Hive & operator =( const Hive & rhs );

public:
Hive();
virtual ~Hive();

// Returns the io_service of this object.
boost::asio::io_service & GetService();

// Returns true if the Stop function has been called.
bool HasStopped();

// Polls the networking subsystem once from the current thread and
// returns.
void Poll();

// Runs the networking system on the current thread. This function blocks
// until the networking system is stopped, so do not call on a single
// threaded application with no other means of being able to call Stop
// unless you code in such logic.
void Run();

// Stops the networking system. All work is finished and no more
// networking interactions will be possible afterwards until Reset is called.
void Stop();

// Restarts the networking system after Stop as been called. A new work
// object is created ad the shutdown flag is cleared.
void Reset();
};

//-----------------------------------------------------------------------------

#endif




"network.h"

//-----------------------------------------------------------------------------

Hive::Hive()
: m_work_ptr( new boost::asio::io_service::work( m_io_service ) ), m_shutdown( 0 )
{
}

Hive::~Hive()
{
}

boost::asio::io_service & Hive::GetService()
{
return m_io_service;
}

bool Hive::HasStopped()
{
return ( boost::interprocess::detail::atomic_cas32( &m_shutdown, 1, 1 ) == 1 );
}

void Hive::Poll()
{
m_io_service.poll();
}

void Hive::Run()
{
m_io_service.run();
}

void Hive::Stop()
{
if( boost::interprocess::detail::atomic_cas32( &m_shutdown, 1, 0 ) == 0 )
{
m_work_ptr.reset();
m_io_service.run();
m_io_service.stop();
}
}

void Hive::Reset()
{
if( boost::interprocess::detail::atomic_cas32( &m_shutdown, 0, 1 ) == 1 )
{
m_io_service.reset();
m_work_ptr.reset( new boost::asio::io_service::work( m_io_service ) );
}
}

//-----------------------------------------------------------------------------

Acceptor::Acceptor( boost::shared_ptr< Hive > hive )
: m_hive( hive ), m_acceptor( hive->GetService() ), m_io_strand( hive->GetService() ), m_timer( hive->GetService() ), m_timer_interval( 1000 ), m_error_state( 0 )
{
}

Acceptor::~Acceptor()
{
}

void Acceptor::StartTimer()
{
m_last_time = boost::posix_time::microsec_clock::local_time();
m_timer.expires_from_now( boost::posix_time::milliseconds( m_timer_interval ) );
m_timer.async_wait( m_io_strand.wrap( boost::bind( &Acceptor::HandleTimer, shared_from_this(), _1 ) ) );
}

void Acceptor::StartError( const boost::system::error_code & error )
{
if( boost::interprocess::detail::atomic_cas32( &m_error_state, 1, 0 ) == 0 )
{
boost::system::error_code ec;
m_acceptor.cancel( ec );
m_acceptor.close( ec );
m_timer.cancel( ec );
OnError( error );
}
}

void Acceptor::DispatchAccept( boost::shared_ptr< Connection > connection )
{
m_acceptor.async_accept( connection->GetSocket(), connection->GetStrand().wrap( boost::bind( &Acceptor::HandleAccept, shared_from_this(), _1, connection ) ) );
}

void Acceptor::HandleTimer( const boost::system::error_code & error )
{
if( error || HasError() || m_hive->HasStopped() )
{
StartError( error );
}
else
{
OnTimer( boost::posix_time::microsec_clock::local_time() - m_last_time );
StartTimer();
}
}

void Acceptor::HandleAccept( const boost::system::error_code & error, boost::shared_ptr< Connection > connection )
{
if( error || HasError() || m_hive->HasStopped() )
{
connection->StartError( error );
}
else
{
if( connection->GetSocket().is_open() )
{
connection->StartTimer();
if( OnAccept( connection, connection->GetSocket().remote_endpoint().address().to_string(), connection->GetSocket().remote_endpoint().port() ) )
{
connection->OnAccept( m_acceptor.local_endpoint().address().to_string(), m_acceptor.local_endpoint().port() );
}
}
else
{
StartError( error );
}
}
}

void Acceptor::Stop()
{
m_io_strand.post( boost::bind( &Acceptor::HandleTimer, shared_from_this(), boost::asio::error::connection_reset ) );
}

void Acceptor::Accept( boost::shared_ptr< Connection > connection )
{
m_io_strand.post( boost::bind( &Acceptor::DispatchAccept, shared_from_this(), connection ) );
}

void Acceptor::Listen( const std::string & host, const uint16_t & port )
{
boost::asio::ip::tcp::resolver resolver( m_hive->GetService() );
boost::asio::ip::tcp::resolver::query query( host, boost::lexical_cast< std::string >( port ) );
boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );
m_acceptor.open( endpoint.protocol() );
m_acceptor.set_option( boost::asio::ip::tcp::acceptor::reuse_address( false ) );
m_acceptor.bind( endpoint );
m_acceptor.listen( boost::asio::socket_base::max_connections );
StartTimer();
}

boost::shared_ptr< Hive > Acceptor::GetHive()
{
return m_hive;
}

boost::asio::ip::tcp::acceptor & Acceptor::GetAcceptor()
{
return m_acceptor;
}

int32_t Acceptor::GetTimerInterval() const
{
return m_timer_interval;
}

void Acceptor::SetTimerInterval( int32_t timer_interval )
{
m_timer_interval = timer_interval;
}

bool Acceptor::HasError()
{
return ( boost::interprocess::detail::atomic_cas32( &m_error_state, 1, 1 ) == 1 );
}

//-----------------------------------------------------------------------------

Connection::Connection( boost::shared_ptr< Hive > hive )
: m_hive( hive ), m_socket( hive->GetService() ), m_io_strand( hive->GetService() ), m_timer( hive->GetService() ), m_receive_buffer_size( 4096 ), m_timer_interval( 1000 ), m_error_state( 0 )
{
}

Connection::~Connection()
{
}

void Connection::Bind( const std::string & ip, uint16_t port )
{
boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::address::from_string( ip ), port );
m_socket.open( endpoint.protocol() );
m_socket.set_option( boost::asio::ip::tcp::acceptor::reuse_address( false ) );
m_socket.bind( endpoint );
}

void Connection::StartSend()
{
if( !m_pending_sends.empty() )
{
boost::asio::async_write( m_socket, boost::asio::buffer( m_pending_sends.front() ), m_io_strand.wrap( boost::bind( &Connection::HandleSend, shared_from_this(), boost::asio::placeholders::error, m_pending_sends.begin() ) ) );
}
}

void Connection::StartRecv( int32_t total_bytes )
{
if( total_bytes > 0 )
{
m_recv_buffer.resize( total_bytes );
boost::asio::async_read( m_socket, boost::asio::buffer( m_recv_buffer ), m_io_strand.wrap( boost::bind( &Connection::HandleRecv, shared_from_this(), _1, _2 ) ) );
}
else
{
m_recv_buffer.resize( m_receive_buffer_size );
m_socket.async_read_some( boost::asio::buffer( m_recv_buffer ), m_io_strand.wrap( boost::bind( &Connection::HandleRecv, shared_from_this(), _1, _2 ) ) );
}
}

void Connection::StartTimer()
{
m_last_time = boost::posix_time::microsec_clock::local_time();
m_timer.expires_from_now( boost::posix_time::milliseconds( m_timer_interval ) );
m_timer.async_wait( m_io_strand.wrap( boost::bind( &Connection::DispatchTimer, shared_from_this(), _1 ) ) );
}

void Connection::StartError( const boost::system::error_code & error )
{
if( boost::interprocess::detail::atomic_cas32( &m_error_state, 1, 0 ) == 0 )
{
boost::system::error_code ec;
m_socket.shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
m_socket.close( ec );
m_timer.cancel( ec );
OnError( error );
}
}

void Connection::HandleConnect( const boost::system::error_code & error )
{
if( error || HasError() || m_hive->HasStopped() )
{
StartError( error );
}
else
{
if( m_socket.is_open() )
{
OnConnect( m_socket.remote_endpoint().address().to_string(), m_socket.remote_endpoint().port() );
}
else
{
StartError( error );
}
}
}

void Connection::HandleSend( const boost::system::error_code & error, std::list< std::vector< uint8_t > >::iterator itr )
{
if( error || HasError() || m_hive->HasStopped() )
{
StartError( error );
}
else
{
OnSend( *itr );
m_pending_sends.erase( itr );
StartSend();
}
}

void Connection::HandleRecv( const boost::system::error_code & error, int32_t actual_bytes )
{
if( error || HasError() || m_hive->HasStopped() )
{
StartError( error );
}
else
{
m_recv_buffer.resize( actual_bytes );
OnRecv( m_recv_buffer );
m_pending_recvs.pop_front();
if( !m_pending_recvs.empty() )
{
StartRecv( m_pending_recvs.front() );
}
}
}

void Connection::HandleTimer( const boost::system::error_code & error )
{
if( error || HasError() || m_hive->HasStopped() )
{
StartError( error );
}
else
{
OnTimer( boost::posix_time::microsec_clock::local_time() - m_last_time );
StartTimer();
}
}

void Connection::DispatchSend( std::vector< uint8_t > buffer )
{
bool should_start_send = m_pending_sends.empty();
m_pending_sends.push_back( buffer );
if( should_start_send )
{
StartSend();
}
}

void Connection::DispatchRecv( int32_t total_bytes )
{
bool should_start_receive = m_pending_recvs.empty();
m_pending_recvs.push_back( total_bytes );
if( should_start_receive )
{
StartRecv( total_bytes );
}
}

void Connection::DispatchTimer( const boost::system::error_code & error )
{
m_io_strand.post( boost::bind( &Connection::HandleTimer, shared_from_this(), error ) );
}

void Connection::Connect( const std::string & host, uint16_t port)
{
boost::system::error_code ec;
boost::asio::ip::tcp::resolver resolver( m_hive->GetService() );
boost::asio::ip::tcp::resolver::query query( host, boost::lexical_cast< std::string >( port ) );
boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
m_socket.async_connect( *iterator, m_io_strand.wrap( boost::bind( &Connection::HandleConnect, shared_from_this(), _1 ) ) );
StartTimer();
}

void Connection::Disconnect()
{
m_io_strand.post( boost::bind( &Connection::HandleTimer, shared_from_this(), boost::asio::error::connection_reset ) );
}

void Connection::Recv( int32_t total_bytes )
{
m_io_strand.post( boost::bind( &Connection::DispatchRecv, shared_from_this(), total_bytes ) );
}

void Connection::Send( const std::vector< uint8_t > & buffer )
{
m_io_strand.post( boost::bind( &Connection::DispatchSend, shared_from_this(), buffer ) );
}

boost::asio::ip::tcp::socket & Connection::GetSocket()
{
return m_socket;
}

boost::asio::strand & Connection::GetStrand()
{
return m_io_strand;
}

boost::shared_ptr< Hive > Connection::GetHive()
{
return m_hive;
}

void Connection::SetReceiveBufferSize( int32_t size )
{
m_receive_buffer_size = size;
}

int32_t Connection::GetReceiveBufferSize() const
{
return m_receive_buffer_size;
}

int32_t Connection::GetTimerInterval() const
{
return m_timer_interval;
}

void Connection::SetTimerInterval( int32_t timer_interval )
{
m_timer_interval = timer_interval;
}

bool Connection::HasError()
{
return ( boost::interprocess::detail::atomic_cas32( &m_error_state, 1, 1 ) == 1 );
}

//-----------------------------------------------------------------------------





我们将看到的第一个示例是使用包装器设置服务器。在这种情况下，它与我们之前的示例非常相似。它只是回应发送或接收的所有流量。这次服务器是一个简单的echo服务器。

例9a
[剧透] [/剧透] 代码应该非常简单。由于我们现在正在使用包装器，所以套接字管理的所有细节现在都在幕后，因此我们可以专注于应用程序逻辑。在此示例中，我们不使用任何工作线程，但是相同的概念适用于前面示例中所示。我们现在知道服务器是什么样的，所以让我们来看看客户端。例9b [扰流板] [/扰流板]

"network.h"

boost::mutex global_stream_lock;

class MyConnection : public Connection
{
private:

private:
void OnAccept( const std::string & host, uint16_t port )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
global_stream_lock.unlock();

// Start the next receive
Recv();
}

void OnConnect( const std::string & host, uint16_t port )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
global_stream_lock.unlock();

// Start the next receive
Recv();
}

void OnSend( const std::vector< uint8_t > & buffer )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << buffer.size() << " bytes" << std::endl;
for( size_t x = 0; x < buffer.size(); ++x )
{
std::cout << std::hex << std::setfill( '0' ) <<
std::setw( 2 ) << (int)buffer[ x ] << " ";
if( ( x + 1 ) % 16 == 0 )
{
std::cout << std::endl;
}
}
std::cout << std::endl;
global_stream_lock.unlock();
}

void OnRecv( std::vector< uint8_t > & buffer )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << buffer.size() << " bytes" << std::endl;
for( size_t x = 0; x < buffer.size(); ++x )
{
std::cout << std::hex << std::setfill( '0' ) <<
std::setw( 2 ) << (int)buffer[ x ] << " ";
if( ( x + 1 ) % 16 == 0 )
{
std::cout << std::endl;
}
}
std::cout << std::endl;
global_stream_lock.unlock();

// Start the next receive
Recv();

// Echo the data back
Send( buffer );
}

void OnTimer( const boost::posix_time::time_duration & delta )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << delta << std::endl;
global_stream_lock.unlock();
}

void OnError( const boost::system::error_code & error )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << error << std::endl;
global_stream_lock.unlock();
}

public:
MyConnection( boost::shared_ptr< Hive > hive )
: Connection( hive )
{
}

~MyConnection()
{
}
};

class MyAcceptor : public Acceptor
{
private:

private:
bool OnAccept( boost::shared_ptr< Connection > connection, const std::string & host, uint16_t port )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
global_stream_lock.unlock();

return true;
}

void OnTimer( const boost::posix_time::time_duration & delta )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << delta << std::endl;
global_stream_lock.unlock();
}

void OnError( const boost::system::error_code & error )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << error << std::endl;
global_stream_lock.unlock();
}

public:
MyAcceptor( boost::shared_ptr< Hive > hive )
: Acceptor( hive )
{
}

~MyAcceptor()
{
}
};

int main( int argc, char * argv[] )
{
boost::shared_ptr< Hive > hive( new Hive() );

boost::shared_ptr< MyAcceptor > acceptor( new MyAcceptor( hive ) );
acceptor->Listen( "127.0.0.1", 7777 );

boost::shared_ptr< MyConnection > connection( new MyConnection( hive ) );
acceptor->Accept( connection );

while( !_kbhit() )
{
hive->Poll();
Sleep( 1 );
}

hive->Stop();

return 0;
}






"network.h"

boost::mutex global_stream_lock;

class MyConnection : public Connection
{
private:

private:
void OnAccept( const std::string & host, uint16_t port )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
global_stream_lock.unlock();

// Start the next receive
Recv();
}

void OnConnect( const std::string & host, uint16_t port )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << host << ":" << port << std::endl;
global_stream_lock.unlock();

// Start the next receive
Recv();

std::string str = "GET / HTTP/1.0\r\n\r\n";

std::vector< uint8_t > request;
std::copy( str.begin(), str.end(), std::back_inserter( request ) );
Send( request );
}

void OnSend( const std::vector< uint8_t > & buffer )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << buffer.size() << " bytes" << std::endl;
for( size_t x = 0; x < buffer.size(); ++x )
{
std::cout << std::hex << std::setfill( '0' ) <<
std::setw( 2 ) << (int)buffer[ x ] << " ";
if( ( x + 1 ) % 16 == 0 )
{
std::cout << std::endl;
}
}
std::cout << std::endl;
global_stream_lock.unlock();
}

void OnRecv( std::vector< uint8_t > & buffer )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << buffer.size() << " bytes" << std::endl;
for( size_t x = 0; x < buffer.size(); ++x )
{
std::cout << std::hex << std::setfill( '0' ) <<
std::setw( 2 ) << (int)buffer[ x ] << " ";
if( ( x + 1 ) % 16 == 0 )
{
std::cout << std::endl;
}
}
std::cout << std::endl;
global_stream_lock.unlock();

// Start the next receive
Recv();
}

void OnTimer( const boost::posix_time::time_duration & delta )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << delta << std::endl;
global_stream_lock.unlock();
}

void OnError( const boost::system::error_code & error )
{
global_stream_lock.lock();
std::cout << "[" << __FUNCTION__ << "] " << error << std::endl;
global_stream_lock.unlock();
}

public:
MyConnection( boost::shared_ptr< Hive > hive )
: Connection( hive )
{
}

~MyConnection()
{
}
};

int main( int argc, char * argv[] )
{
boost::shared_ptr< Hive > hive( new Hive() );

boost::shared_ptr< MyConnection > connection( new MyConnection( hive ) );
connection->Connect( "www.google.com", 80 );

while( !_kbhit() )
{
hive->Poll();
Sleep( 1 );
}

hive->Stop();

return 0;
}


此客户端只是向Google发送HTTP GET，然后将输出以十六进制格式转储到控制台。可重用性的主题在此示例中进一步显示，因为此示例中的代码与服务器的代码没有太大区别。这意味着我们的客户端和服务器程序不会完全不同。这是一件好事，因为它使未来的维护更加轻松。

通过我们的网络包装器，我们可以看到为我们简化了大量工作。应该注意这个特定的网络包装器有很多设计含义。首先，对于“服务器”，没有将每个连接存储到容器中以便于引用的概念。这样做是因为要将所有连接添加到容器中，最终用户必须锁定容器以同步对其的访问，或者实现添加和删除连接的异步方法。简而言之，这是最终用户在需要时必须实现的行为。并非所有网络应用程序都必须知道一次存在的所有连接（例如HTTP服务器），因此包装器采用对所有人都有益的最通用方法。

接下来，所有连接交互都通过唯一链完成。正如我们已经介绍过的那样，strand对象允许事件按顺序执行。因此，每次事件发生时我们都不必显式锁定连接，因为无论我们有多少工作线程，都不会同时发生任何事件。必须要小心，因为如果用户添加任何自定义方法，那么他们必须遵循相同的设计来实现他们的逻辑以保持类线程的安全。

简单的发送/接收缓冲逻辑通过向量和列表实现。从长远来看，这显然具有严重的记忆意义。但是，任何具有自定义内存需求的人都将拥有自己独特的系统，因此他们会根据需要修改代码。对于大多数简单的应用程序，提供的系统足够好并且跨平台（标准C ++库容器），因此没有努力使该方面复杂化。

最后，包装的具体设计并不适合所有人。这只是boost :: asio库可能实现的一个例子。我更喜欢并自己使用这个包装器，这就是我分享它的原因。随意提出您自己的，并根据您的需要进行定制！重要的是熟悉boost :: asio库。

[页]

10.前方的道路现在

有三件基本的事情需要发生。我们需要：
1。继续学习boost和boost :: asio库的细节，以进一步扩展我们的程序。
2.为基本的boost :: asio网络内容编写，测试和部署可重用的包装器，这样我们就不必继续重写它了。
3.在处理手头协议特定任务的包装器之上编写我们自己的网络库。

通过这样做，我们将能够最终利用boost :: asio库的全部潜力。boost :: asio库仍有很多方面，本指南未涉及到这些方面。这是因为本指南是一本快速入门指南。虽然本指南当然不短，但它只是表明需要多少才能真正进入boost :: asio库。希望本指南能够为学习boost :: asio的基础知识提供一个很好的方向。我在尝试学习boost :: asio时遇到的最大问题是找到真正涵盖了库的许多基础的实用资源，以便最终用户知道他们需要了解什么以充分利用库。

UDP不像TCP那样涉及，因为它是无连接的。但是，程序员需要在特定于其应用程序的UDP之上实现自己的层。因此，目前没有涉及UDP主题，因为在查看TCP API之后使用UDP特定API非常简单。大多数用户甚至可能会考虑使用现有的UDP库。很多人都会编写自己的TCP包装器然后编写库，而由于协议的性质，很少有人根据自己的需要编写自己的UDP。根据一个人的应用和网络经验，决定会有所不同。

作为目标＃3的一部分，我们还想编写一个自定义的“数据包”类来使用。仅仅因为增加的复杂性，这样的类不包括在这些示例中。理想情况下，我们希望有一些可重用的东西可以构建和重用，但根据应用程序，这可能是不可能的。例如，较大规模的应用程序可能需要使用自定义内存分配接口的代码，因此之前编写的任何代码可能不兼容。这些是进行网络编程时要记住的重要事项！

最后，我们必须考虑对我们来说是boost :: asio。对于某些应用程序，它可能会被过度杀死。对于其他人，我们可能决定要使用其他库。我自己开始喜欢boost :: asio的设计，并愿意接受boost :: bind带来的开销，以提高库提供的灵活性和功能。虽然获得“完美”代码需要大量工作和测试，但它仍然可行。但是，其他一些库可能会提供更多功能和适合任务的API集。

现在已经完成了关于boost :: asio入门的指南。将编写未来指南以涵盖更多主题。请随时留下任何反馈，更正或问题。


11

   


30评论
1 2 下一个 第1页，共2页  


Imgelling 222
发布 2011年2月1日
我没有读过所有内容，但它看起来很有帮助。在我目前的项目中，我刚刚开始研究boost：asio for networking，这将给我一个良好的开端。谢谢！
1

TTH 106
发布 2011年2月1日
谢谢你的那篇文章！我已经在使用boost :: asio（因为它很棒）但是我并不知道关于通过链序列化的工作的“无序vs有序”问题。如果我错了请纠正我，但据我所知，我可以确保在更改这些行时在示例4b中进行排序...
[code] io_service-> post（strand.wrap（boost :: bind（＆PrintNum，1） ）））; [/ code]
...像这样...
[code] strand.post（boost :: bind（＆PrintNum，1））; [/ code]
...，对吧？！
2

Drew_Benton 1862年
发布 2011年2月1日
[quote name ='TTH'timestamp ='1296588474']
谢谢你的那篇文章！我已经在使用boost :: asio（因为它很棒）但是我并不知道关于通过链序列化的工作的“无序vs有序”问题。如果我错了请纠正我，但据我所知，我可以确保在更改这些行时在示例4b中进行排序...
[code] io_service-> post（strand.wrap（boost :: bind（＆PrintNum，1） ）））; [/ code]
...像这样...
[code] strand.post（boost :: bind（＆PrintNum，1））; [/ code]
...，对吧？！
[/引用]

正确！您必须通过链本身来发布以确保显式排序，而如果您只是换行，则确保通过该链进行序列化（不保证实际顺序）。

我也没有真正意识到这个问题，直到它出现在我的一个程序中，并且男孩跟踪是一件痛苦的事情。我的意思是文档确实清楚地解释了这一点，但这是我完全误解的内容所以我要指出每个人都要小心它并仔细阅读文档！

[quote]
注意在以下情况中：

async_op_1（...，s.wrap（a））;
async_op_2（...，s.wrap（b）;
第一个异步操作的完成将执行s.dispatch（a），第二个将执行s.dispatch（b），但未指定执行这些操作的顺序。也就是说，你无法说明一个是否发生在另一个之前。因此，不满足上述条件，也不进行订购保证。


[/ quote]


在例4b中，async_op_1将是“io_service-> post（strand.wrap（boost :: bind（＆PrintNum，1）））;” 和async_op_2将是“io_service-> post（strand.wrap（boost :: bind（＆PrintNum，2）））;“。后见之明是20/20，但是只有在你犯错之后或者有人提出它真的很突出，假设你之前不理解它（我确定很多资深程序员和文档读者都不会犯这个错误，但毕竟我们都是人。

__Homer__ 58
发布 2011年2月5日
嗨!! 很棒的文章，非常丰富的信息

我之前使用Windows IOCP api编写了一个IOCP网络引擎，它通过可插入的抽象eventink类实现了每个连接协议处理，你的实现让我想起了很多我的iocp框架，但事件除外调度...

我对最终的示例实现有点困惑，工作线程池突然消失了？
我希望看到下一个令人兴奋的剧集实现线程池 - 我是否正确地认为它属于Hive类，或者派生的MyHive？如果你能澄清这一点，我将不胜感激。

我还想看到一个在一个引擎盖下实现客户端和服务器功能的包装器类，我可以想到服务器应用程序需要进行出站连接的几种情况。一个简单的socksv4代理服务器将是一个很好的示例实现

__Homer__ 58
发布 2011年2月5日
我想我看到一个小问题：<br> <br>当我检查调试输出时，它说明发送了18个字节，但只有12个被回送 - 在两种情况下都显示十六进制输出显示正确的18字节数量当我在OnSend和OnRecv处理程序中注释掉“十六进制输出”代码时，连接的两端都正确地报告了18个字节被回显。<br>尽管客户端和服务器在不同的应用程序中执行，尽管全局锁定调试输出，所以它似乎不是一个线程问题。<br> <br>什么可能导致缓冲区向量的长度以这种方式被操纵？<br> <br>

__Homer__ 58
发布 2011年2月5日
是的，＆nbsp; 在MyConnection.OnRecv和OnSend中，我将其替换为：<br>＆nbsp;＆nbsp;＆nbsp; ＆NBSP;＆NBSP;＆NBSP; std :: cout＆lt;＆lt; “[”＆lt;＆lt; __FUNCTION__＆lt;＆lt; “]”＆lt;＆lt; buffer.size（）＆lt;＆lt; “bytes”＆lt;＆lt; std :: endl; <br> <br>包含以下内容：<br>＆nbsp;＆nbsp;＆nbsp; ＆NBSP;＆NBSP;＆NBSP; char blah [50]; <br>＆nbsp;＆nbsp;＆nbsp; ＆NBSP;＆NBSP;＆NBSP; sprintf_s（blah，“％d bytes \ n”，buffer.size（））; <br>   p;＆nbsp;＆nbsp; std :: cout＆lt;＆lt; “[”＆lt;＆lt; __FUNCTION__＆lt;＆lt; “]”＆lt;＆lt; 等等;搜索结果

Drew_Benton 1862年
发布 2011年2月8日
[quote]我对最终的示例实现有点困惑，工作线程池突然消失了？[/ quote]

是的，仅仅因为9b中显示的客户端应用程序不需要真正的。在这种情况下，我想要一个能够通过按键退出的简单示例（抱歉，仅限Windows的示例！）并且没有使用线程池，因为它不需要。对不起，我没有澄清这一点，但这只是个人偏好的例子。

[quote]我希望看到下一个令人兴奋的剧集实现线程池 - 我是否正确地假设它属于Hive类，或者派生的MyHive？如果你能澄清一下，我将不胜感激。[/ quote]

随着线程的扩展，没有更多的东西可以扩展。自定义Hive类将根据需要使用您自己的方法扩展对象，因此它们都被包装到一个对象中。然后，您可以使用boost :: dynamic_pointer_cast将shared_ptr基类型更改为派生类型。

[quote]我还想看到一个在一个引擎盖下实现客户端和服务器功能的包装器类，我可以想到一些服务器应用程序需要进行出站连接的情况。一个简单的socksv4代理服务器将是一个很好的示例实现[/ quote]

更多示例正在对网络包装器代码进行一些修复。我在很多不同的设置中积极使用代码，所以我将涵盖所有实际的基础。我现在代码的主要用途实际上与您请求的内容类似，这是一个代理。不用说，我觉得它在实践中总体上非常好用。

Rutix 112
发布 2011年3月21日
好东西。虽然我已经知道很多这里讨论的内容，但我认为这对刚开始的人来说是个不错的指南。做得很好

Emmanuel Deloget 1381
发布 2011年9月1日
我很想看到这篇文章写得很好（即胶乳化）的PDF版本。真的很棒。

FrEEzE2046 106
发布 2011年9月12日
首先，我非常喜欢这个教程。它很好地讨论了boost :: asio的基础知识。
但是，我想在您的代码中更改几件事。还有其他机会可以让我们创建更具可读性和更高性能的代码。我创建了一个示例并记录了我所做的大部分更改：

[code] <boost / asio.hpp >
<boost / bind.hpp >
<boost / shared_ptr.hpp >
<boost / thread .hpp >
<boost / thread / mutex.hpp >

<iostream>


void worker_thread（boost :: asio :: io_service＆，boost :: mutex＆）;




boost :: mutex互斥;
boost :: asio :: io_service io_service;
boost :: asio :: io_service :: work work（io_service）;

//请注意：你不需要锁定对`std :: cout`的访问，因为没有其他线程同时
//试图修改对象
std :: cout <<“thread [”<< boost :: this_thread :: get_id（）<<“]按[任意键]退出。” << std :: endl;

boost :: thread_group worker_threads;
for（int i = 0; i <4; ++ i）
{
//请注意：你可以传递对worker_thread（）函数的引用，即使你想用它来
与boost :: bind创建的函数对象一起使用它（）。你唯一需要做的就是
//使用boost :: ref或boost :: cref将对象包装在reference_wrapper中。
//但是，您不再需要管理任何全局对象，也不必担心
//复制可构造性。
worker_threads.create_thread（
boost :: bind（worker_thread，boost :: ref（io_service），boost :: ref（mutex）））;
}

//现在有在后台运行多个线程，我们需要警惕的访问
//我们worker_thread（）函数中使用任何全局对象。

std :: cin.get（）;

io_service.stop（）;

worker_threads.join_all（）;

返回0;
}


void worker_thread（boost :: asio :: io_service＆io_service，boost :: mutex＆mutex）
{
{
//我们可以限制lock_guard的范围。因此，更容易看到哪些代码位置
被保护。
boost :: lock_guard <boost :: mutex> lock（互斥锁）;
std :: cout <<“thread [”<< boost :: this_thread :: get_id（）<<“] start”<< std :: endl;
}

io_service.run（）;

{
boost :: lock_guard <boost :: mutex> lock（互斥锁）;
std :: cout <<“thread [”<< boost :: this_thread :: get_id（）<<“]完成”<< std :: endl;
}
}
[/代码]

纳努克 537
发布 2012年4月29日
好文章！

你提到了快速代理，有没有办法使用它们与boost线程和asio io_service一起使用？我一直试图搞乱第四页上的最后一个问题..但是3个快速委托实现中没有一个似乎能像boost一样绑定，所以当我发布或发送作业时我无法提供参数..如果我使用没有参数的那个，我可以使用快速代理。

ilbardosilente 101
发布 2012年7月7日
大家好，
是否有本指南的可打印版本？

谢谢

专家 105
发布 2012年9月29日
小记。在示例示例2e中，您可以使用std：ref使其更简洁。然后示例将看起来像

[CODE]
void WorkerThread（boost :: asio :: io_service＆io_service）
{
std :: cout <<“Thread Start \ n”;
io_service.run（）;
std :: cout <<“Thread Finish \ n”;
}
int main（int argc，char ** argv）
{
boost :: asio :: io_service io_service;
boost :: asio :: io_service :: work work（io_service）;
std :: cout <<“按[return]退出。” << std :: endl;
boost :: thread_group worker_threads;
for（int x = 0; x <4; ++ x）
{
worker_threads.create_thread（boost :: bind（＆WorkerThread，std :: ref（io_service）））;

cin.get（）;
io_service.stop（）;
worker_threads.join_all（）;
返回0;
}
[/ CODE]

专家 105
发布 2012年10月1日
同样在样品8a链中创建但从未使用过。

howie_007 285
发布 2012年10月22日
我真的很喜欢这个指南，直到你把网络封装器放在我们身上作为一个突然的结局。在小块中最好理解Boost，因为在我看来它不可读。网络包装器甚至不遵循您到目前为止所展示的内容。什么是蜂巢？这就像是一部好动漫的糟糕结局。

lucastamoios 104
发布 2012年12月30日
自从我开始学习boost.asio以来，这是我发现的最好的帖子，考虑到我是这个主题的新手。我想祝贺你，我的朋友，还要感谢这位伟大的职位。我正在考虑用葡萄牙语制作这个主题的简单版本，我想知道你是否让我从这里展示一些样本 - 当然，我将包括来源。
谢谢。

mwpowellhtx 106
发布 2013年2月13日
好吧，我承认我正在刷新一些生锈的C ++，对启动和模板开发有一些熟悉，但它已经有一段时间了。话虽如此，并没有像早期提到的那样寻找进修课程。我确实有一个问题，我正在设计一个“外围设备”网络基础设施，最终将在嵌入式设备上运行，但是现在想要将感兴趣的外围设备暴露给Windows主机程序。
基本上，我们每个外设都有一个io_service（可能）（如果我们需要控制和数据（响应）套接字可能会有两个不同。这样可以保持IO关注点整齐分离，如果它不是太忙了Asio内部。
那么......我正在阅读的“工作”和“股”这个概念是什么？写作很简单，但是读取，阻止或轮询的不对称性，是否与异步读取一起使用。我假设要解析我们收到的任何响应协议？然后用它做一些事情，提交给事件经纪人或类似的东西，已收到外围响应？
如果你愿意的话，需要一些帮助来掌握这种魔力。谢谢！

mwpowellhtx 106
发布 2013年2月13日

我有点困惑这个。这里有什么真正的工作要做？换句话说，这个想法贯穿了我的脑海，“所以我们绑定了工作线程，所以我们将io_service作为一个参数传递给所有人？” 换句话说，这里真正的工作是什么？或者何时适合这样做？
退后一步，还有“简单”的用例，对吗？将一些数据，控制，请求等写入服务器。从客户端（或作为客户端到另一台服务器）读取一些数据，控制，响应等等？

小记。在示例示例2e中，您可以使用std：ref使其更简洁。然后示例将是
void WorkerThread（boost :: asio :: io_service＆io_service）
{
std :: cout <<“Thread Start \ n”;io_service.run（）;
std :: cout <<“Thread Finish \ n”;
}
int main（int argc，char ** argv）
{
boost :: asio :: io_service io_service;
boost :: asio :: io_service :: work work（io_service）;
std :: cout <<“按[return]退出。” << std :: endl;
boost :: thread_group worker_threads;
for（int x = 0; x <4; ++ x）
{
worker_threads.create_thread（boost :: bind（＆WorkerThread，std :: ref（io_service）））;
}
std :: cin.get（）;
io_service.stop（）;
worker_threads.join_all（）;
返回0;
}
1

mwpowellhtx 106
发布 2013年2月13日
啊! 现在我开玩笑！或者我开始了。这里的例子对我很有启发。因此，耦合实际上与设置io_service一次有关，除了适当的发布或调度之外，关键是在工作线程上运行io_service。这为无缝（希望）非阻塞操作打开了整个过程。

mwpowellhtx 106
发布 2013年2月14日


在第8页，也许我错过了关于boost :: asio :: ip :: tcp :: resolver :: query的一些内容？例7a有主机地址作为网址吗？或者，如果我们连接到非DHCP映射地址，IP地址是否可行？然后是一个词汇演员从80（端口？）到字符串？出于什么目的？这是什么查询称为“主机”？虽然地址被称为“服务”？我很困惑。

mwpowellhtx 106
发布 2013年2月14日
我试图遵循异步客户端连接示例，虽然这不是一个关于boost :: bind的教程，但本身，我遇到了问题。我正在构建一个类层次结构来处理基于套接字的IPC系统所需的各种场景，并且我有以下类型场景。
void async_socket_client :: connect（const boost :: asio :: ip :: tcp :: endpoint＆ep）{//将Socket与连接的回调异步连接到TCP / IP端点。m_sp_socket-> async_connect（ep，boost :: bind（async_socket_client :: connected，_1，m_sp_socket））;}
并且connected方法接受我的shared_ptr <socket>：
virtual void connected（boost :: shared_ptr <boost :: asio :: ip :: tcp :: socket> sp_socket）覆盖{/*...*/}
然而，我在这一个上得到了大量的编译器错误，接着是看起来像大量混乱试图解决绑定符号（宏，模板函数，无论它是什么）：
错误1错误C3867：'our :: io :: socket_client_base :: connected'：函数调用缺少参数列表; 使用'＆our :: io :: socket_client_base :: connected'创建指向成员c：\ source \ our \ device \ our-device \ our-asio \ async_socket_client.cpp的指针 22 1 our-asio

到目前为止，这一切都运行良好。另一个与boost :: bind有关的例子需要一个静态运行方法。然后绑定工作。这里的区别在于我们有一个我想要绑定的成员函数（并希望继续以这种方式公开）。或者我只是按原样给它一个成员函数？

我可能在语法上做了一些愚蠢的事情，但我想知道，这是一个编译器问题：我们现在正在使用Visual Studio 2010 SP1进行构建。对于C / C ++而言，也可能是我的生锈。

*编辑*好的，首先，需要至少公开访问回调才能工作。有道理所以我明确说明了成员函数，命名空间，类名和所有的路径。现在回调是否强制要求error_code？我看到OnConnect有，但是回调需要吗？

mwpowellhtx 106
发布 2013年2月14日
我试图遵循异步客户端连接示例，虽然这不是一个关于boost :: bind的教程，但本身，我遇到了问题。我正在构建一个类层次结构来处理基于套接字的IPC系统所需的各种场景，并且我有以下类型场景。
void async_socket_client :: connect（const boost :: asio :: ip :: tcp :: endpoint＆ep）{//将Socket与连接的回调异步连接到TCP / IP端点。m_sp_socket-> async_connect（ep，boost :: bind（async_socket_client :: connected，_1，m_sp_socket））;}
并且connected方法接受我的shared_ptr：
virtual void connected（boost :: shared_ptr sp_socket）覆盖{/*...*/}
然而，我在这一个上得到了大量的编译器错误，接着是看起来像大量混乱试图解决绑定符号（宏，模板函数，无论它是什么）：
错误1错误C3867：'our :: io :: socket_client_base :: connected'：函数调用缺少参数列表; 使用'＆our :: io :: socket_client_base :: connected'创建指向成员c：\ source \ our \ device \ our-device \ our-asio \ async_socket_client.cpp的指针 22 1 our-asio
到目前为止，这一切都运行良好。另一个与boost :: bind有关的例子需要一个静态运行方法。然后绑定工作。这里的区别在于我们有一个我想要绑定的成员函数（并希望继续以这种方式公开）。或者我只是按原样给它一个成员函数？我可能在语法上做了一些愚蠢的事情，但我想知道，这是一个编译器问题：我们现在正在使用Visual Studio 2010 SP1进行构建。对于C / C ++而言，也可能是我的生锈。*编辑*好的，首先，需要至少公开访问回调才能工作。有道理所以我明确说明了成员函数，命名空间，类名和所有的路径。现在回调是否强制要求error_code？我看到OnConnect有，但是回调需要吗？

mwpowellhtx 106
发布 2013年2月14日
问题回答了。显然我不知道，有一个隐含的error_code参数，就像所说的例子一样！咄！谢谢！谢谢！

Shenjoku 161
发布 2013年9月1日
你有没有开始创建服务器+客户端示例？我几天试图设置一个只在127.0.0.1:8888上连接的简单代理应用程序，几乎没有对数据做任何事情，并将其发送到需要去的任何地方。看起来这可能正是我一直在寻找的，如果它只是更完整。

pnraj 110
发布 2015年5月10日

我从德鲁的文章中受益匪浅。我现在陷入了一个特定的问题，没有找到任何线索。有人可以帮忙！就像这样。我想知道是否有一种方法可以在线程中执行函数，以访问复制到线程结构中的值。
让我用示例2e解释，通过添加int的参数进行修改。
Boost doc告诉我“int x”将被复制到线程的内部结构中。现在，有没有办法发布到线程池的处理程序可以访问它运行的特定线程的“myThreadnum”的值？
换句话说，有没有一种方法可以让线程在其内部结构中保存值/对象，可以通过线程上的runnables访问？

void WorkerThread （boost :: asio :: io_service ＆io_service，int x）{ int myThreadNum = x;

int myThread-num = i;                    

std :: cout << “Thread Start \ n” ; io_service 。run （）; std :: cout << “Thread Finish \ n” ; } int main （int argc ，char ** argv ）{ boost :: asio :: io_service io_service ; boost :: asio :: io_service :: work work （io_service ）; std :: cout <<







“按[返回]退出。” << std :: endl ; boost :: thread_group worker_threads ; for （int x = 0 ; x < 4 ; ++ x ）{ worker_threads 。create_thread （boost :: bind （＆WorkerThread ，std :: ref （io_service，x ）））; } std :: cin 。get （）;





io_service 。停止（）;
worker_threads 。join_all （）;
返回0 ; }
非常感谢！
1 2 下一个 第1页，共2页  
创建一个帐户或登录进行评论
您需要成为会员才能发表评论
创建一个帐户
在我们的社区中注册一个新帐户。这很简单！
注册一个新帐户
登入
已经有账号？在这里登录。
现在
广告
广告
博客条目

下一轮挑战民意调查！
通过 khawk的挑战     37

GC：爆炸球（游戏）
作者： GoliathForge在ѾAHobbyistTaléΔΔ     20

Ping，Zap，Zing！侧滚动街机射击挑战 - 没有武器！
通过 khawk的挑战     19

Boom先生的海运集装箱
由 鲁丁在艺术作品     8

挑战1 - 3D国际象棋 - 第1部分
由 鲁丁参加比赛挑战赛     8
广告
GDNet聊天 所有活动 家  博客  他们不在学校教这些东西  boost :: asio入门指南
GameDev.net
资源
文章和教程
博客
日历
论坛
画廊
社区
活动
方针
GameDev市场
GameDev Jobs
排行榜
关于
关于我们
服务条款
隐私政策
联系我们
社会
版权所有©1999-2019 GameDev.net，LLC
重要信息
使用GameDev.net，即表示您同意我们的社区准则，使用条款和隐私政策。
