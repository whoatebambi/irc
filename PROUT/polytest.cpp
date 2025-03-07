#include <iostream>
#include <vector>
#include <string>
#include <map>

class Channel 
{
public:
    void addOperator(const std::string& nickname) 
	{
        std::cout << "Adding operator: " << nickname << "\n";
    }

    void removeLimit() {
        std::cout << "Removing user limit\n";
    }

    void setKey(const std::string& key) {
        std::cout << "Setting channel key: " << key << "\n";
    }
};







// Define function signatures for command execution
typedef void (*ModeFunction)(Channel&, const std::string&); // -> goes to ModeCommand.hpp



// Function implementations examples


void modeOpAdd(Channel& channel, const std::string& nickname) 
{
    channel.addOperator(nickname);
}

void modeLimitRemove(Channel& channel, const std::string&) 
{
    channel.removeLimit();
}

void modeSetKey(Channel& channel, const std::string& key) 
{
    channel.setKey(key);
}






// Function to process mode changes dynamically
void processModeChanges(Channel& channel, const std::vector<std::pair<std::string, std::string> >& modeChanges) 

{
    // Create a map of mode commands to function pointers
    std::map<std::string, ModeFunction> modeHandlers;
	modeHandlers["+o"] = &modeOpAdd;
    modeHandlers["-l"] = &modeLimitRemove;
    modeHandlers["+k"] = &modeSetKey;
	// and so on for -o +l etc..

    // Iterate through mode changes and execute the corresponding function
    for (size_t i = 0; i < modeChanges.size(); ++i) 
	{
        std::map<std::string, ModeFunction>::iterator it = modeHandlers.find(modeChanges[i].first);

        it->second(channel, modeChanges[i].second);
    }
}