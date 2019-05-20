###  boost asio 前摄器模型 


![Alt text](proator.png "optional title")
`![](_posts/proactor.png)`
[proator.png]

```
  boost::shared_ptr< boost::asio::io_service > io_service(
      new boost::asio::io_service
  );
  boost::shared_ptr< boost::asio::io_service::work > work(
      new boost::asio::io_service::work( *io_service )
  );
  
  boost::shared_ptr< boost::asio::io_service::strand > strand(
      new boost::asio::io_service::strand( *io_service )
  );
  // 绑定超时的回调函数
  boost::shared_ptr< boost::asio::deadline_timer > timer(
      new boost::asio::deadline_timer( *io_service )
  );
  
  
  timer->expires_from_now( boost::posix_time::seconds( 1 ) );
  timer->async_wait(
      // 超时时间 属于对基础io读写时间的 包装 ，使用装饰者设计模式进行装饰
      strand->wrap( boost::bind( &TimerHandler, _1, timer, strand ) )
  );

  std::cin.get();

  io_service->stop();

  worker_threads.join_all();

```

##### strand->post


```
  // 有序的socket回调
  strand.post( boost::bind( &PrintNum, 1 ) );
  strand.post( boost::bind( &PrintNum, 2 ) );
  strand.post( boost::bind( &PrintNum, 3 ) );
  strand.post( boost::bind( &PrintNum, 4 ) );
  strand.post( boost::bind( &PrintNum, 5 ) );

  // 无顺序的socket回调
//  io_service->post( boost::bind( &PrintNum, 1 ) );
//  io_service->post( boost::bind( &PrintNum, 2 ) );
//  io_service->post( boost::bind( &PrintNum, 3 ) );
//  io_service->post( boost::bind( &PrintNum, 4 ) );
//  io_service->post( boost::bind( &PrintNum, 5 ) );


```
##### io_context
#####  boost_bind   
绑定函数

```
  void F1()
  {
    std::cout << __FUNCTION__ << std::endl;
  }


  int f(int a , int b){
    std::cout<<  a<< b <<std::endl;
  }

```

绑定并且调用函数

```

class MyClass
{
 public:
  void F3( int i, float f )
  {
    std::cout << "i: " << i << std::endl;
    std::cout << "f: " << f << std::endl;
  }
};


// 绑定并且调用
int test_bind_and_call( )
{
  MyClass c;
  // 绑定的是类  泛型编程实现
  boost::bind( &MyClass::F3, &c, 42, 3.14f )();

  return 0;
}
```

```
- ps 绑定调用课使用参数模板  用_1 , _2 表示引用参数
//bind(f, _1, 5)(x)等价于f(x, 5)，其中_1是一个占位符，表示用第一个参数来替换;
//bind(f, _2, _1)(x, y)等价于f(y, x);
//bind(g, _1, 9, _1)(x)等价于g(x, 9, x);
//bind(g, _3, _3, _3)(x, y, z)等价于g(z, z, z);
//bind(f, _2,_1)(x, y); //f(y, x)
//bind(g, _1, 9, _1)(x); //g(x, 9, x)
//bind(g, _3, _3, _3)(x, y, z); //g(z, z, z)
//bind(g, _1, _1, _1)(x, y, z); //g(x, x, x)
```


start boost (对库响应介绍，权威)
https://www.boost.org/doc/libs/1_62_0/more/getting_started/windows.html

boost asio toturial（对asio介绍，准确完整）
https://www.gamedev.net/blogs/entry/2249317-a-guide-to-getting-started-with-boostasio/
