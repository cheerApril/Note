/*
	Lambda表达式
	4.0 Lambda表达式或者Lambda函数为闭包结构。闭包是描述未命名对象的通用术语，也可以称为匿名函数。
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter4/chapter4-0-chinese.md
	4.1 使用Lambda表达式定义函数
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter4/chapter4-1-chinese.md
	！！主要是 capture list 
	[capture list] (parameters)
		mutable			(optional) -> 当函数对象需要去修改通过副本传入的变量时，表达式必须用mutable修饰。这就相当于对捕获的对象使用非常量函数。
		constexpr		(optional)
		exception attr	(optional)
		-> return type	(optional)
	{
		body
	}
	4.2 使用Lambda为std::function添加多态性
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter4/chapter4-2-chinese.md
	static: 
	A. 局部变量，没啥变化
	B. 静态全局变量仅对当前文件可见，其他文件不可访问，其他文件可以定义与其同名的变量，两者互不影响。
	C. 函数
	函数的使用方式与全局变量类似，在函数的返回类型前加上static，就是静态函数。其特性如下：
		1. 静态函数只能在声明它的文件中可见，其他文件不能引用该函数
		2. 不同的文件可以使用相同名字的静态函数，互不影响
	D. 静态数据成员
		1.静态数据成员存储在全局数据区，静态数据成员在定义时分配存储空间，所以不能在类声明中定义
		2.静态数据成员是类的成员，无论定义了多少个类的对象，静态数据成员的拷贝只有一个，且对该类的所有对象可见。也就是说任一对象都可以对静态数据成员进行操作。而对于非静态数据成员，每个对象都有自己的一份拷贝。
		3.由于上面的原因，静态数据成员不属于任何对象，在没有类的实例时其作用域就可见，在没有任何对象时，就可以进行操作
		4.和普通数据成员一样，静态数据成员也遵从public, protected, private访问规则
		5.静态数据成员的初始化格式：<数据类型><类名>::<静态数据成员名>=<值>
		6.类的静态数据成员有两种访问方式：<类对象名>.<静态数据成员名> 或 <类类型名>::<静态数据成员名>
		————————————————
		版权声明：本文为CSDN博主「guotianqing」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
		原文链接：https://blog.csdn.net/guotianqing/article/details/79828100
	E. 静态成员函数
		1. 静态成员函数没有this指针，它无法访问属于类对象的非静态数据成员，也无法访问非静态成员函数，它只能调用其余的静态成员函数
		2. 出现在类体外的函数定义不能指定关键字static
		3. 非静态成员函数可以任意地访问静态成员函数和静态数据成员
	4.3 并置函数
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter4/chapter4-3-chinese.md
	4.4 通过逻辑连接创建复杂谓词
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter4/chapter4-4-chinese.md
	4.5 使用同一输入调用多个函数
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter4/chapter4-5-chinese.md
	4.6 使用std::accumulate和Lambda函数实现transform_if
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter4/chapter4-6-chinese.md
	4.7 编译时生成笛卡尔乘积
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter4/chapter4-7-chinese.md
*/

#include<iostream>
void lambda_func1() {
	auto cout_func([] {
		std::cout << "hello, cheer\n";
	});
	cout_func();
}

#include<string>
void lambda_func2() {
	auto just_one{ [] {return 1;} };
	auto just_two{ []() {return 2;} };
	std::cout << "just_one: " << just_one() << " just_two: " << just_two() << "\n";
	auto plus{ [](int a, int b) {return a + b;} };
	std::cout << "plus(1, 2): " << plus(1, 2) << "\n";
	std::cout << "[](auto a, auto b){return a + b}('hello, ', 'cheer')" 
		<< [](auto str1, auto str2) {return str1 + str2;}(std::string{"hello "}, ", cheer") << "\n";
	auto increment{ [count = 0]() mutable {return count++;} };
	for (size_t i = 0; i < 5; i++) {
		std::cout << "increment() : " << increment() << "\n";
	};
	int a{0};
	auto counter{ [&]() {a++;} };
	counter();
	counter();
	counter();
	std::cout << "counter a three times " << a << "\n";
	auto minus{ [=](int x) -> int {return  x - a;} };
	std::cout << "minus(10) :" << minus(10) << "\n";
}

template <typename C>
static auto consumer(C& container) {
	return [&](int value) {
		container.push_back(value);
	};
};

