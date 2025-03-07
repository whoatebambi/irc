#include "ErrMsg.hpp"


ErrMsg::ErrMsg()
{
    replies[221] = &ErrMsg::rpl_Umodeis;

    replies[324] = &ErrMsg::rpl_ChannelModeIs;
    replies[329] = &ErrMsg::rpl_CreationTime;
    replies[341] = &ErrMsg::rpl_Inviting;
    replies[353] = &ErrMsg::rpl_NamReply;
    replies[366] = &ErrMsg::rpl_EndofNames;
    replies[367] = &ErrMsg::rpl_BanList;
    replies[368] = &ErrMsg::rpl_EndofBanList;
    
    replies[401] = &ErrMsg::err_NoSuchNick;
    replies[403] = &ErrMsg::err_NoSuchChannel;
    replies[404] = &ErrMsg::err_CannotSendToChan;
    replies[411] = &ErrMsg::err_NoRecipient;
    replies[412] = &ErrMsg::err_NoTextToSend;
    replies[431] = &ErrMsg::err_NoNicknameGiven;
    replies[432] = &ErrMsg::err_ErroneusNickname;
    replies[433] = &ErrMsg::err_NicknameInUse;
    replies[441] = &ErrMsg::err_UserNotInChannel;
    replies[442] = &ErrMsg::err_NotOnChannel;
    replies[443] = &ErrMsg::err_UserOnChannel;
    replies[461] = &ErrMsg::err_NeedMoreParams;
    replies[462] = &ErrMsg::err_AlreadyRegistered;
    replies[464] = &ErrMsg::err_PasswdMismatch;
    replies[471] = &ErrMsg::err_ChannelIsFull;
    replies[472] = &ErrMsg::err_UnknownMode;
    replies[473] = &ErrMsg::err_InviteOnlyChan;
    replies[474] = &ErrMsg::err_BannedFromChan;
    replies[475] = &ErrMsg::err_BadChannelKey;
    replies[476] = &ErrMsg::err_BadChanMask;
    replies[482] = &ErrMsg::err_ChanOpPrivsNeeded;
    replies[502] = &ErrMsg::err_UserDontMatch;
    replies[525] = &ErrMsg::err_InvalidKey;


}

std::string ErrMsg::getErr(int code, va_list &args)
{
    std::ostringstream msg;
    msg << code << " ";

    std::map<int, sendErr>::iterator it = replies.find(code);
    if (it != replies.end())
        (this->*(it->second))(msg, args);
    else
        msg << "Unkown error code: " << code;
    return msg.str();
}


// ---------REPLIES---------

void ErrMsg::rpl_Umodeis(std::ostringstream &msg, va_list args)
{
    const char* rawClient = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
          
    msg << client;
}

void ErrMsg::rpl_ChannelModeIs(std::ostringstream &msg, va_list args)
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    const char* rawMode = va_arg(args, const char*);
    const char* rawKey = va_arg(args, const char*);
    const char* rawLimit = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");
    std::string mode = (rawMode ? std::string(rawMode) : "");
    std::string key = (rawKey ? std::string(rawKey) : "");
    std::string limit = (rawLimit ? std::string(rawLimit) : "");
          
    msg << client << " " << channel << " " << mode << " " << key << " " << limit;
}

void ErrMsg::rpl_CreationTime(std::ostringstream &msg, va_list args)
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    const char* rawTime = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");
    std::string time = (rawTime ? std::string(rawTime) : "");
          
    msg << client << " " << channel << " " << rawTime;
}

void ErrMsg::rpl_Inviting(std::ostringstream &msg, va_list args)
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawNick = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string nick = (rawNick ? std::string(rawNick) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");
          
    msg << client << " " << nick << " " << channel;
}

void ErrMsg::rpl_NamReply(std::ostringstream &msg, va_list args)
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    const char* rawList = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");
    std::string list = (rawList ? std::string(rawList) : "");

    msg << client << " " << channel << " :" << list;
}

void ErrMsg::rpl_EndofNames(std::ostringstream &msg, va_list args)
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");
          
    msg << client << " " << channel << " :End of /NAMES list";
}

