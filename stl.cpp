/*
	1. 擦除/移除std::vector 元素

	std::remove函数对其他容器同样有效。当使用std::array时，其不支持erase操作，因为其内存空间固定，无法进行自动化处理。
	因为std::remove只是将要删除的元素移动到容器末尾，而不是将其真正删除，所以这个函数也可以用于不支持空间大小变化的数据类型。
	当然也有其他类似的方法，例如字符串中，可以用哨兵值\0来覆盖原始的end迭代所指向的值。

	2.以O(1)的时间复杂度删除未排序std::vector中的元素
	因为其他元素要填补删除元素所留下来的空隙，从而需要进行移动，所以从std::vector中删除元素的时间复杂度为O(n)
	移动其他元素也与此类似，当很多很大或很复杂的元素需要移动，那么就会花费很长的时间。当无法保证顺序时，我们需要对其进行优化，这就是本节的内容。

	不过试想我们存储了一些非常长的字符串在vector中，或存储了另一个vector或map——这种情况下，
	简单的赋值将对这些值进行拷贝，那么就会带来非常大的开销。这里使用std::move可将这部分开销优化掉：
	比如字符串，指向堆内存上存储的一个大字符串。我们无需拷贝它。只需要移动这个字符串即可，就是将目标指针指向这块地址即可。

	3.快速或安全的访问std::vector实例的方法
	std::vector提供了[]操作符和at函数，它们的作用几乎是一样的。at函数会检查给定的索引值是否越界，如果越界则返回一个异常。这对于很多情景都十分适用，不过因为检查越界要花费一些时间，所以at函数会让程序慢一些。
	当需要非常快的索引成员时，并能保证索引不越界，我们会使用[]快速访问vector实例。很多情况下，at函数在牺牲一点性能的基础上，有助于发现程序内在的bug。

	4. 保持对std::vector实例的排序
	array和vector不会对他们所承载的对象进行排序。有时我们去需要排序，但这不代表着我们总是要去切换数据结构，需要排序能够自动完成。
	在我们的例子有如有一个std::vector实例，将添加元素后的实例依旧保持排序，会是一项十分有用的功能。

	5. 向std::map实例中高效并有条件的插入元素
	我通常会使用insert或emplace函数对map插入新元素，如果插入不成功，那么就是第二种情况，就需要去修改现有的元素。insert和emplace都会创建一个新元素尝试插入到map实例中，
	不过在第二种情况下，这个新生成的元素会被扔掉。两种情况下，我们都会多余调用一次构造函数。

	6. 了解std::map::insert新的插入提示语义
	std::map中查找元素的时间复杂度为O(log(n))，与插入元素的时间复杂相同，因为要在对应位置上插入元素，那么就先要找到这个位置。通常，插入M个新元素的时间复杂度为O(M*log(n))。
	为了让插入更加高效，std::map插入函数接受一个迭代器参数hint。自C++11起，该参数为指向将插入新元素到其前的位置的迭代器。如果这个迭代器给定正确，那么插入的时间复杂度就为O(1)。
	总体来说就是你插入的位置越靠近你设置插入的位置,你的速度跟效率就越快。

	7. 高效的修改std::map元素的键值

	8. 过滤用户的重复输入，并以字母序将重复信息打印出——std::set

	9.实现简单的逆波兰表示法计算器——std::stack

	10.实现词频计数器——std::map
	
	11.实现写作风格助手用来查找文本中很长的句子——std::multimap (这个有点问题)

*/

#include<vector>
#include<iostream>
#include<algorithm>

void func1() {
	std::vector<int> v{ 101, 2, 3, 10, 4, 5, 30, 20, 10 };
	for (auto i : v) {
		std::cout << "before vector i: " << i << std::endl;
	}
	//const auto new_end = std::remove(std::begin(v), std::end(v), 20);
	/*const auto new_end(std::remove(std::begin(v), std::end(v), 20)); // 这里用的就是括号初始化规则
	v.erase(new_end, std::end(v));
	for (auto i : v) {
		std::cout << "after vector i: " << i << std::endl;
	}*/

	//Lambda
	const auto odd([](int i) {return i % 2 != 0;});
	// 我们可以清楚的看到，要从一个vector实例中移除一个元素，首先要进行删除，然后进行擦除，这样才算真正的移除。
	const auto new_end(std::remove_if(std::begin(v), std::end(v), odd));
	v.erase(new_end, std::end(v));
	for (auto i : v) {
		std::cout << "after vector remove_if i:" << i << std::endl;
	}
};

template<typename T>
void printVector(std::vector<T> v) {
	for (auto i : v) {
		std::cout << "vector i: " << i << std::endl;
	}
}

template <typename T>
void quick_remove_at(std::vector<T> &v, size_t idx) {
	if(idx <= v.size()) {
		std::cout << "quick_remove_at by idx: " << idx << std::endl;
		v[idx] = std::move(v.back());
		v.pop_back();
	}
};

