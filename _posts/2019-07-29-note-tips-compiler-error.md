
### 编译故障记录


###
在protobuf中使用  
``` #pragma pack(push,1)  ```  或者 ```#pragma pack(1)  ```

 序列化过程   在计算pb序列大小时候  就一顿报错.
 等死吧  没救了  :( 


 如果你使用网络通信  使用这行代码避免对齐 那么在windows release 条件下可能会报错
  这个问题查了很久,才找到结果,就知道是个很坑爹的问题,浪费时间却没有提升~   
 问题分析还是选择  ***排除对比法***   一点点比较  找到错误代码行 接近这个问题

```
#pragma pack(push,1)
void test_pb()
{

    aa::bb::Response resp_body;
    for (int i = 1; i < 2 + 1; ++i)
    {

        aa::bb::Response::detail* detail = resp_body.add_detail_list();

        detail->set_freeze_amount("");
        detail->set_clear_amount("");
        detail->set_total_cancelled_quantity(0);
    }

    // 此处序列化 可能会因为pack(1) 报错
    std::string xxx = resp_body.SerializeAsString();


}
```