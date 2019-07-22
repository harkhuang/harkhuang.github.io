C++中实现数据共享的5种方式：

1. 使用局部变量和全局变量共享数据
使用局部变量能够在调用和被调用函数之问通过参数传递实现不同函数块之问的数据共享。
局部变量具有局部作用域，能很好地实现函数之间的数据隐蔽。
但在传递过程中需要很大的系统开销，故一般只用于传递少量的数据。
全局变量具有文件作用域。全局变量所存储的数据在程序中任何地方都可以访问，
能够在程序中的所有函数之间实现数据共享。使用全局变量实现共享数据相当方便，
但其副作用也相当大。因为全局变量无法对数据的访问权进行有效控制。
也就是说，我们很难知道程序中数据在那些函数中共享，这样一旦共享的数据发生结构性调整，
也就很难判断它到底影响到哪些函数，从而给程序维护带来相当大的困难。
这种共享方式，直接影响到数据安全、程序代码重用和扩充。
所以，在程序设计中，应可能少的使用这种共享方式。



-----
2. 通过类的数据成员共享数据
C++中类内部封装的数据成员提供了同一类的函数成员之间进行数据共享机制 。
这种共享方式一方面实现了类内部函数的全面共享，同时也提供了通过设置适当的访问控制属性，
把共享只限制在类的范围之内，这样对类外来说，类的数据成员仍是隐藏的，
达到了共享与隐藏两全，解决了全局变量和局部变量所无法克服的矛盾。

例如：一个时钟类，该类中封装了私有数据成员有Hour，Minute，Second，在类的成员函数之间实现数据共享。在类的外面，对它们的访问只能通过类自身提供的外部接口进行，无法直接访问。

```
Class Clock//时钟类的声明
{
public：//#b部接口，公有数据成员函数
void SetTime(int NewH =0，int NewM =0，int News=0)；
void ShowTime()；
private：//私有数据成员 ．
int Hour，Minute，Second；//在类的成员函数之间共享
}；
```
------

3. 通过类的静态成员实现数据共享
   C++中使用静态成员可以实现同一类的不同对象之间共享数据 j。类的普通数据成员在类的每一个对象都有一个拷贝，就是说每个对象的同名数据成员可以分别存储不同数值，这就保证对象拥有自身区别其他对象的特征的需要。静态数据成员是类的数据成员的一种特例，采用static关键字来声明；每个类只有一个拷贝，由该类的所有对象共同维护和使用，从而实现了同一类的不同对象之间的数据共享。

例如：
```
#include <iostream>
using namespace std;
class sample
{
private:
static char m_sArray[10];
public:
       sample(){ cout << "default constructor! "<< endl;}
sample(sample & s){ cout <<"copy constructor! " << endl;}
       void show(void) { cout << m_sArray << endl;}
void input(void) { cin.get(m_sArray, 10);}
};
char sample::m_sArray[10] = "I am a engineer";
int main(void)
{
sample e1;
       e1.show();
       sample e2(e1);
e2.show();
e2.input();
e1.show();
}
```

运行结果如下：
```
default constructor!
I am a engineer
copy constructor!
I am a engineer
this is my job
this is my job
```

  静态成员变量m_sArray确实起到了在不同对象间共享的作用！不过由于其是静态属性，内存是在全局/静态区域开辟的，属于栈内存区，内存大小使用受限。如果能动态从堆中申请内存，则可以使用大内存空间了。

有一学生类：
```
class engineer
{
    Private:
    im ID :
    char nalne;
    static count;//静态数据成员，用来存放“人数”
    string name;
}
```
     如果程序中需要统计学生人数，这个数据存放在什么地方呢?若以类外的全局变量来存放，不能实现数据的隐藏，若在类中增加一个数据成员用以存放人数，必然在每一个对象中都存储一副本，这样不仅冗余，而且每个对象分别维护一个“人数”，势必造成数据的不一致性。因此，比较好的方案是在engineer类中增加一个静态数据成员。static count用来存放学生“人数”。

4. 通过友元机制共享数据

        在一个类中，可以利用关键字friend将别的模块(一般函数、其他类的成员函数或其他类)声明为它的友元，这样这个类中本来隐藏的信息就可以被友元访问j。如果友元是一般函数或类的成员函数，称为友元函数；如果友元是一个类，则称为友元类，友元类的所有成员函数都成为友元函数。

例如：

class B
{
    //B类的成员声明
    friend float f(B＆x，B＆y)；//友元函数声明
    friend class A；//声明A为B的友元类
}；
        在B类声明f函数为友元函数，则在f函数中通过对象名可直接访问B类所有的数据成员。同时在B类声明A类为友元类，则A类的所有成员函数都是B类的友元函数，都可以访问B类的私有和保护成员。采用友元类共享数据机制，使友元类成员函数可以通过对象名直接访问到隐藏的数据，从而使程序达到高效协调工作。在较为复杂的问题中，实现不同类之间的数据共享，友元类的使用也是必不可少的选择。友元在类之间、类与普通函数之间共享了内部封装的数据的同时，必然会对类的封装性带来一定的破坏。因此在程序设计中使用友元，要在共享和封装之间找到一个恰当的平衡点，从而达到提高程序效率同时，将程序隐患降来最低。

5. 指针成员变量与计数
```
#include <iostream>
using namespace std;
class sample
{
private:
    char *m_pArray;
    static int m_sCount;
    size_t m_size;
public:
    sample()
    {
        m_size = 20;
        m_pArray = new char[m_size];
        memset(m_pArray, 0, m_size);
        cout << "default constructor!"<< endl;
        m_sCount++;
    }
    sample(int n)
    {
        cout << "paraconstructor!"<< endl;
        m_sCount++;
        m_pArray = new char[n];
        m_size = n;
    }
    sample(sample & s)
    {
        cout << "copy constructor!" << endl;
        m_sCount++;
    if(s.m_pArray != NULL)
    {
        m_pArray = s.m_pArray;
    }
    else
    {
        m_pArray = NULL;
    }
}
~sample()
{
    m_sCount--;
if(0==m_sCount)
{
    delete[] m_pArray;
    m_pArray = NULL;
}
}
sample & operator=(const sample &s)
{
    if(m_pArray == s.m_pArray)
        return *this;
        m_sCount--;
    if(0==m_sCount)
    {
        delete[] m_pArray;
    }
        m_pArray = s.m_pArray;
        m_size = s.m_size;
        m_sCount++;
    }
    void show(void)
    {
        if( m_pArray != NULL)
        cout << m_pArray << endl;
    }
    void input(void)
    {
        if(m_size!=0)
        {
            if(m_pArray == NULL)
            {
                m_pArray = new char[m_size];
            }
            cin.get(m_pArray, m_size);
        }
    }
};
int sample::m_sCount=0;
int main()
{
    sample e1;
    e1.show();
    sample e2(e1);
    e2.show();
    e2.input();
    e1.show();
}
运行结果如下：
default constructor!
copy constructor!
i am a engineer
i am a engineer
```
   在大型程序设计中数据共享问题是一个复杂的综合性问题。设计过程中既要实现数据共享，又要保证共享数据的安全。这就要求程序设计人员必须充分了解各种实现机制的运行场合，熟知运用它们所带来的利与弊，这样才能恰到好处地运用这些机制，设计出高质量程序。