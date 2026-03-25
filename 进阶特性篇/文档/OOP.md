### 🧱 基石：类与对象

#### 核心概念
- **类**：这是**图纸**。比如“汽车设计图”，它定义了汽车应该有什么颜色、什么品牌，能做什么（跑、刹车）。
- **对象**：这是**实物**。根据图纸造出来的具体的一辆车（比如一辆红色的法拉利）。

#### 关键点
- `class` 关键字定义类。
- 成员变量（属性）和成员函数（行为）。
- **封装**的雏形：把数据和方法捆绑在一起。

#### 代码演示
```cpp
#include <iostream>
#include <string>
using namespace std;

class Car {
public:
    string brand;  // 属性：品牌
    int speed;     // 属性：速度

    // 行为：跑
    void run() {
        cout << brand << " 正在以 " << speed << " km/h 行驶" << endl;
    }
};

int main() {
    Car myCar;          // 创建对象
    myCar.brand = "保时捷";
    myCar.speed = 200;
    myCar.run();        // 调用行为
    return 0;
}
```

---

### 🛡️ 护城河：访问权限与封装

#### 核心概念
为了保护对象内部的数据不被随意篡改，C++ 设立了三个等级的“门禁”：

| 权限 | 形象比喻 | 访问范围 |
| :--- | :--- | :--- |
| **public** | 广场 | 谁都能进（类内、类外都能访问） |
| **protected** | 员工通道 | 自己人和亲戚（子类）能进，外人不行 |
| **private** | 卧室 | 只有自己人能进（外部无法直接访问） |

#### 代码演示
```cpp
class Person {
public:
    string name;      // 公开：名字谁都可以叫
protected:
    int age;          // 保护：年龄只有自己和子女知道
private:
    string password;  // 私有：密码只有自己能改

public:
    void setPassword(string pwd) {
        password = pwd; // 类内部可以访问私有成员
    }
};
```

---

### 🧬 三大特性：封装、继承、多态

#### 封装
- **概念**： hide the complexity（隐藏复杂性）。把属性和方法打包，只暴露必要的接口。
- **作用**： 减少耦合，提高安全性。

#### 继承
- **概念**： 儿子继承父亲的财产。
- **作用**： 代码复用。子类自动拥有父类的属性和方法。

#### 多态
- **概念**： 同一个接口，不同的表现形式。比如“动物叫”，猫是“喵喵”，狗是“汪汪”。
- **实现条件**：
    1. 有继承关系。
    2. 父类指针（或引用）指向子类对象。
    3. 函数必须是**虚函数**。

#### 代码演示（继承与多态）
```cpp
// 父类
class Animal {
public:
    // 虚函数是实现多态的关键
    virtual void speak() {
        cout << "动物在说话" << endl;
    }
};

// 子类
class Cat : public Animal {
public:
    void speak() override { // override表示重写
        cout << "小猫在喵喵叫" << endl;
    }
};

class Dog : public Animal {
public:
    void speak() override {
        cout << "小狗在汪汪叫" << endl;
    }
};

void makeSound(Animal* animal) { // 父类指针
    animal->speak(); // 运行时决定调用哪个speak
}

int main() {
    Cat c;
    Dog d;
    makeSound(&c); // 输出：小猫在喵喵叫
    makeSound(&d); // 输出：小狗在汪汪叫
    return 0;
}
```

---

### 🏗️ 生命周期：构造与析构

#### 核心概念
- **构造函数**： 出生证明。对象创建时自动调用，用于初始化。
- **析构函数**： 死亡证明。对象销毁前自动调用，用于清理资源（如释放内存）。
- **深拷贝与浅拷贝**：
    - **浅拷贝**： 只是简单的赋值，指针指向同一块内存（容易导致重复释放错误）。
    - **深拷贝**： 在堆区重新申请内存，把数据复制过去（彻底独立）。

