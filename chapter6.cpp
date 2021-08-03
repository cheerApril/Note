/*
	chapter6: STL算法的高级使用方式

	STL算法旨在保持简单和通用。这样大多数代码就可以使用STL算法，从而让代码看起来简单明了。
	一个经验丰富的C++开发者对于STL算法非常了解，其会在代码中尽可能使用STL算法，这会让其他人更容易明白这段代码存在的原因，这样能帮助开发者和阅读者产生最大程度的共鸣。
	一个开发者的大脑会很容易的解析这些普及度很高的算法，这要比解析一段复杂的循环简单的多。
	虽然实现的主要方式一样，但是细节方面还有不同。

	我们使用STL数据结构结构能够很好的避免指针、裸数组和粗犷的结构体。那么接下来我们将升级对STL算法的理解，以便使用通用的STL算法来替代复杂的循环控制符合代码块。因为代码变得短小，在提高可读性的同时，又增加了通用性。这就能避免循环，仅调用std命名空间的算法即可，不过有时也会造成很糟糕的代码。我们不会去衡量代码是否糟糕，只会讨论可能性。

	6.1:使用STL算法实现单词查找树类 (语法树用来做搜索处理，屏蔽词处理)
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter6/chapter6-1-chinese.md
	
	6.2 使用树实现搜索输入建议生成器
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter6/chapter6-2-chinese.md

	6.3 使用STL数值算法实现傅里叶变换(难产) 
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter6/chapter6-3-chinese.md

	6.4 计算两个vector的误差和
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter6/chapter6-4-chinese.md

	6.5 使用ASCII字符曼德尔布罗特集合(好装的算法)
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter6/chapter6-5-chinese.md

	6.6 实现分割算法
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter6/chapter6-6-chinese.md
 
	6.7 将标准算法进行组合
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter6/chapter6-7-chinese.md

	6.8 删除词组间连续的空格
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter6/chapter6-8-chinese.md

	6.9 压缩和解压缩字符串
	https://github.com/xiaoweiChen/CPP-17-STL-cookbook/blob/master/content/chapter6/chapter6-9-chinese.md

 */

#include<map>
#include<vector>
#include<string>
#include<iterator>
#include<iostream>
#include<optional>
template <typename T>
class trie {
	std::map<T, trie> tries;
public:
	template<typename It>
	void insert(It it, It end_it) {
		if (it == end_it) { return; }
		tries[*it].insert(std::next(it), end_it);
	}

	template<typename C>
	void insert(const C& container) {
		insert(std::begin(container), std::end(container));
	};

	void insert(const std::initializer_list<T>& li) {
		insert(std::begin(li), std::end(li));
	}

	void print(std::vector<T>& v) const {
		if (tries.empty()) {
			std::copy(v.begin(), v.end(), std::ostream_iterator<T>{std::cout, " "});
			std::cout << "\n";
		};
		
		for (const auto& p : tries) {
			v.push_back(p.first);
			p.second.print(v);
			v.pop_back();
		}
	};

	void print() const {
		std::vector<T> v;
		print(v);
	}

	template <typename It>
	std::optional<std::reference_wrapper<const trie>>
		subtrie(It it, It end_it) const {
		if (it == end_it) { return std::ref(*this); }
		auto found(tries.find(*it));
		if (found == tries.end()) { return {}; }

		return found->second.subtrie(std::next(it), end_it);
	}

	template <typename C>
	auto subtrie(const C& c) {
		return subtrie(std::begin(c), std::end(c));
	}
};

void chapter6_func1() {
	trie<std::string> t;
	t.insert({"hi", "how", "are", "you"});
	t.insert({ "hi", "i", "am", "great", "thanks"});
	t.insert({ "what", "are", "you", "doing" });
	t.insert({ "I", "am", "watching", "a", "movie" });

	std::cout << "recorded sentnces: \n";
	t.print();

	std::cout << "\npossible suggestions after \"hi\":\n";

	if (auto st(t.subtrie(std::initializer_list<std::string>{"hi"}));
		st) {
		st->get().print();
	}
}

