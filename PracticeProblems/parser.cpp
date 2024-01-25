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

unordered_map<string, Element> parseHRMLOpeners(vector<string> unparsedHRMLOpeningTags) {
	auto opening_string = unparsedHRMLOpeningTags
		| transform([](const string& str) { 
		string string_without_end_tags = str.substr(1, str.length() - 2);
		return string_without_end_tags; 
		})
		| to<vector<string>>();
	auto HTRMLOpeningTokens = opening_string
		| transform([](const string& str) {
		vector<string> split_string = (split(str, ' ') | to<vector<string>>());
		return split_string;
			})
		| to <vector<vector<string>>>();
	vector<string> tagnames = HTRMLOpeningTokens
		| transform([](const vector<string>& opener_list) { return opener_list[0]; })
		| to<vector<string>>();
	auto attribute_tokens = HTRMLOpeningTokens
		| transform([](const vector<string>& opener_list) {
		return vector<string>(opener_list.begin() + 1, opener_list.end());
			})
		| to<vector<vector<string>>>();
	auto attribute_list = attribute_tokens
		| transform([](const vector<string>& attribute_token_list) {
		auto tokenIndexes = range(attribute_token_list.size());
		auto equalIndexes = tokenIndexes
			| filter([&attribute_token_list](const auto& i) {
			string token = attribute_token_list[i];
			return token == "="; });
		auto attributePairs = accumulate(equalIndexes.begin(), equalIndexes.end(), vector<pair<string, string>>{},
			[&attribute_token_list](vector<pair<string, string>> acc, const auto& index) {
				auto stripped_attribute = attribute_token_list[index + 1];
				stripped_attribute.erase(remove(stripped_attribute.begin(), stripped_attribute.end(), '\"'), stripped_attribute.end());
				auto attributePair = pair(attribute_token_list[index - 1], stripped_attribute);
				acc.push_back(attributePair);
				return acc;
			});
		return attributePairs;
			})
		| to<vector<vector<pair<string, string>>>>();
	auto tagIndexes = range(tagnames.size()); // sublist?
	vector<Element> elements = accumulate(tagIndexes.begin(), tagIndexes.end(), vector<Element>{},
		[tagnames, attribute_list](vector<Element> elements, const int& index) {
			auto attribute_map = unordered_map<string, string>(attribute_list[index].begin(), attribute_list[index].end());
			auto element = Element(tagnames[index], attribute_map);
			elements.push_back(element);
			return elements;
		});
	for (auto index : vector(0, tagIndexes.size() - 1)) {
		auto current_element = elements[index];
		auto next_element = elements[index + 1];
		current_element.addChildren(next_element);
	}
	auto nameToElement = accumulate(elements.begin(), elements.end(), unordered_map<string, Element>{}, [](auto acc, auto element) {
		acc.emplace(element.name, element);
		return acc;
		});
	return nameToElement;
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
	//4 3
	//    < / tag2>
	//    < / tag1>
	//    tag1.tag2~name
	//    tag1~name
	//    tag1~value
	auto unparsedHRMLTags = vector<string>{
		"<tag1 value = \"HelloWorld\">",
		"<tag2 name = \"Name1\">",
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