#### 代码演示
```cpp
class Student {
public:
    string name;
    int* score; // 堆区指针

    // 构造函数
    Student(string n, int s) {
        name = n;
        score = new int(s); // 堆区分配内存
        cout << "Student 构造" << endl;
    }

    // 拷贝构造函数 (深拷贝演示)
    Student(const Student& p) {
        name = p.name;
        score = new int(*p.score); // 重新开辟内存，而不是复制指针
        cout << "Student 深拷贝构造" << endl;
    }

    // 析构函数
    ~Student() {
        if (score != nullptr) {
            delete score;
            score = nullptr;
        }
        cout << "Student 析构" << endl;
    }
};
```

---

### 🤝 特殊关系：友元

#### 核心概念
- **友元**： 也就是“特权朋友”。
- 正常情况下，私有成员外人不能看。但如果你把某个函数或类声明为 `friend`，它就可以无视 `private` 权限，直接访问私有成员。
- **缺点**： 破坏了封装性，慎用。

#### 代码演示
```cpp
class House {
private:
    string secret;

public:
    House() { secret = "藏宝图"; }

    // 声明全局函数为友元
    friend void visit(House& h);
};

// 这个函数不是成员函数，但因为是友元，可以访问私有成员
void visit(House& h) {
    cout << "朋友看到了：" << h.secret << endl;
}
```

---

### ⚙️ 语法糖：运算符重载

#### 核心概念
- 让自定义的数据类型（类）也能像 `int` 或 `double` 一样使用 `+`, `-`, `<<` 等符号。
- 本质是函数重载，函数名是 `operator` 加上符号。

#### 代码演示
```cpp
class Complex {
public:
    int real; // 实部
    int imag; // 虚部

    Complex(int r, int i) : real(r), imag(i) {}

    // 重载 "+" 号
    Complex operator+(const Complex& other) {
        return Complex(this->real + other.real, this->imag + other.imag);
    }

    // 重载 "<<" 用于打印 (通常需要友元)
    friend ostream& operator<<(ostream& out, const Complex& c);
};

ostream& operator<<(ostream& out, const Complex& c) {
    out << c.real << " + " << c.imag << "i";
    return out;
}

int main() {
    Complex c1(1, 2);
    Complex c2(3, 4);
    Complex c3 = c1 + c2; // 像数字一样相加
    cout << c3 << endl;   // 输出: 4 + 6i
    return 0;
}
```

---

### 🎭 抽象类与虚函数进阶

#### 纯虚函数与抽象类
- **纯虚函数**： 只有声明没有实现的虚函数。语法：`virtual void func() = 0;`
- **抽象类**： 只要类里有一个纯虚函数，这个类就是抽象类。
- **规则**： 抽象类**不能实例化对象**（你不能创建一个“动物”对象，只能创建“猫”或“狗”）。它强制子类必须重写该函数。

#### 虚析构函数
- **问题**： 如果父类指针指向子类对象（多态），删除指针时，如果析构函数不是虚的，只会调用父类的析构，导致子类特有的堆区内存泄漏。
- **解决**： 将父类的析构函数加上 `virtual`。

#### 代码演示
```cpp
class Shape {
public:
    // 纯虚函数，强制子类计算面积
    virtual double getArea() = 0;

    // 虚析构函数，保证多态删除时安全释放
    virtual ~Shape() {
        cout << "Shape 析构" << endl;
    }
};

class Circle : public Shape {
    double r;
public:
    Circle(double radius) : r(radius) {}
    
    double getArea() override {
        return 3.14 * r * r;
    }

    ~Circle() {
        cout << "Circle 析构" << endl;
    }
};

int main() {
    // Shape s; // 错误！抽象类不能实例化
    Shape* s = new Circle(10.0);
    
    cout << "面积: " << s->getArea() << endl;
    
    delete s; // 正确调用 Circle 的析构，然后调用 Shape 的析构
    return 0;
}
```

---
