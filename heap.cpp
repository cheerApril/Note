/*
	番外篇: heap 来源于:https://www.geeksforgeeks.org/heap-using-stl-c/
	Heap in C++ STL | make_heap(), push_heap(), pop_heap(), sort_heap(), is_heap, is_heap_until()
	1. make_heap() :- This function is used to convert a range in a container to a heap.
	2. front() :- This function displays the first element of heap which is the maximum number.
	3. push_heap() :- This function is used to insert elements into heap. The size of the heap is increased by 1. New element is placed appropriately in the heap.
	4. pop_heap() :- This function is used to delete the maximum element of the heap. The size of heap is decreased by 1. The heap elements are reorganised accordingly after this operation.
	5. sort_heap() :- This function is used to sort the heap. After this operation, the container is no longer a heap.
	6. is_heap() :- This function is used to check whether the container is heap or not. Generally, in most implementations, the reverse sorted container is considered as heap. Returns true if container is heap else returns false.
	6. is_heap_until() :- This function returns the iterator to the position till the container is the heap. Generally, in most implementations, the reverse sorted container is considered as heap.
*/

#include<vector>
#include<iostream>
#include<algorithm> //heap 
void heap_func1() {
	std::vector<int> v = {20, 30, 40, 25, 15};
	std::make_heap(v.begin(), v.end());
	std::cout << "the maximunm element of heap is " << v.front() << '\n';
}

void heap_func2() {
	std::vector<int> v = { 20, 30, 40, 25, 15 };
	std::make_heap(v.begin(), v.end());
	std::cout << "The maximunm element of heap is " << v.front() << '\n';
	// 这里插入是vector
	v.push_back(100);
	// 这个是根据heap来重新排序
	std::push_heap(v.begin(), v.end());
	std::cout << "The maximunm element of heap is " << v.front() << '\n';
	// 同样remove最大值，再排序
	std::pop_heap(v.begin(), v.end());
	std::cout << "The maximunm element of heap is " << v.front() << '\n';
}

void heap_func3() {
	std::vector<int> v = { 20, 30, 40, 25, 15 };
	std::make_heap(v.begin(), v.end());
	for (int i : v) {
		std::cout << i << " ";
	}
	std::cout << "\n";
	// 排序好就不再是heap了
	std::sort_heap(v.begin(), v.end());
	for (int i : v) {
		std::cout << i << " ";
	}
	std::cout << "\n";
}

void heap_func4() {
	std::vector<int> v{ 20, 30, 40, 25, 15 };
	std::is_heap(v.begin(), v.end()) ? 
		std::cout << "The container is heap": 
		std::cout << "The container is not heap";
	std::cout << '\n';
	
	auto it = std::is_heap_until(v.begin(), v.end());
	std::cout << "The heap elements in container are : ";
	for (auto it1 = v.begin(); it1 != it; it1++)
		std::cout << *it1 << " ";
}

int main() {
	heap_func4();
	return 0;
}
