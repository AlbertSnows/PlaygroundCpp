#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <string>
#include <ranges>
#include <span>
#include <numeric>
#include <regex>
#include "parser.h"
//using namespace std::views;
using std::string;
using std::unordered_map;
using std::vector;
using std::ranges::to;
using std::span;
using std::pair;
using std::accumulate;
using std::cout;
using std::endl;
using std::generate; 
using std::views::transform;
using std::views::split;
using std::views::filter;
using std::regex;
using std::sregex_token_iterator;
using std::sregex_iterator;
using std::smatch;
using std::regex_search;
class Element {
public:
	const string name;
	const unordered_map<string, string> properties;
	vector<Element> children;
	Element() {} // need this because maps will call the default constructor for a class if it doesn't exist...?
	Element(string name, unordered_map<string, string> properties)
		: name(name), properties(properties) {
		// this->name = name;
		// this->properties = properties;
	}

	Element addChildren(Element child) {
		this->children.push_back(child);
		return *this;
	}
};

vector<int> range(const int& end, const int& start = 0, const int& step_size = 1) {
	int slots_needed = (end - start + step_size - 1) / step_size;
	auto list = vector<int>(slots_needed);
	iota(list.begin(), list.end(), start);
	return list;
}

vector<string> parse_on_regex(string unparsed, regex pattern) {
	// Create an iterator over the matches in the input string
	sregex_iterator iter(unparsed.begin(), unparsed.end(), pattern);
	sregex_iterator end;
	// Iterate over the matches and print key-value pairs
	auto attribute_pairs = vector<string>();
	while (iter != end) {
		std::smatch match = *iter;
		attribute_pairs.push_back(match.str());
		++iter;
	}
	return attribute_pairs;
}

unordered_map<string, Element> parseHRMLOpeners(vector<string> unparsedHRMLOpeningTags) {
	auto unparsed_tag_data = unparsedHRMLOpeningTags
		| transform([](const string& str) { 
		string string_without_end_tags = str.substr(1, str.length() - 2);
		return string_without_end_tags; 
		})
		| to<vector<string>>();
	auto list_of_tags_with_unparsed_attributes = unparsed_tag_data
		| transform([](string str) {
			auto tag_position= str.find(' ');
			string tagname = str.substr(0, tag_position);
			string unparsed_attributes = str.substr(tag_position + 1);
			unparsed_attributes.erase(remove_if(unparsed_attributes.begin(), unparsed_attributes.end(), ::isspace), unparsed_attributes.end());
			auto break_on_second_quote = regex("[^\"]*\"[^\"]*\"");
			auto unparsed_attributes_list = parse_on_regex(unparsed_attributes, break_on_second_quote);
			auto attribute_pair_list = vector<pair<string, string>>{};
			for (string unparsed_attribute : unparsed_attributes_list) {
				auto attribute_split_position = unparsed_attribute.find("=");
				auto tagname = unparsed_attribute.substr(0, attribute_split_position);
				auto attribute_value = unparsed_attribute.substr(attribute_split_position + 1);
				attribute_pair_list.push_back(pair(tagname, attribute_value));
			}
			return pair(tagname, attribute_pair_list);
		})
		| to <vector<pair<string, vector<pair<string, string>>>>>();

	vector<Element> elements = accumulate(list_of_tags_with_unparsed_attributes.begin(), list_of_tags_with_unparsed_attributes.end(), vector<Element>{},
		[](vector<Element> elements, const pair<string, vector<pair<string, string>>>& tags_with_attribute_pairs_list) {
			auto tagname = tags_with_attribute_pairs_list.first;
			auto attribute_pair_list = tags_with_attribute_pairs_list.second;
			auto attribute_map = unordered_map<string, string>(attribute_pair_list.begin(), attribute_pair_list.end());
			auto element = Element(tagname, attribute_map);
			elements.push_back(element);
			return elements;
		});
	for (auto index : range(elements.size())) {
		auto current_element = elements[index];
		auto next_element = elements[index + 1];
		current_element.addChildren(next_element);
	}
	auto nametoelement = accumulate(elements.begin(), elements.end(), unordered_map<string, Element>{}, [](auto acc, auto element) {
		acc.emplace(element.name, element);
		return acc;
		});
	return unordered_map<string, Element>{};
};

vector<string> split_by_regex(const string& s, const regex& pattern) {
	sregex_token_iterator iter(s.begin(), s.end(), pattern, -1);
	sregex_token_iterator end;
	vector<string> elems;
	while (iter != end) {
		if (iter->length()) {
			elems.push_back(*iter);
		}
		++iter;
	}

	return elems;
}

vector<pair<string, string>> parseHRMLQueries(vector<string> unparsedQueries) {
	auto splitQueries = unparsedQueries | transform([](string query) {
		vector<string> split_query = split_by_regex(query, regex("[.~]"));
		return split_query;
		})
		| to<vector<vector<string>>>();
	auto lengthToProperty = accumulate(splitQueries.begin(), splitQueries.end(), vector<pair<string, string>>{},
		[](auto acc, auto query_token) {
			auto length = query_token.size();
			acc.emplace_back(query_token[length - 2], query_token[length - 1]);
			return acc;
		});
	return lengthToProperty;
};

int entry() {
	/* Enter your code here. Read input from STDIN. Print output to STDOUT */
	// read from stdin

	auto unparsedHRMLTags = vector<string>{
		"<tag1 value = \"HelloWorld\">",
		"<tag2 name = \"Name1\">",
		"<tag3 name=\"Name1\" other=\"blairisaweeb\">",
		"</tag2>",
		"</tag1>" };
	auto unparsedQueries = vector<string>{
		"tag1.tag2~name",
		"tag1~name",
		"tag1~value",
		"tag1.tag2~name"};

	auto openingTagsView = unparsedHRMLTags | filter([](string str) {
		return str[1] != '/';
		});
	auto unparsedHRMLOpeningTags = vector(openingTagsView.begin(), openingTagsView.end());
	//// elements *should* be fully populated
	auto elements = parseHRMLOpeners(unparsedHRMLOpeningTags);
	auto queryWithAttribute = parseHRMLQueries(unparsedQueries);

	for (auto pair : queryWithAttribute) {
		auto tagname = pair.first;
		string attribute = pair.second;
		Element queriedProperty = elements[tagname];
		unordered_map<string, string> props = queriedProperty.properties;
		auto thing = props.count(attribute);
		bool key_exists = props.find(attribute) != props.end();
		if (!key_exists) {
			cout << "Not Found!" << endl;
		}
		else {
			string property = props[attribute]; // create a value in a map???? reee
			cout << property << endl;
		}
	}
	return 0;
}