// 函数特例化
template <typename T>
void quick_remove_at(std::vector<T> &v, typename std::vector<T>::iterator it) {
	if (it != v.end()) {
		std::cout << "quick_remove_at by it " << std::endl;

		*it = std::move(v.back());
		v.pop_back();
	}
};

void func2() {
	std::vector<int> v{ 123, 456, 789 };
	quick_remove_at(v, 2);
	printVector(v);
	quick_remove_at(v, std::find(std::begin(v), std::end(v), 123));
	printVector(v);
};

/*
	打印输出:
	v[vector_size + 100]0
	terminate called after throwing an instance of 'std::out_of_range'
	what():  vector::_M_range_check: __n (which is 100) >= this->size() (which is 100)
*/
void func3() {
	const size_t vector_size = 100;
	std::vector<int> v(vector_size, 123);
	std::cout << "v[vector_size + 100]" << v[vector_size + 100] << "\n";
	std::cout << "out of range v.at(vector_size + 100)" << v.at(vector_size) << "\n";
}

template<typename C, typename T>
void insert_sorted(C &c, const T &t) {
	const auto insert_pos(std::lower_bound(c.begin(), c.end(), t));
	c.insert(insert_pos, t);
};

void func4() {
	std::vector<std::string> v{ "aa", "zcw", "wqs", "qqz", "ww" };
	// is_sorted，如果v.beigin() 跟v.end() 范围少于两个值，必定返回true
	bool isSorted = std::is_sorted(v.begin(), v.end()); 
	printVector(v);
	std::cout << "func4 is vector sorted" << isSorted<< "\n";
	std::sort(v.begin(), v.end());
	isSorted = std::is_sorted(v.begin(), v.end());
	printVector(v);
	std::cout << "func4 is vector after std::sort" << isSorted << "\n";
	insert_sorted(v, "cc");
	std::cout << "insert Sorted after " << std::endl;
	printVector(v);
};

struct billionaire {
	std::string name;
	double dollers;
	std::string country;
};

#include<map>
#include<list>
void func5() {
	std::list<billionaire> billionaires {
		{"Bill Gates", 86.0, "USA"},
		{"Warren Buffet", 75.6, "USA"},
		{"Jeff Bezos", 72.8, "USA"},
		{"Amancio Ortega", 71.3, "Spain"},
		{"Mark Zuckerberg", 56.0, "USA"},
		{"Carlos Slim", 54.5, "Mexico"},
		{"Bernard Arnault", 41.5, "France"},
		{"Liliane Bettencourt", 39.5, "France"},
		{"Wang Jianlin", 31.3, "China" },
		{ "Li Ka-shing", 31.2, "Hong Kong" },
	};

	std::map<std::string, std::pair<billionaire, size_t>> m;
	for (auto& b : billionaires) {
		//std::pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);
		//其函数第一个参数k是插入的键，args表示这个键对应的值。如果我们成功的插入了元素，那么函数就会返回一个迭代器，其指向新节点在表中的位置，组对中布尔变量的值被置为true。当插入不成功，组对中的布尔变量值会置为false，并且迭代器指向与新元素冲突的位置。
		auto [it, success] = m.try_emplace(b.country, b, 1);
		if (!success) {
			it->second.second += 1;
		}
	}
	
	for (const auto& [country, val] : m) {
		const auto& [b, cnt] = val;
		std::cout << "country: " << country << "has :" << cnt << " billionaires" << std::endl;
	}
 }

void func6() {
	std::map<std::string, size_t> m{ {"b", 2}, {"c", 3}, {"d", 4} };
	auto insert_end(m.end());
	for (const auto& s : { "z", "y", "x", "w", "v" }) {
		insert_end = m.insert(insert_end, { s, 1 });
	}
	m.insert(m.end(), { "a", 1 });
	for (const auto& [key, value] : m) {
		std::cout << "key: " << key << " value: " << value << std::endl;
 	}
}

template <typename T, typename S>
void printMap(std::map<T, S> &m) {
	for (const auto& [index, val] : m) {
		std::cout << "map index: " << index << " val : " << val << std::endl;
	}
}

void func7() {
	std::map<int, std::string> ranks{
		{1, "maliao"}, {2, "Tom"}, {3, "jerry"},
		{4, "hello kitty"}, {5, "yi"}, {6, "cheer"}
	};
	printMap(ranks);
	// extract函数是C++17新加的特性。其可以从map中删除元素，并没有内存重分配的副作用
	auto a(ranks.extract(1));
	auto b(ranks.extract(6));
	std::cout << "after extract" << "\n";
	printMap(ranks);
	std::swap(a.key(), b.key());
	ranks.insert(std::move(a));
	ranks.insert(std::move(b));
	std::cout << "after swap" << "\n";
	printMap(ranks);
}

#include<string>
#include<set>
#include<iterator>
void func8() {
	std::string str("1 2 3 4 5 6 7");
	std::set<std::string> s{"1", "2", "3", "4", "1", "2", "333", "4", "23232", "5"};
	/*std::istream_iterator<std::string> it{ std::cin };
	std::istream_iterator<std::string> end;
	std::copy(it, end, std::inserter(s, s.end()));*/

	for(const auto & val: s){
		std::cout << val << ", ";
	} 
	std::cout << "\n";

}

