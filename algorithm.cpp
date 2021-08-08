/*
	第5章 STL基础算法

	使用STL算法的好处：

	维护性：算法的名字已经说明它要做什么了。显式使用循环的方式与使用STL算法的方式没法对比。
	正确性：STL是由专家编写和审阅过的，并且经过了良好的测试，重新实现的复杂程度可能是你无法想象的。
	高效性：STL算法真的很高效，至少要比手写的循环要强许多。
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter5/chapter5-0-chinese.md
	5.1:容器间相互复制元素
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter5/chapter5-1-chinese.md
	5.2:容器元素排序
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter5/chapter5-2-chinese.md
	5.3:从容器中删除指定元素
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter5/chapter5-3-chinese.md
	5.4:改变容器内容
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter5/chapter5-4-chinese.md
	5.5:在有序和无序的vector中查找元素
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter5/chapter5-5-chinese.md
	5.6:将vector中的值控制在特定数值范围内——std::clamp
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter5/chapter5-6-chinese.md
	5.7:在字符串中定位模式并选择最佳实现——std::search
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter5/chapter5-7-chinese.md
	5.8:对大vector进行采样
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter5/chapter5-8-chinese.md
	5.9:生成输入序列的序列
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter5/chapter5-9-chinese.md

	我们在std::search中使用了4种查找方式，得到了相同的结果。这几种方式适用于哪种情况呢？
		std::default_searcher：其会重定向到std::search的实现。
		std::boyer_moore_searcher：使用Boyer-Moore查找算法。 （默认算法，性能最优秀）
		std::boyer_moore_horspool_searcher：使用Boyer-Moore-Horspool查找算法。
*/

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<iterator>
#include<algorithm>

template<typename T>
void algorithm_func1_print(T t) {
	for (auto val : t) {
		std::cout << "( " << val.first << ") " << val.second;
	}
	std::cout << "\n";
};

void algorithm_func1() {
	std::vector<std::pair<int, std::string>> v {
		{1, "one"}, {2, "two"}, {3, "three"},
		{4, "four"},{5, "five"}
	};
	std::map<int, std::string> m;
	std::copy_n(v.begin(), 3, std::inserter(m, m.begin()));
	algorithm_func1_print(m);
	m.clear();
	std::move(v.begin(), v.end(), std::inserter(m, m.begin()));
	algorithm_func1_print(m);
	algorithm_func1_print(v);
};

void algorithm_func2_print(std::vector<int> &v) {
	for (auto val : v) {
		std::cout << "val: " << val << " ";
	}
	std::cout << "\n";
}

#include<random>
void algorithm_func2() {
	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9,10 };

	std::random_device rd;
	std::mt19937 g{ rd() };

	std::cout << "is_sorted: " << std::is_sorted(v.begin(), v.end()) << "\n";
	// 吧g当作随机数据生成器来打乱v
	std::shuffle(v.begin(), v.end(), g);
	std::cout << "is_sorted: " << std::is_sorted(v.begin(), v.end()) << "\n";
	algorithm_func2_print(v);
	// 按照升序来排序
	std::sort(v.begin(), v.end());
	std::cout << "is_sorted: " << std::is_sorted(v.begin(), v.end()) << "\n";
	algorithm_func2_print(v);
	std::shuffle(v.begin(), v.end(), g);
	// 让使用partition将数值小于5的元素排到前面
	std::partition(v.begin(), v.end(), [](int i) {return i < 5;});
	algorithm_func2_print(v);
	std::shuffle(v.begin(), v.end(), g);
	auto middle(std::next(v.begin(), int(v.size() / 2)));
	//我们可以使用这个函数对容器的内容进行排序，
	//不过只是在某种程度上的排序。其会将vector中最小的N个数，放在容器的前半部分。其余的留在vector的后半部分，不进行排序
	std::partial_sort(v.begin(), middle, v.end());
	algorithm_func2_print(v);
	
	struct algorithm2_struct {
		int a;
		int b;
	};

	std::vector<algorithm2_struct> v1{
		{1, 2}, {2, 222}, {3, 23232}, {4, 1}, {5, 2}
	};

	std::sort(v1.begin(), v1.end(), [](const algorithm2_struct &s1, const algorithm2_struct &s2) {
		return s1.b < s2.b;
	});

	for (auto& [a, b] : v1) {
		std::cout << "{ " << a << ", " << b << " }";
	};
	std::cout << "\n";
};

