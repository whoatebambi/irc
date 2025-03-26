#include "Command.hpp"

// need to check edge cases with ":"
std::vector<std::string> Command::splitArgs(const std::string &input)
{
	std::vector<std::string> vec;
	std::istringstream iss(input);
	std::string word;
	bool trailing = false;

	while (iss >> word)
	{
		if (!trailing && word[0] == ':')
		{
			trailing = true;
			vec.push_back(input.substr(input.find(':') + 1));
			break;
		}
		vec.push_back(word);
	}
	return (vec);
}

std::vector<std::string> Command::splitString(const std::string &str, char delimiter) 
{
	std::vector<std::string> result;
	std::string item;
	std::stringstream ss(str);

	if (!str.empty() && str[0] == delimiter)
		result.push_back("");

	while (std::getline(ss, item, delimiter))
		result.push_back(item);

	if (!str.empty() && str[str.length() - 1] == delimiter)
		result.push_back("");

	return (result);
}