#include<sstream>
#include<fstream>
static void prompt() {
	std::cout << "Next input please: \n >";
}

#include<stdio.h>
void chapter6_func2() {
	trie<std::string> t;
	std::fstream infile{ "db.txt" };

	for (std::string line; std::getline(infile, line);) {
		std::istringstream iss{ line };
		t.insert(std::istream_iterator<std::string>{iss}, {});
	}
	
	prompt();
	for (std::string line; std::getline(std::cin, line);) {
		std::istringstream iss{ line };
		if (auto st(t.subtrie(std::istream_iterator<std::string>{iss}, {}));
			st) {
			std::cout << "Suggestions:\n";
			st->get().print();
		}
		else {
			std::cout << "No suggestions found.\n";
		}
		std::cout << "----------------\n";
		prompt();
	};
}

#include<algorithm>
#include<numeric>
const double M_PI = 3.14;
void chapter6_func4() {
	const size_t sig_len{ 100 };
	std::vector<double> as(sig_len);
	std::vector<int> ds(sig_len);
	
	auto sin_gen([n{ 0 }]() mutable{
		return 5.0 * sin(n++ * 2.0 * M_PI / 100);
	});
	
	std::generate(as.begin(), as.end(), sin_gen);
	std::copy(as.begin(), as.end(), std::begin(ds));
	std::copy(as.begin(), as.end(), std::ostream_iterator<double>{std::cout, " "});
	std::cout << "as print end \n";
	std::copy(ds.begin(), ds.end(), std::ostream_iterator<double>{std::cout, " "});
	std::cout << "ds print \n";
	
	std::cout << std::inner_product(as.begin(), as.end(), ds.begin(), 0.0, std::plus<double>{},
		[](double a, double b) {
			return pow(a - b, 2);
		}) << "\n";

};

#include<complex>
using cmplx = std::complex<double>;
static auto scaler(int min_from, int max_from, double min_to, double max_to) {
	const int w_from{ max_from - min_from };
	const double w_to{ max_to - min_to };
	const int mid_from{ (max_from - min_from) / 2 + min_from };
	const double mid_to{ (max_to - min_to) / 2.0 + min_to };

	return [=](int from) {
		return double(from - mid_from) / w_from * w_to + mid_to;
	};
}

template <typename A, typename B>
static auto scaled_cmplx(A scaler_x, B scaler_y)
{
	return [=](int x, int y) {
		return cmplx{ scaler_x(x), scaler_y(y) };
	};
}

static auto mandelbrot_iterations(cmplx c)
{
	cmplx z{};
	size_t iterations{ 0 };
	const size_t max_iterations{ 1000 };
	while (abs(z) < 2 && iterations < max_iterations) {
		++iterations;
		z = pow(z, 2) + c;
	}
	return iterations;
}

void chapter6_func5() {
	const size_t w{ 100 };
	const size_t h{ 40 };
	auto scale(scaled_cmplx(
		scaler(0, w, -2.0, 1.0),
		scaler(0, h, -1.0, 1.0)
	)); 
	auto i_to_xy([=](int i) { return scale(i % w, i / w); });
	auto to_iteration_count([=](int i) {
		return mandelbrot_iterations(i_to_xy(i));
	});

	std::vector<int> v(w * h);
	std::iota(v.begin(), v.end(), 0);
	std::transform(v.begin(), v.end(), v.begin(), to_iteration_count);
	
	auto binfunc([w, n{ 0 }](auto output_it, int x) mutable {
		*++output_it = (x > 50 ? '*' : ' ');
		if (++n % w == 0) { ++output_it = '\n'; }
		return output_it;
	});
	
	std::accumulate(v.begin(), v.end(), std::ostream_iterator<char>{std::cout}, binfunc);
};

template <typename InIt, typename OutIt, typename T, typename F>
InIt split(InIt it, InIt end_it, OutIt out_it, T split_val, F bin_func) {
	while (it != end_it) {
		auto slice_end = std::find(it, end_it, split_val);
		*out_it++ = bin_func(it, slice_end);
		if (slice_end == end_it) { return end_it; }
		it = std::next(slice_end);
	}
	return it;
}