void algorithm_func3() {
	std::vector<int> v{ 1, 2, 3, 4, 5, 6 };
	algorithm_func2_print(v);

	// 移除vector所有的2，并且这个迭代器指向新的末尾处
	const auto new_end(std::remove(v.begin(), v.end(), 2));
	v.erase(new_end, v.end());
	algorithm_func2_print(v);

	v = { 1, 2, 3, 4, 5, 6 };
	auto odd_number([](int i) {return i % 2 != 0;});
	// remove_if 移除所有奇数
	const auto new_end1(std::remove_if(v.begin(), v.end(), odd_number));
	v.erase(new_end1, v.end());
	algorithm_func2_print(v);
	////std::replace,我们使用这个函数将所有4替换成123
	std::replace(v.begin(), v.end(), 4, 123);
	algorithm_func2_print(v);

	v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	std::vector<int> v1;
	std::vector<int> v2;
	//
	auto even_number([](int i) { return i % 2 == 0;});
	// 当相应数值不满足谓词条件时，函数会从源容器中拷贝到另一个容器中
	std::remove_copy_if(v.begin(), v.end(), std::back_inserter(v1), odd_number);
	//// d::copy_if则是拷贝满足谓词条件的元素。
	std::copy_if(v.begin(), v.end(), std::back_inserter(v2), even_number);
	algorithm_func2_print(v1);
	algorithm_func2_print(v2);
};

#include<sstream>
void algorithm_func4() {
	std::vector<int> v{ 1, 2, 3, 4, 5, 6 };
	std::transform(v.begin(), v.end(),
		std::ostream_iterator<int>{std::cout, ", "},
		[](int i) {return i * i;}
		);
	std::cout << "\n";
	std::vector<std::string> v1;
	auto int_to_string(
		[](int i) {
			std::stringstream ss;
			ss << i << "^2 = " << i * i;
			return ss.str();
		}
	);
	
	std::transform(v.begin(), v.end(), std::back_inserter(v1), int_to_string);
	
	for (auto& s : v1) {
		std::cout << s << "";
	}
	std::cout << "\n";
};

struct city {
	std::string name;
	int population;

	bool operator==(const city& a) const {
		return name == a.name && population == a.population;
	}
};

void algorithm_func5_print(city &c){
	std::cout << "{" << c.name << ", " << c.population << "}";
}

template <typename C>
static auto opt_print(const C& container) {
	return [end_it(container.end())](const auto &item) {
		if (item != end_it) {
			//algorithm_func5_print(*item);
			std::cout << *item << "\n";
		}
		else {
			std::cout << "<end>\n";
		};
	};
};

void algorithm_func5() {
	std::vector<city> c{
		{"Beijing", 23232323},
		{"GuangZhou", 232323},
		{"XiaMen", 1111},
		{"TianJin", 2222333}
	};

	//auto print_city(opt_print(c));
	//// 查找城市的名字叫广州而且人口为232323
	//auto found_GZ(std::find(c.begin(), c.end(), city{ "GuangZhou", 232323 }));
	//print_city(found_GZ);

	//// 仅仅查找的名字是beijing的城市
	//auto found_BJ(std::find_if(c.begin(), c.end(), [](const city& c) {
	//	return	c.name == "Beijing";
	//	}));
	//print_city(found_BJ);

	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7,  8};
	std::sort(v.begin(), v.end());
	auto print_int(opt_print(v));
	// 二分查找vector是不是有7, 有返回true，没有返回false
	bool contains_7{ std::binary_search(v.begin(), v.end(), 7) };
	std::cout << "contains_7: " << contains_7 << "\n";
	// 第一个迭代器是指向第一个不小于给定值的元素 == lower_bound
	// 第二个迭代器是指向第一个大于给定值的元素	 == upper_bound
	auto [lower_it, upper_it](std::equal_range(v.begin(), v.end(), 7));
	print_int(lower_it);
	print_int(upper_it);

	print_int(std::lower_bound(v.begin(), v.end(), 7));
	print_int(std::upper_bound(v.begin(), v.end(), 7));

}

