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
#include "parser.h"
using namespace std::views;
using std::string;
using std::unordered_map;
using std::vector;
using std::ranges::to;
using std::span;
using std::pair;
using std::accumulate;
using std::cout;
using std::endl;

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

unordered_map<string, Element> parseHRMLOpeners(vector<string> unparsedHRMLOpeningTags) {
    auto tag_view = unparsedHRMLOpeningTags 
        | transform([](const string& str) { return str.substr(1, str.length() - 2); }) 
        | to<vector<string>>();
    auto HTRMLOpeningTokens = unparsedHRMLOpeningTags 
        | transform([](const string& str) { return split(str, " ") | to<vector<string>>(); })
        | to <vector<vector<string>>>();
    auto tagnames = HTRMLOpeningTokens | transform([](const vector<string>& opener_list) { return opener_list[0]; });
    auto attribute_tokens = HTRMLOpeningTokens 
        | transform([](const vector<string>& opener_list) { 
            return vector<string>(opener_list.begin() + 1, opener_list.end()); 
            })
        | to<vector<vector<string>>>();
    auto attribute_list = attribute_tokens
        | transform([](const vector<string>& attribute_token_list) {
            //auto attributePairs = vector<pair<string, string>>();
            auto equalIndexes = vector(0, attribute_token_list.size()) 
                | filter([&attribute_token_list](const auto& i) { return attribute_token_list[i] == "="; });
            auto attributePairs = accumulate(equalIndexes.begin(), equalIndexes.end(), vector<pair<string, string>>{},
                [&attribute_token_list](vector<pair<string, string>> acc, const auto& index) {
                    auto attributePair = pair(attribute_token_list[index - 1], attribute_token_list[index + 1]);
                    acc.push_back(attributePair);
                    return acc;
                });
            return attributePairs;
            })
        | to<vector<vector<pair<string, string>>>>();
    auto tagIndexes = vector<int>(0, tagnames.size()  - 1); // sublist?
    vector<Element> elements = accumulate(tagIndexes.begin(), tagIndexes.end(), vector<Element>{}, 
        [tagnames, attribute_list](vector<Element> elements, const int& index) {
            auto attribute_map = unordered_map<string, string>(attribute_list[index].begin(), attribute_list[index].end());
            auto element = Element(tagnames[index], attribute_map);
            elements.push_back(element);
            return elements;
            });
    for (auto index : vector(0, tagIndexes.size() - 1)) {
        auto current_element= elements[index];
        auto next_element = elements[index + 1];
        current_element.addChildren(next_element);
    }
    auto nameToElement = accumulate(elements.begin(), elements.end(), unordered_map<string, Element>{}, [](auto acc, auto element) {
        acc.emplace(element.name, element);
        return acc;
        });
    return nameToElement;
    };

    vector<pair<string, string>> parseHRMLQueries(vector<string> unparsedQueries) {
        auto splitQueries = unparsedQueries | transform([](string query) {
            return split(query, "[.~]") | to<vector<string>>();
            });
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
    auto unparsedHRMLTags = vector<string>();
    auto unparsedQueries = vector<string>();

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
        string property = props[attribute];
        cout << property << endl;
    }
    return 0;
}