/*
	第3章 迭代器:
	迭代器是C++中非常重要的概念。STL旨在打造一组灵活和通用的工具集，迭代器是工具集中重要的一环。不过，有时候迭代器使用起来比较繁琐，所以很多编程人员还是喜欢用C的指针来完成相应的功能。一半的编程人员基本上会放弃使用STL中的迭代器。本章介绍了迭代器，并展示如何让它们很快的工作起来。快速地介绍是不能完全覆盖迭代器强大的功能，但是这种小例子能让你增加对迭代器的好感度。
	其实迭代器相当于中间件，让STL容器通过对应的迭代器使用算法。

	3.1 建立可迭代区域
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter3/chapter3-1-chinese.md
	3.2 让自己的迭代器与STL的迭代器兼容
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter3/chapter3-2-chinese.md
	3.3: 使用迭代适配器填充通用数据结构
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter3/chapter3-3-chinese.md
	3.4 使用迭代器实现算法(斐波那契数列)
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter3/chapter3-4-chinese.md
	3.5 使用反向迭代适配器进行迭代
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter3/chapter3-5-chinese.md
	3.6 使用哨兵终止迭代
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter3/chapter3-6-chinese.md
	3.7 使用检查过的迭代器自动化检查迭代器代码（有点高级）
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter3/chapter3-7-chinese.md
	3.8 构建zip迭代适配器
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter3/chapter3-8-chinese.md
*/


class num_iterator {
	int i;
public:
	// C++中的explicit关键字只能用于修饰只有一个参数的类构造函数
	//  explicit关键字的作用就是防止类构造函数的隐式自动转换.
	explicit num_iterator(int position = 0) : i(position) {};
	int operator*() const {
		return i;
	}
	
	bool operator!=(const num_iterator &n) const {
		return n.i != i;
	}
	
	num_iterator& operator++() {
		++i; 
		return *this;
	}
	
	bool operator==(const num_iterator &other) const {
		return other.i == i;
	}
};

class num_range {
	int a;
	int b;
public:
	num_range(int begin, int end) : a(begin), b(end) {};
	num_iterator begin()  const{
		return num_iterator{ a };
	}
	num_iterator end() const {
		return num_iterator{ b };
	}
};

#include<iostream>
void iterator_func1() {
	for (int i : num_range(100, 150)) {
		std::cout << "i : " << i << "\n";
	}
	std::cout << "\n";
}


#include<algorithm>
namespace std {
	template<>
	struct iterator_traits<num_iterator> {
		using iterator_category = std::forward_iterator_tag;
		using value_type = int;
	};
}
void iterator_func2() {
	num_range r{ 12, 111 };

	auto pair = std::minmax_element(r.begin(), r.end()); 
	std::cout << "min: " << *pair.first << " max: " << *pair.second << "\n";	
}

#include<deque>
#include<iterator>
#include<sstream>
void iterator_func3() {
	std::istream_iterator<int> it_cin{ std::cin };
	std::istream_iterator<int> it_end;
	
	std::deque<int> v;
	std::copy(it_cin, it_end, std::back_inserter(v));

	std::istringstream sstr{ "456 123 879" };
	auto deque_middle(std::next(std::begin(v), static_cast<int>(v.size()) / 2));
	std::copy(std::istream_iterator<int>{sstr}, {}, std::inserter(v, deque_middle));

	std::initializer_list<int> il2{ -1, -2 , -3 };
	std::copy(std::begin(il2), std::end(il2), std::front_inserter(v));

	std::copy(std::begin(v), std::end(v), std::ostream_iterator<int>{ std::cout, "," });
	std::cout << "\n";
}

class fibit {
	size_t i{ 0 };
	size_t a{ 0 };
	size_t b{ 1 };
public:
	fibit() = default;
	explicit fibit(size_t _i) : i(_i) {};
	size_t operator*() const { return b; };
	fibit& operator++() {
		const size_t old_b{ b };
		b = a + b;
		a = old_b;
		++i;
		return *this;
	}
	
	bool operator!=(const fibit& other) {
		return other.i != i;
	}
};

class fibit_range {
	size_t end_n;
public:
	fibit_range(size_t end_n_) : end_n(end_n_) {};
	fibit begin() const { return fibit{}; };
	fibit end() const { return fibit{ end_n }; };
};

void iterator_func4() {
	for (size_t val : fibit_range(10)) {
		std::cout << val << " ";
	}
	std::cout << "\n";
}

#include<list>
void iterator_func5() {
	std::list<int> z{1, 2, 3, 4, 5};
	std::copy(z.rbegin(), z.rend(), std::ostream_iterator<int>{ std::cout, " ,"});
	std::cout << "\n";
	std::copy(std::make_reverse_iterator(std::end(z)),
		std::make_reverse_iterator(std::begin(z)),
		std::ostream_iterator<int>{std::cout, " ,"}
	);
}

class cstring_iterator_sentinel {};
class cstring_iterator {
	const char* s{ nullptr };
public:
	explicit cstring_iterator(const char* str) : s(str) {};
	char operator*() const { return *s; };
	cstring_iterator& operator++() {
		s++;
		return *this;
	}
	bool operator!=(const cstring_iterator_sentinel) const {
		return s != nullptr && *s != '\0';
	}
};

class cstring_range {
	const char* s{ nullptr };
public:
	cstring_range(char* str) :s{ str } {};
	cstring_iterator begin() const { return cstring_iterator{ s }; };
	cstring_iterator_sentinel end() const { return {}; };
};

#include<string>
#include<cstring>
void iterator_func6() {
	char* str = (char *)"hello cheer";
	for (char c : cstring_range(str)) {
		std::cout << c  << ", ";
	}
	std::cout << "\n";
}

#include<vector>
void iterator_func7() {
	std::vector<int> v{ 1, 2, 3 };
	// 要求容器减少它的容量适应到他的长度
	v.shrink_to_fit(); // Requests the container to reduce its capacity to fit its size
	const auto it(std::begin(v));
	std::cout << "*it: " << *it << "\n";
	// 我们向vector中增加一个新数。这样vector的长度就不够再放下另外一个数，这里vector会自动增加其长度。通过分配一个新的更大的内存块来实现长度的增加，会将所有现存的项移到新的块，然后删除旧的内存块。
	v.push_back(123); 
	std::cout << "after push_back *it: " << *it << "\n";
}

int main() {
	iterator_func7();
	return 0;
}
