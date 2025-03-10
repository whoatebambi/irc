#pragma once

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <cstdarg>


class ErrMsg
{
private:
    typedef void (ErrMsg::*sendErr)(std::ostringstream &, va_list);
    std::map<int, sendErr> replies;

    void rpl_Umodeis(std::ostringstream &msg, va_list args);
    void rpl_Inviting(std::ostringstream &msg, va_list args);

    void rpl_ChannelModeIs(std::ostringstream &msg, va_list args);
    void rpl_CreationTime(std::ostringstream &msg, va_list args);
    void rpl_NamReply(std::ostringstream &msg, va_list args);
    void rpl_EndofNames(std::ostringstream &msg, va_list args);
    void rpl_BanList(std::ostringstream &msg, va_list args);
    void rpl_EndofBanList(std::ostringstream &msg, va_list args);
    
    void err_NoSuchNick(std::ostringstream &msg, va_list args);
    void err_NoSuchChannel(std::ostringstream &msg, va_list args);
    void err_CannotSendToChan(std::ostringstream &msg, va_list args);
    void err_NoRecipient(std::ostringstream &msg, va_list args);
    void err_NoTextToSend(std::ostringstream &msg, va_list args);
    void err_NoNicknameGiven(std::ostringstream &msg, va_list args);
    void err_ErroneusNickname(std::ostringstream &msg, va_list args);
    void err_NicknameInUse(std::ostringstream &msg, va_list args);
    void err_UserNotInChannel(std::ostringstream &msg, va_list args);
    void err_NotRegistered(std::ostringstream &msg, va_list args);
    void err_NotOnChannel(std::ostringstream &msg, va_list args);
    void err_UserOnChannel(std::ostringstream &msg, va_list args);
    void err_NeedMoreParams(std::ostringstream &msg, va_list args);
    void err_AlreadyRegistered(std::ostringstream &msg, va_list args);
    void err_PasswdMismatch(std::ostringstream &msg, va_list args);
    void err_ChannelIsFull(std::ostringstream &msg, va_list args);
    void err_UnknownMode(std::ostringstream &msg, va_list args);
    void err_InviteOnlyChan(std::ostringstream &msg, va_list args);
    void err_BannedFromChan(std::ostringstream &msg, va_list args);
    void err_BadChannelKey(std::ostringstream &msg, va_list args);
    void err_BadChanMask(std::ostringstream &msg, va_list args);
    void err_ChanOpPrivsNeeded(std::ostringstream &msg, va_list args);
    void err_UserDontMatch(std::ostringstream &msg, va_list args);
    void err_InvalidKey(std::ostringstream &msg, va_list args);


public:
    ErrMsg();
    std::string getErr(int code, va_list &args);
};