void ErrMsg::rpl_BanList(std::ostringstream &msg, va_list args)
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    const char* rawList = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");
    std::string list = (rawList ? std::string(rawList) : "");

    msg << client << " " << channel << " " << list;
}

void ErrMsg::rpl_EndofBanList(std::ostringstream &msg, va_list args)
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");
          
    msg << client << " " << channel << " :End of channel ban list";
}

// -----------ERRORS---------------

void ErrMsg::err_NoSuchNick(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawNick = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string nick = (rawNick ? std::string(rawNick) : "");      

    msg << client << " " << nick << " :No such nick/channel";
}

void ErrMsg::err_NoSuchChannel(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << client << " " << channel << " :No such channel";
}

void ErrMsg::err_CannotSendToChan(std::ostringstream &msg, va_list args)
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << client << " " << channel << " :Cannot send to channel";
}

void ErrMsg::err_NoRecipient(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");

    msg << client << " :No recipient given";
}

void ErrMsg::err_NoTextToSend(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");

    msg << client << " :No text to send";
}

void ErrMsg::err_NoNicknameGiven(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");

    msg << client << " :No nickname given";
}

void ErrMsg::err_ErroneusNickname(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawNick = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string nick = (rawNick ? std::string(rawNick) : "");

    msg << client << " " << nick << " :Erroneous nickname";
}

void ErrMsg::err_NicknameInUse(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawNick = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string nick = (rawNick ? std::string(rawNick) : "");

    msg << client << " " << nick << " :Nickname is already in use";
}

void ErrMsg::err_UserNotInChannel(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawNick = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string nick = (rawNick ? std::string(rawNick) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << client << " " << nick << " " << channel << " :They aren't on that channel";
}


void ErrMsg::err_NotOnChannel(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << client << " " << channel << " :You're not on that channel";
}

void ErrMsg::err_UserOnChannel(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawNick = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string nick = (rawNick ? std::string(rawNick) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << client << " " << nick << " " << channel << " :is already on channel";
}

void ErrMsg::err_NotRegistered(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");

    msg << client << " :You have not registered";
}

void ErrMsg::err_NeedMoreParams(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawCustom = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string custom = (rawCustom ? std::string(rawCustom) : "");
    
    msg << client << " " << (custom.empty() ? ":Not enough parameters" : custom);
}

void ErrMsg::err_AlreadyRegistered(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");

    msg << client << " :You may not reregister";
}

void ErrMsg::err_PasswdMismatch(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");

    msg << client << " :Password incorrect";
}

void ErrMsg::err_ChannelIsFull(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << client << " " << channel << " :Cannot join channel (+l)";
}

void ErrMsg::err_UnknownMode(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawMode = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string Mode = (rawMode ? std::string(rawMode) : "");

    msg << client << " " << rawMode << " MODE :is unknown mode char to me";
}


void ErrMsg::err_InviteOnlyChan(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << client << " " << channel << " :Cannot join channel (+i)";
}

void ErrMsg::err_BannedFromChan(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << client << " " << channel << " :Cannot join channel (+b)";
}

void ErrMsg::err_BadChannelKey(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << client << " " << channel << " :Cannot join channel (+k)";
}

void ErrMsg::err_BadChanMask(std::ostringstream &msg, va_list args) 
{
    const char* rawChannel = va_arg(args, const char*);
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << channel << " :Bad Channel Mask";
}

void ErrMsg::err_ChanOpPrivsNeeded(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawChannel = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string channel = (rawChannel ? std::string(rawChannel) : "");

    msg << client << " " << channel << " :You're not channel operator";
}

void ErrMsg::err_UserDontMatch(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");

    msg << client << " :Cant change mode for other users";
}

void ErrMsg::err_InvalidKey(std::ostringstream &msg, va_list args) 
{
    const char* rawClient = va_arg(args, const char*);
    const char* rawKey = va_arg(args, const char*);
    std::string client = (rawClient ? std::string(rawClient) : "");
    std::string key = (rawKey ? std::string(rawKey) : "");      

    msg << client << " " << key << " :Key is not well-formed";
}