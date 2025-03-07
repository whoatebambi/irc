#pragma once
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include "Server.hpp"
#include <ctime>


class CliSocket;

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::string _key;
        std::string _topicEditor;
        std::string _founderMask;

        std::set<int> _clients;
        std::set<std::string> _banlist; //full mask
        std::set<std::string> _whitelist; //full mask
        std::set<std::string>  _ops; //full mask

        size_t _limClients;
        std::map<char, bool> _modes; // rajouter check limite user a join

        time_t _topicTime;
        time_t _creationTime;


    public:
        Channel(CliSocket *client, std::string name);
        Channel(CliSocket *client, std::string name, std::string key);
        ~Channel();

        void joinChan(CliSocket *client, std::string chanName, std::string key);
        bool canJoin(CliSocket *client, std::string key);

        static void removeFromList(std::set<int> &list, int fd);
        static void removeFromList(std::set<std::string> &list, std::string mask);
        static void removeFromList(std::set<int> &list, std::string mask);
        bool isEmpty();

        bool checkMode(char c);

        std::set<int> &getMembers();
        std::set<std::string> &getBanList();
        std::set<std::string> &getWhiteList();
        std::set<std::string> &getOpsList();
        std::string getName();
        std::string &getFounderMask();
        std::string getKey();
        std::string getLimitSize();

        std::string getTopicTime();
        std::string getTopic();
        std::string getTopicEditor();
        std::string getCreationTime();

        void setTopicInfo(std::string topic, std::string nickname);
        std::string modeString(void);


/////////////////////////////////////////////////////////////

        void addOp(const std::string &mask, CliSocket *target, CliSocket *client);
        void delOp(const std::string &nicmaskk, CliSocket *target, CliSocket *client);

        void addLimit(const int &limit, CliSocket *client);
        void delLimit(CliSocket *client);

        void addKey(const std::string &key, CliSocket *client);
        void delKey(CliSocket *client);

        void addInvOnly(CliSocket *client); //checker si peut prendre un client en argument auquel cas doit pouvoir prendre un argument
        void delInvOnly(CliSocket *client);

        void addNeedOpTopic(CliSocket *client);
        void delNeedOpTopic(CliSocket *client);

        void addBanList(CliSocket *target, CliSocket *client);
        void delBanList(CliSocket *target, CliSocket *client);
};

