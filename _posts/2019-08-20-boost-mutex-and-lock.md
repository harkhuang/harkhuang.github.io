[read lock  and share lock]https://stackoverflow.com/questions/989795/example-for-boost-shared-mutex-multiple-reads-one-write



这里没有使用新的特性 要指出来一点  是因为没有能够使用raii 自动释放局部变量来达到unlock的目的


eg1:
```
boost::shared_mutex _access;
void reader()
{
  // get shared access
  boost::shared_lock<boost::shared_mutex> lock(_access);

  // now we have shared access
}

void writer()
{
  // get upgradable access
  boost::upgrade_lock<boost::shared_mutex> lock(_access);

  // get exclusive access
  boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
  // now we have exclusive access
}


```


eg2:清晰的条件变量写的例子

```
boost::shared_mutex _access;
void reader()
{
  boost::shared_lock< boost::shared_mutex > lock(_access);
  // do work here, without anyone having exclusive access
}


/// 条件变量 清晰可见
void conditional_writer()
{
  boost::upgrade_lock< boost::shared_mutex > lock(_access);
  // do work here, without anyone having exclusive access
  // 此处操作粒度明显粗于但存的写锁  速度相对较快


  // 升级锁为 独享写锁
  if (something) {
    boost::upgrade_to_unique_lock< boost::shared_mutex > uniqueLock(lock);
    // do work here, but now you have exclusive access
  }

  // do more work here, without anyone having exclusive access
}

void unconditional_writer()
{
    // 直接使用写锁
  boost::unique_lock< boost::shared_mutex > lock(_access);
  // do work here, with exclusive access
}

```