#include<stack>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include <cmath>
template<typename IT>
double evaluate_rpn(IT it, IT end) {
	std::stack<double> val_stack;
	auto pop_stack([&]() {
		auto r(val_stack.top());
		val_stack.pop();
		return r;
	});

	std::map<std::string, double(*) (double, double)> ops{
		{"+", [](double a, double b) {return a + b;}},
		{"-", [](double a, double b) { return a - b; }},
		{"*", [](double a, double b) { return a * b; }},
		{"/", [](double a, double b) { return a / b; }},
		{"^", [](double a, double b) { return pow(a, b); }},
		{"%", [](double a, double b) { return fmod(a, b); }},
	};

	for (; it != end; ++it) {
		std::stringstream ss{ *it };
		if (double val; ss >> val) {
			val_stack.push(val);
		}
		else {
			const auto r{pop_stack()};
			const auto l{ pop_stack() };
			try {
				const auto& op(ops.at(*it)); // lamba函数
				const double result{ op(l, r) };
				val_stack.push(result);
			}
			catch (const std::out_of_range&) {
				throw std::invalid_argument(*it);
			}
		}
	}

	return val_stack.top();
}

#include<iterator>
void func9() {
 	std::stringstream s{ "1 3 4 + * 2 /" };
	std::cout << evaluate_rpn(std::istream_iterator<std::string>{s}, {}) << '\n';
}

std::string filter_punctuation(const std::string& str){
	const char* forbidden{".,:; "};
	const auto idx_start(str.find_first_not_of(forbidden));
	const auto idx_end(str.find_last_not_of(forbidden));
	return str.substr(idx_start, idx_end - idx_start + 1);
}

#include<iomanip>
void func10() {
	std::map<std::string, size_t> words;
	int max_word_len{0};
	std::vector<std::string> str_vector{" hello", " cheer", " happy", " children", "day",
		"day", "day", "up", "cheer", "your ", "light"};
	for (const auto& str : str_vector) {
		auto filter_str(filter_punctuation(str));
		max_word_len = std::max<int>(max_word_len, filter_str.length());
		++words[filter_str];
	}

	std::vector<std::pair<std::string, size_t>> sort_vectror;
	sort_vectror.reserve(words.size());
	//我们使用std::move函数将词-频对应关系填充整个vector。这样的好处是让单词不会重复，不过这样会将元素从map中完全删除。使用move方法，减少了很多不必要的拷贝。
	std::move(std::begin(words), std::end(words), std::back_inserter(sort_vectror));
	std::sort(std::begin(sort_vectror), std::end(sort_vectror),
		[](const auto& a, const auto& b) {
			return a.second > b.second;
		}
	);

	// setw: 设置用于输出操作的字段宽度。
	/*std::cout << "# " << std::setw(max_word_len) << "<WORD>" << " #<COUNT>\n";
	for (const auto& [word, count] : sort_vectror) {
		std::cout << std::setw(max_word_len + 2) << word << " #"
			<< count << '\n';
	}*/
}

//std::string fiflter_ws(const std::string& str) {
//	const char* ws{ " \r\n\t" };
//	const auto a(str.find_first_not_of(ws));
//	const auto b(str.find_last_not_of(ws));
//	if (a == std::string::npos) {
//		return {};
//	}
//
//	return str.substr(a, b);
//}
//
//std::multimap<size_t, std::string> gen_sentence_stats(const std::string& content) {
//	std::multimap<size_t, std::string> ret;
//	const auto end_it(std::end(content));
//	auto it1(std::begin(content));
//	auto it2(find(it1, end_it, '.'));
//
//	while (it1 != end_it && std::distance(it1, it2) > 0) {
//		std::string s{ fiflter_ws({it1, it2}) };
//
//		if (s.length() > 0) {
//			const auto words(std::count(std::begin(s), std::end(s), ' ') + 1);
//			ret.emplace(words, std::move(s));
//		}
//		it1 = std::next(it2, 1);
//		it2 = std::find(it1, end_it, ".");
//	}
//	return ret;
//}
//
//void func11() {
//	std::cin.unsetf(std::ios::skipws);
//	std::string content{ std::istream_iterator<char>{std::cin}, {} };
//	for (const auto& [word_count, sentence]
//		: gen_sentence_stats(content)) {
//		std::cout << word_count << " words: " << sentence << ".\n";
//	}
//}

#include<queue>
void func12() {
	using item_type = std::pair<int, std::string>;
	std::priority_queue<item_type> q;
	std::initializer_list<item_type> il{
		{1, "dishes"},
		{0, "watch tv"},
		{2, "do homework"},
		{0, "read comics"},
	};

	for (auto& val : il) {
		q.push(val);
	}

	while (!q.empty()) {
		std::cout << q.top().first << ": " << q.top().second << "\n";
		q.pop();
	}

}

int main() {
	func12();
}