static auto norm(int min, int max, int new_max) {
	const double diff(max - min);
	return [=](int val) {
		return int((val - min) / diff * new_max);
	};
};

static auto clamp_val(int min, int max) {
	return [=](int val) -> int {
		// 输出val, min, max 的中间值

		return std::clamp(val, min, max);
	};
};

void algorithm_func6() {
	std::vector<int> v{ 0, 1000, 5, 250, 300, 800, 900, 321 };
	// std::minmax_element 返回的是数据pair [最小值（多个相同返回第一个最小），最大值(多个相同，最后一个最大)]
	const auto [min_it, max_it] = std::minmax_element(v.begin(), v.end());
	std::cout << "min_it" << *min_it << ",max_it" << *max_it << "\n";

	std::vector<int> v_norm;
	v_norm.reserve(v.size());
	std::transform(v.begin(), v.end(), std::back_inserter(v_norm), norm(*min_it, *max_it, 255));
	// 输出伤害
	std::copy(v_norm.begin(), v_norm.end(), std::ostream_iterator<int>{std::cout, ", "});
	std::cout << "\n";
	std::transform(v.begin(), v.end(), v_norm.begin(), clamp_val(0, 255));
	std::copy(v_norm.begin(), v_norm.end(), std::ostream_iterator<int>{std::cout, ", "});
	std::cout << "\n";
}

template <typename Itr>
static void algorithm_fun7_print(Itr it, size_t chars) {
	std::copy_n(it, chars, std::ostream_iterator<char>{std::cout});
	std::cout << "\n";
};

#include<functional>
void algorithm_fun7() {
	const std::string long_string{
		"Lorem ipsum dolor sit amet, consetetur"
		" sadipscing elitr, sed diam nonumy eirmod" };
	const std::string needle{ "elitr" };
	auto match(std::search(long_string.begin(), long_string.end(), needle.begin(), needle.end()));
	algorithm_fun7_print(match, 5);
	auto match1(std::search(long_string.begin(), long_string.end(), std::boyer_moore_searcher(needle.begin(), needle.end())));
	algorithm_fun7_print(match1, 5);
	auto match2(std::search(long_string.begin(), long_string.end(), std::boyer_moore_horspool_searcher(needle.begin(), needle.end())));
	algorithm_fun7_print(match2, 5);
	auto match3(std::search(long_string.begin(), long_string.end(), std::default_searcher(needle.begin(), needle.end())));
	algorithm_fun7_print(match3, 5);
};

#include<iomanip> // setw
void algorithm_fun8() {
	const size_t data_points{ 100000 };
	const size_t sample_points{ 100 };

	const int mean{ 10 };
	const size_t dev{ 3 };

	std::random_device rd;
	std::mt19937 gen{ rd() };
	std::normal_distribution<> d{ mean, dev };

	std::vector<int> v;
	v.reserve(data_points);
	
	// std::generate_n算法，其会将随机值，通过back_inserter迭代器插入vector中。生成函数对象包装成了d(gen)表达式，其能生成符合分布的随机值
	std::generate_n(std::back_inserter(v), data_points, [&] {return d(gen);});

	std::vector<int> samples;
	samples.reserve(sample_points);
	// std::sample算法与std::copy的原理类似，不过其需要两个额外的参数：采样数量和随机值生成对象。前者确定输入范围，后者去确定采样点：
	std::sample(v.begin(), v.end(), std::back_inserter(samples), sample_points, std::mt19937{ std::random_device{}() });

	std::map<int, size_t> hist;
	for (int i : samples) {
		++hist[i];
	};

	for (const auto& [value, count] : hist) {
		std::cout << std::setw(2) << value << " " << std::string(count, '*') << "\n";
	}
}

void algorithm_func9() {
	std::vector<std::string> v{ "a", "b", "c", "z"};
	do {
		std::copy(v.begin(), v.end(), std::ostream_iterator<std::string> {std::cout, ", "});
		std::cout << "\n";
	} while (std::next_permutation(v.begin(), v.end()));
}

int main() {
	algorithm_func9();
	return 0;
}
