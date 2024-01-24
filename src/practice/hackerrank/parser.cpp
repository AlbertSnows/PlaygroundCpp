#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
// #include <ranges>
// #include <views>
using namespace std;

class Element {
    const string name;
    const unordered_map<string, string> properties;
    vector<Element> children;
    Element(string name, unordered_map<string, string> properties)
        : name(name), properties(properties) {
        // this->name = name;
        // this->properties = properties;
    }

    Element addChildren(Element child) {
        this->children.push_back(child);
        return *this;
    }
}

class Parser {

    unordered_map<string, Element> parseHRMLOpeners(vector<string> unparsedHRMLOpeningTags) {
        vector<string> unwrappedHTRMLOpeners;
        for (const string str : unparsedHRMLOpeningTags) {
            auto end = str.length() - 1;
            unwrappedHTRMLOpeners.push_back(str.substr(1, end - 1));
        }
        // vector<vector<string>> HTRMLOpeningTokens; 
        // for(const string str : unwrappedHTRMLOpeners) {
        //     HTRMLOpeningTokens.push_back(str.split(" "));
        // }
        vector<string> HTRMLOpeningTokens;
        for (const auto word : views::split(unwrappedHTRMLOpeners, " ")) {
            HTRMLOpeningTokens.push_back(word);
        }

        vector<string> names;
        transform(
            HTRMLOpeningTokens.begin(),
            HTRMLOpeningTokens.end(),
            names.begin(),
            [](vector<string> tag_tokens) { return tag_tokens[0]; });
        vector<vector<string>> attributesTokens;
        transform(
            HTRMLOpeningTokens.begin(),
            HTRMLOpeningTokens.end(),
            attributesTokens.);
        // auto attributeTokens = HTRMLOpeningTokens.map(str -> str.sublist(1, str.length));
        auto attributes =
            .map(attribute_tokens -> {
            let attributePairs = new List<Pair>();
            let indexes = List.range(0, attribute_tokens.length);
            let equalIndexes = indexes.filter(i->attributePairs[i] == "=");
            for (var equalIndex : equalIndexes) {
                let attributePair = new Pair(
                    attribute_token[equalIndex - 1],
                    attribute_token[equalIndex + 1]);
                attributePairs.add(attributePair);
            }
        });
        auto tagIndexes = List.range(0, names.length - 1);
        auto elements = HashMap<String, Element>();
        for (auto tagIndex : tagIndexes) {
            auto element = new Element(names[tagIndex], new HashMap<>(attributes[tagIndex]));
            elements.put(element.name, element);
        }
        for (auto elementIndex : tagIndexes.sublist(0, tagIndexes.length - 1)) {
            auto currentElement = elements[elementIndex];
            element.addChildren(new List<>(elements[elementIndex + 1]));
        }
        return elements;
    };

    vector<vector<string>> parseHRMLQueries(vector<string> unparsedQueries) {
        auto splitQueries = unparsedQueries.map(str->str.split("[.~]"));
        auto lengthToProperty = splitQueries.reduce(new List<>(), (pairs, str) -> {
            pairs.add(new EntrySet<>(str[str.length - 2], str[str.length - 1]));
            return pairs;
        });
        return lengthToProperty;
    };

    int main() {
        /* Enter your code here. Read input from STDIN. Print output to STDOUT */
        // read from stdin
        auto unparsedHRMLTags = new vector<string>();
        auto unparsedQueries = new vector<string>();

        auto unparsedHRMLOpeningTags = unparsedHRMLTags.filter(str->str[1] != "/");

        // elements *should* be fully populated
        auto elements = parseHRMLOpeners(unparsedHRMLOpeningTags);

        auto queryWithAttribute = parseHRMLQueries(unparsedQueries);

        queryWithAttribute.foreach((tagname, property) -> {
            auto queriedProperty = elements.get(tagname);
            auto property = queriedProperty.get(property);
            cout << property;
        });
        return 0;
    }

};