#include<list>
void chapter6_func6() {
	const std::string s{"hello-cheer-a-b-c-d"};
	auto bind_func([](auto it, auto end_it) {
		return std::string(it, end_it);
	});
	std::list<std::string> l;
	split(std::begin(s), std::end(s), std::back_inserter(l), '-', bind_func);
	std::copy(std::begin(l), std::end(l), std::ostream_iterator<std::string>{std::cout, "\n"});
}

#include<algorithm>
#include<functional>
template<typename It, typename F>
std::pair<It, It> gather(It first, It end, It middle, F predicate) {
	// std::stable_partition 会把range(first, middle] Match predicate的函数移动到前面
	return { std::stable_partition(first, middle, std::not_fn(predicate)),
		std::stable_partition(middle, end, predicate)};
}

//template<typename It, typename F>
//void gather_sort(It first, It last, It gather_pos, F comp_func) {
//	auto inv_comp_func([&](const auto &...ps) {
//		return !comp_func(ps...);
//		});
//
//	std::stable_partition(first, gather_pos, inv_comp_func);
//	std::stable_partition(gather_pos, last, comp_func);
//}

void chapter6_func7() {
	auto is_a([](char c) {return c == 'a';});
	std::string a{ "a_a_a_a_a_a_a_a_a_a_a" };
	auto middle(std::begin(a) + a.size() / 2);
	
	gather(std::begin(a), std::end(a), middle, is_a);
	std::cout << a << '\n';

	gather(std::begin(a), std::end(a), std::begin(a), is_a);
	std::cout << a << '\n';

	gather(std::begin(a), std::end(a), std::end(a), is_a);
	std::cout << a << '\n';

	// This will NOT work as naively expected
	gather(std::begin(a), std::end(a), middle, is_a);
	std::cout << a << '\n';

	/*std::string b{ "_9_2_4_7_3_8_1_6_5_0_" };
	gather_sort(std::begin(b), std::end(b), std::begin(b) + b.size() / 2,
		std::less<char>{});
	std::cout << b << '\n';*/
};

template <typename It>
It remove_mulit_whitespace(It it, It end_it)
{
	// std::unique 移除相邻相同的值，但是总的物理空间不会改变
	return std::unique(it, end_it, [](const auto& a, const auto& b) {
		return isspace(a) && isspace(b);
		});
};

void chapter6_func8() {
	std::string s{ "hello cheer \t welcome \t  home \t" };
	std::cout << s << '\n';
	s.erase(remove_mulit_whitespace(begin(s), end(s)), end(s));
	std::cout << s << '\n';

	std::vector<int> v{ 1, 1, 2, 1, 3, 2, 5, 4, 5, 6, 6 };
	auto it = std::unique(v.begin(), v.end());
	v.erase(it, v.end());
	for (auto val : v) {
		std::cout << val << " ";
	}
	std::cout << '\n';

	std::sort(v.begin(), v.end());
	v.erase(std::unique(v.begin(), v.end()), v.end());
	for (auto val : v) {
		std::cout << val << " ";
	}
}

template <typename It>
std::tuple<It, char, size_t> occurrences(It it, It end_it) {
	if (it == end_it) { return { it,'?', 0 }; };

	const char c{ *it };
	const auto diff(std::find_if(it, end_it, [c](char x) {return c != x;}));
	return { diff, c, std::distance(it, diff) };
}

std::string compress(const std::string& s)
{
	const auto end_it(s.end());
	std::stringstream r;

	for (auto it(s.begin()); it != end_it;) {
		const auto [next_diff, c, n] = occurrences(it, end_it);
		r << c << n;
		it = next_diff;
	}

	return r.str();
}

std::string decompress(const std::string& s) {
	std::stringstream ss{ s };
	std::stringstream r;
	char c;
	size_t n;
	while (ss >> c >> n) { r << std::string(n, c); }
	return r.str();
}

void chaoter6_func9() {
	std::string s{ "abbbbbbbbbccccccccccc" };
	std::cout << compress(s) << '\n';
	std::cout << decompress(compress(s)) << '\n';
}

int main() {
	chaoter6_func9();
	return 0;
}
