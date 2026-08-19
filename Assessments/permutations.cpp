#include <vector>
#include <numeric>
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include <random>
#include <sstream>
#include <cmath>
#include "permutations.h"
//using namespace std;

//size_t operator()(vector<uint32_t> const& vec) const {
//	size_t seed = vec.size();
//	for (auto x : vec) {
//		x = ((x >> 16) ^ x) * 0x45d9f3b;
//		x = ((x >> 16) ^ x) * 0x45d9f3b;
//		x = (x >> 16) ^ x;
//		seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//	}
//}
//
//template <>
//struct hash<pair<int, int>>
//{
//	size_t operator()(const vector<int>& k) const
//	{
//		hash<string> hasher;
//		string as_sequence;
//		for (auto num : k) {
//			as_sequence = as_sequence + to_string(num);
//		}
//		size_t hashed_first = hasher(as_sequence);
//		hash_combine(hashed_first, as_sequence);
//		return hashed_first;
//	}
//};

using std::string;
using std::vector;
using std::ostringstream;
using std::random_device;
using std::mt19937;
using std::numeric_limits;
using std::unordered_set;
using std::unordered_map;
using std::cout;
using std::endl;

static string stringify_vector_ints(vector<int> list) {
	ostringstream oss;
	for (const auto& num : list) {
		oss << num;
	}
	auto set_as_string = oss.str();
	return set_as_string;
}

static vector<int> range(const int& end, const int& start = 0, const int& step_size = 1) {
	int slots_needed = (end - start + step_size - 1) / step_size;
	auto list = vector<int>(slots_needed);
	iota(list.begin(), list.end(), start);
	return list;
}
using std::min;
void perm_entry() {
	auto mapper_before_index_adjustment = vector<int>{ 2, 3, 1 };
	auto from_to_next = accumulate(mapper_before_index_adjustment.begin(), mapper_before_index_adjustment.end(), vector<int>{},
		[](auto acc, auto num) {
			acc.push_back(num - 1);
			return acc;
		});

	vector<int> permuted_set = from_to_next;
	sort(permuted_set.begin(), permuted_set.end());
	auto list_of_permutations = vector<vector<int>>{ permuted_set };
	while (next_permutation(permuted_set.begin(), permuted_set.end())) {
		list_of_permutations.push_back(permuted_set);
	}

	auto min_cycles = 1000000000;
	for (auto permutation : list_of_permutations) {
		auto permutation_id = stringify_vector_ints(permutation);
		auto visited_permutations = unordered_set<string>{ permutation_id };
		auto found_original = false;
		auto found_cycle = false;
		auto next_mutation = permutation;
		auto current_cycles = 0;
		while (!found_original && !found_cycle) {
			auto next_slot_to_overridden_value = unordered_map<int, int>{};
			for (auto slot_to_take_from : range(from_to_next.size())) {
				auto next_slot_location = from_to_next[slot_to_take_from];
				auto value_in_next_slot_that_is_getting_overriden = next_mutation[next_slot_location];
				next_slot_to_overridden_value[next_slot_location] = value_in_next_slot_that_is_getting_overriden;
				auto the_value_at_this_slot_was_overwritten =
					next_slot_to_overridden_value.find(slot_to_take_from) != next_slot_to_overridden_value.end();
				if (the_value_at_this_slot_was_overwritten) {
					auto value_that_was_overwritten = next_slot_to_overridden_value[slot_to_take_from];
					next_mutation[next_slot_location] = value_that_was_overwritten;
					next_slot_to_overridden_value.erase(slot_to_take_from);
				}
				else {
					next_mutation[next_slot_location] = next_mutation[slot_to_take_from];
				}
			}
			auto next_mutation_id = stringify_vector_ints(next_mutation);
			found_cycle = visited_permutations.find(next_mutation_id) != visited_permutations.end();
			visited_permutations.insert(next_mutation_id);
			found_original = next_mutation_id == permutation_id;
			current_cycles++;
		}
		if (found_original) {
			min_cycles = min(min_cycles, current_cycles);
		}
	}

	//auto original_vector = from_to_next;
//auto original_set = stringify_vector_ints(original_vector);
//auto counter = 1;
//auto tried_sets = unordered_set<string>{ stringify_vector_ints(original_vector) };
//int max_possible_size = tgamma(from_to_next.size() + 1);
//auto found_input = false;
//auto exhausted_permutations = false;
//auto found_cycle = true;
//auto found_sets = unordered_set<string>{ };


	//random_device rd;
//mt19937 g(rd());
//shuffle(from_to_next.begin(), from_to_next.end(), g);
//auto min_cycles = numeric_limits<int>::max();
//auto permuted_set = original_vector;

// make a list of permutations P of the initial I random list
// cycle through P until I or visited V of P
// keep track of minimum


	//while (!exhausted_permutations) {
	//	found_input = true;
	//	if (found_cycle) {
	//		next_permutation(permuted_set.begin(), permuted_set.end());
	//		tried_sets.insert(stringify_vector_ints(permuted_set));
	//		found_cycle = false;
	//	}


	//	exhausted_permutations = tried_sets.size() >= max_possible_size;
	//	auto set_as_string = stringify_vector_ints(permuted_set);
	//	found_input = set_as_string == original_set;
	//	found_cycle = found_sets.find(set_as_string) != found_sets.end();
	//	auto currently_cycling = !found_input && !exhausted_permutations && !found_cycle;
	//	counter++;
	//	if (currently_cycling) {
	//		found_sets.insert(set_as_string);
	//	}
	//	else if (found_input) {
	//		min_cycles = min(min_cycles, counter + 1);
	//		counter = 0;
	//	}
	//	else if (found_cycle) {
	//		for (auto set : found_sets) {
	//			tried_sets.insert(set);
	//		}
	//		found_sets = unordered_set<string>{};
	//		counter = 0;
	//	}
	//	else {
	//		min_cycles = min(min_cycles, counter + 1);
	//	}
	//	
	//}
	cout << min_cycles << endl;
}