template <typename C>
static void print(C &container) {
	for (auto val : container) {
		std::cout << "val : " << val << " ";
	};
	std::cout << "\n";
};


#include<deque>
#include<vector>
#include<list>
#include<functional>
void lambda_func3() {
	std::deque<int> d;
	std::list<int> l;
	std::vector<int> v;

	std::vector<std::function<void(int)>> consumers{
		consumer(d), consumer(l), consumer(v)
	};

	for (int i = 0; i <= 10; i++) {
		for (auto& consumer : consumers) {
			consumer(i);
		}
	};
	
	print(d);
	print(l);
	print(v);
};

template<typename T, typename ... Ts>
auto contact(T t, Ts ...ts) {
	if constexpr (sizeof...(ts) > 0) {
		return [=](auto ...param) {
			return t(contact(ts...)(param...));
		};
	}
	else
	{
		return t;
	}
}

void lambda_func4() {
	auto twice{ [](int i) {return i * 2;} };
	auto thrie{ [](int i) {return i * 3;} };
	
	auto combined(contact(twice, thrie, std::plus<int>{}));
	std::cout << combined(2, 3) << "\n";

}

static bool begin_with_a(const std::string& str) {
	return str.find("a") == 0;
};

static bool end_with_b(const std::string& str) {
	return str.rfind("b") == str.length() - 1;
};

template<typename A, typename B, typename F>
auto combine(F bin_func, A a, B b) {
	return [=](auto param) {
		return bin_func(a(param), b(param));
	};
};

#include<algorithm>
void lambda_func5() {
	auto a_xxx_b_func(combine(std::logical_and<>{}, begin_with_a, end_with_b));
	std::vector < std::string > v{
		"av", "bv", "ab", "cheer", "ssss", "axxxb"
	};

	for (auto val : v) {
		if (a_xxx_b_func(val)) {
			std::cout << "string begin with a end with b " << val << "\n";
		}
	}
}

#include<initializer_list>
template <typename ... Ts>
static auto multicall(Ts ...functions)
{
	return [=](auto x) {
		(void)std::initializer_list<int>{
			((void)functions(x), 0)...
		};
	};
}

template <typename F, typename ... Ts>
static auto for_each(F f, Ts ...xs) {
	(void)std::initializer_list<int>{
		((void)f(xs), 0)...
	};
}

static auto base_print(char a, char b) {
	return [=](auto x) {
		std::cout << a << x << b << ",";
	};
}

void lambda_func6() {
	auto f(base_print('(', ')'));
	auto g(base_print('[', ']'));
	auto h(base_print('{', '}'));
	auto nl([](auto) { std::cout << '\n'; });
	auto call_fgh(multicall(f, g, h, nl));
	for_each(call_fgh, 1, 2, 3, 4, 5);
}

template <typename T>
auto map(T fn) {
	return [=](auto reduce_fn) {
		return [=](auto accum, auto input) {
			return reduce_fn(accum, fn(input));
		};
	};
};

template <typename T>
auto filter(T predicate) {
	return [=](auto reduce_fn) {
		return [=](auto accum, auto input) {
			if (predicate(input)) {
				return reduce_fn(accum, input);
			}
			else {
				return accum;
			};
		};
	};
};

#include<iterator>
#include<numeric>
void lambda_func7() {
	auto even([](int i) {
		return i % 2 == 0;
	});

	auto twice([](int i) {
		return 	i * 2;
	});

	auto copy_and_advance([](auto it, auto input){
		*it = input;
		return ++it;
	});
	
	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 0};
	auto func = filter(even)(map(twice)(copy_and_advance));
	
	std::accumulate(v.begin(), v.end(),
		std::ostream_iterator<int>{std::cout, ", "},
		func
	);
	
	std::cout << "\n";
};

static void test_print(int x, int y) {
	std::cout << "(" << x << ", " << y << ")\n";
};

constexpr auto call_cart(
	[=](auto f, auto x, auto ...rest) constexpr {
		(void)std::initializer_list<int>{
			(((x < rest) ? (void)f(x, rest) : (void)0), 0)...
		};
	}
);
constexpr auto cartesian([=](auto ...xs) constexpr {
	return [=](auto f) constexpr {
		(void)std::initializer_list<int>{
			((void)call_cart(f, xs, xs...), 0)...
		};
	};
	});

void lambda_func8() {
	constexpr auto print_cart(cartesian(1, 2, 3));
	
	print_cart(test_print);
};
int main() {
	lambda_func8();

	return 0;
}
