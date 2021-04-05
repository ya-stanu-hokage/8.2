#include <iostream>
#include <algorithm>
#include <thread>
#include <string>
#include <mutex>
#include <vector>
#include <cstring>
#include <future>

void FindSubsequence(const std::string& base, const std::string& find, std::size_t left, std::size_t right, std::vector <std::size_t>& vector, std::mutex& mtx)
{
	std::size_t F_size = std::size(find);
	for (auto i = left; i < right; i++)
	{
		bool marker = true;
		for (auto n = 0U; n < F_size; n++)
		{
			if (base[i + n] != find[n])
				marker = false;
		}
		if (marker)
		{
			std::lock_guard lg(mtx);
			vector.push_back(i);
		}
	}
}


void MultyFindSubsequence(const std::string& base, const std::string& find, std::size_t left, std::size_t right, std::vector <std::size_t>& vector, std::mutex& mtx)
{
	
	if (right - left < 4 * std::size(find))
		FindSubsequence(base, find, left, right, vector, mtx);
	else
	{
		auto mid = left + (right - left) / 2;
		std::future<void> future = std::async(MultyFindSubsequence, std::ref(base), std::ref(find), left, mid, std::ref(vector), std::ref(mtx));
		MultyFindSubsequence(base, find, mid, right, vector, mtx);
		future.get();
	}
}


int main()
{
	std::string base = "AAAAGGGGGTTTTCCCCAGTCCATGAGCTGACTGCGCATGCACTGAAAAAGGGGTTTTTCCCCCCCTTTTGCGGGAG";

	std::string find;
	std::cin >> find;

	std::mutex mtx;

	std::vector <std::size_t> vector;
	std::size_t left = 0;
	std::size_t right = std::size(base) - std::size(find) + 1;
	MultyFindSubsequence(base, find, left, right, vector, mtx);

	for (auto elements : vector)
	{
		for (auto n = 0U; n < std::size(find); n++)
		{
			std::cout << base[elements + n];
		}
		std::cout << std::endl;
	}

	return 0;
}
