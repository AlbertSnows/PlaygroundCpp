#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <ranges>
#include <numeric>
#include <string>
#include <sstream>
#include <unordered_map>
using namespace std;

vector<string> tokenized_unparsed_dates(string date) {
	vector<string> tokens;
	stringstream tokenStream(date);
	string token;

	while (getline(tokenStream, token, ' ')) {
		tokens.push_back(token);
	}
	return tokens;
}

unordered_map<string, string> month_phr_to_num = {
		{"Jan", "01"},
		{"Feb", "02"},
		{"Mar", "03"},
		{"Apr", "04"},
		{"May", "05"},
		{"Jun", "06"},
		{"Jul", "07"},
		{"Aug", "08"},
		{"Sep", "09"},
		{"Oct", "10"},
		{"Nov", "11"},
		{"Dec", "12"}
};
unordered_map<string, string> day_phr_to_num = {
		{"1st", "01"},
		{"2nd", "02"},
		{"3rd", "03"},
		{"21st", "21"},
		{"22nd", "22"},
		{"23rd", "23"},
		{"31st", "31"}
};

string parse_day(string unp_day) {
	auto is_special = month_phr_to_num.find(unp_day) != month_phr_to_num.end();
	if(is_special) {
		return day_phr_to_num[unp_day];
	}
	else {
		return unp_day.substr(0, unp_day.length() - 2);
	}
}

void ppd_entry() {
	vector<string> lines = {
		"20th Oct 2052",
		"6th Jun 1933",
		"26th May 1960",
		"20th Sep 1958",
		"16th Mar 2068",
		"25th May 1912",
		"16th Dec 2018",
		"26th Dec 2061",
		"4th Nov 2030",
		"28th Jul 1963" };
	string line;
	while (getline(cin, line) && !line.empty()) {
		lines.push_back(line);
	}


		


	auto tokenized = accumulate(lines.begin(), lines.end(), vector<vector<string>>{}, [](auto acc, string unparsed_date) {
		acc.push_back(tokenized_unparsed_dates(unparsed_date));
		return acc;
		});
	auto parsed = accumulate(tokenized.begin(), tokenized.end(), vector<string>{}, [](auto acc, vector<string> token_data) {
		auto parsed_day = parse_day(token_data[0]);
		auto parse_month = month_phr_to_num[token_data[1]];
		auto parse_year = token_data[2];
		auto combined = parse_year + "-" + parse_month + "-" + parsed_day;
		acc.push_back(combined);
		return acc;
		});
	for (auto formatted_date : parsed) {
		cout << formatted_date << endl;
	}
}