
条件变量
https://thispointer.com/c11-multithreading-part-7-condition-variables-explained/

条件变量官网的用法 细细品味
https://en.cppreference.com/w/cpp/thread/condition_variable


C11 并发编程之条件变量
https://thispointer.com/c11-multithreading-part-7-condition-variables-explained/
这个章节有空得给它读下来


不能解锁的锁
Lock_guard 和 uniqu_lock. 区别
http://jakascorner.com/blog/2016/02/lock_guard-and-unique_lock.html




```
#include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
using namespace std::placeholders;

class Application
{
  std::mutex m_mutex;
  std::condition_variable m_condVar;
  bool m_bDataLoaded;  
public:
  Application()
  {
    m_bDataLoaded = false;
  } 
  void loadData()
  {
   // Make This Thread sleep for 1 Second
   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   std::cout<<"Loading Data from XML"<<std::endl;
   // Lock The Data structure
   
   // 守卫锁
   std::lock_guard<std::mutex> guard(m_mutex);
   // Set the flag to true, means data is loaded
   m_bDataLoaded = true;
   // Notify the condition variable
   
   // 通知一个线程开始
   m_condVar.notify_one();
  }
  bool isDataLoaded()
  {
    return m_bDataLoaded;
  }
  void mainTask()
  {
    std::cout<<"Do Some Handshaking"<<std::endl;
    // Acquire the lock
    std::unique_lock<std::mutex> mlock(m_mutex);
    // Start waiting for the Condition Variable to get signaled
    // Wait() will internally release the lock and make the thread to block
    // As soon as condition variable get signaled, resume the thread and
    // again acquire the lock. Then check if condition is met or not
    // If condition is met then continue else again go in wait.
    m_condVar.wait(mlock, std::bind(&Application::isDataLoaded, this));
    std::cout<<"Do Processing On loaded Data"<<std::endl;
  }
  
};
int main()
{
   Application app;
   // 线程mainTask  应该在 loadDate后面执行的
   std::thread thread_1(&Application::mainTask, &app);
   
   // 内部线程锁 
   std::thread thread_2(&Application::loadData, &app);
   
   
   // 正常序列
   
   
   thread_2.join();
   thread_1.join();
   return 0;
}
```

std :: lock_guard和std :: unique_lock之间的区别
2016年2月8日

防止线程之间数据争用的一种方法是使用互斥锁。

互斥锁通常与资源相关联。锁定互斥锁的线程已授予对资源的访问权限。然后没有其他线程可以锁定互斥锁，因为它已被锁定（如下图所示）。因此，没有其他线程可以访问被锁定的互斥锁保护的资源。这是互斥：在任何给定时间只有一个线程可以访问资源。

互斥

我们已经谈到了在代码中使用互斥锁时出现的问题：记住Mutex和死锁。在那里，我们介绍了这门std::lock_guard课程。但是，当我们同步的线程有一个条件变量，我们使用了类似的类： std::unique_lock。这两个班级有什么区别？

区别
std::lock_guard和 之间的区别之一std::unique_lock是程序员能够解锁 std::unique_lock，但她/他无法解锁 std::lock_guard。让我们更详细地解释一下。

的std :: lock_guard
如果你有一个对象

std::lock_guard guard1(mutex);
然后guard1锁定互斥锁的构造函数。在guard1生命结束时 ，析构函数解锁互斥锁。没有其他可能性。实际上，std::lock_guard该类没有任何其他成员函数。

的std :: unique_lock
另一方面，我们有一个对象std::unique_lock。

std::unique_lock guard2(mutex);
与std::lock_guard班级有相似之处。构造函数guard2也锁定互斥锁和析构函数guard2也解锁互斥锁。但它 std::unique_lock具有额外的功能。

程序员可以在保护对象的帮助下解锁互斥锁

guard2.unlock();
这意味着程序员可以在guard2生命结束之前解锁互斥锁 。互斥锁解锁后，程序员也可以再次锁定它

guard2.lock();
我们应该提一下，std::unique_lock 还有其他一些成员函数。你可以在这里查找 。

何时使用std :: unique_lock？
使用至少有两个原因 std::unique_lock。有时我们被迫使用它：其他功能需要它作为输入。而其他时候使用 std::unique_lock允许我们有更多可并行化的代码。

更高的并行化
假设我们有很长的功能。该函数的第一部分访问某些共享资源，第二部分本地处理该资源。

std::vector< int > vector; // shared between threads

...

int function(...)
{
    ...
    Getting int from the shared vector.
    ...
       
    ...
    Long, complicated computation with int. 
    This part does not depend on the vector. 
    ... 

}
必须在函数的第一部分中锁定互斥锁，因为我们访问向量的元素。在第二部分中，不再需要锁定互斥锁（因为我们不访问任何共享变量）。

std::vector< int > vector; // shared between threads
std::mutex mutex; 

...

int function(...)
{
    ...
    std::unique_lock guard(mutex);
    Getting int from the shared vector.
    ...
       
    ...
    guard.unlock();
    Long, complicated computation with int. 
    This part does not depend on the vector. 
    ... 

}
实际上，最好是互斥锁没有锁定在第二部分中，因为其他线程可以锁定它。原则上，我们希望锁具尽可能少的时间。这样可以最大限度地减少线程等待锁定互斥锁而不执行任何有用工作的时间。我们获得更多可并行化的代码。

使用需要std :: unique_lock的函数
在Condition变量中，我们不得不使用std::unique_lock，因为 std::condition_variable::wait(...)需要 std::unique_lock作为输入。

在std::condition_variable::wait(...)解锁互斥量及等待std::condition_variable.notify_one()的成员函数调用。然后，wait(...)重新获取锁定并继续。

我们认识到wait(...)成员职能需要 std::unique_lock。该函数不能正常使用 std::lock_guard，因为它解锁/锁定互斥锁。

何时使用std :: lock_guard？
在std::unique_lock拥有所有的功能的 std::lock_guard。所有可能做的事情 std::lock_guard也可以做 std::unique_lock。那么，我们std::lock_guard什么时候应该使用 ？

经验法则是始终使用std::lock_guard。但是如果我们需要一些更高级别的功能 std::unique_lock，那么我们应该使用 std::unique_lock。

摘要
我们了解了std::lock_guard和之间的差异std::unique_lock。我们还列出了一些我们应该使用的情况std::unique_lock。

链接：

的std :: lock_guard
的std :: unique_lock




```
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
 
std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;
 
void worker_thread()
{
    // Wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return ready;});
 
    // after the wait, we own the lock.
    std::cout << "Worker thread is processing data\n";
    data += " after processing";
 
    // Send data back to main()
    processed = true;
    std::cout << "Worker thread signals data processing completed\n";
 
    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();
}
 
int main()
{
    std::thread worker(worker_thread);
 
    data = "Example data";
    // send data to the worker thread
    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    cv.notify_one();
 
    // wait for the worker
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{return processed;});
    }
    std::cout << "Back in main(), data = " << data << '\n';
 
    worker.join();
}

```
