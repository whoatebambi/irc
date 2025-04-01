#!/bin/bash

# Replace with your server's IP and port if needed
HOST="127.0.0.1"
PORT="6667"

{
  echo -ne "CAP LS\r\n"
  sleep 0.2

  echo -ne "PASS 1234\r\n"
  sleep 0.2

  echo -ne "NICK flo\r\n"
  sleep 0.2

  echo -ne "USER florencecousergue florencecousergue 127.0.0.1 :Florence Cousergue\r\n"
  sleep 0.2

  echo -ne "CAP END\r\n"
  sleep 0.2

  echo -ne "MODE flo +i\r\n"
  sleep 0.2

  echo -ne "PING Jean-Claude\r\n"
  sleep 0.2

  echo -ne "JOIN #test\r\n"
  sleep 0.2

  echo -ne "PRIVMSG #test :Hello from script!\r\n"
  sleep 0.2

  echo -ne "QUIT :bye\r\n"
} | nc $HOST $PORT


# # irc_test_invite_only
# {
#   echo -ne "PASS 1234\r\n"
#   sleep 0.2
#   echo -ne "NICK uninvited\r\n"
#   sleep 0.2
#   echo -ne "USER uninvited 0 * :Sneaky User\r\n"
#   sleep 0.2
#   echo -ne "JOIN #private\r\n"
#   sleep 0.2
#   echo -ne "QUIT :rejected\r\n"
# } | nc 127.0.0.1 6667

# # irc_test_join_and_privmsg
# {
#   echo -ne "PASS 1234\r\n"
#   sleep 0.2
#   echo -ne "NICK alice\r\n"
#   sleep 0.2
#   echo -ne "USER alice 0 * :Alice Test\r\n"
#   sleep 0.2
#   echo -ne "JOIN #test\r\n"
#   sleep 0.2
#   echo -ne "PRIVMSG #test :Hello from Alice!\r\n"
#   sleep 0.2
#   echo -ne "QUIT :bye\r\n"
# } | nc 127.0.0.1 6667

# # irc_test_kick
# {
#   echo -ne "PASS 1234\r\n"
#   sleep 0.2
#   echo -ne "NICK admin\r\n"
#   sleep 0.2
#   echo -ne "USER admin 0 * :Admin User\r\n"
#   sleep 0.2
#   echo -ne "JOIN #ops\r\n"
#   sleep 0.2
#   echo -ne "MODE #ops +o admin\r\n"
#   sleep 0.2
#   echo -ne "KICK #ops victim :testing kick\r\n"
#   sleep 0.2
#   echo -ne "QUIT :power down\r\n"
# } | nc 127.0.0.1 6667

# # irc_test_topic
# {
#   echo -ne "PASS 1234\r\n"
#   sleep 0.2
#   echo -ne "NICK topicuser\r\n"
#   sleep 0.2
#   echo -ne "USER topicuser 0 * :Topic Setter\r\n"
#   sleep 0.2
#   echo -ne "JOIN #test\r\n"
#   sleep 0.2
#   echo -ne "TOPIC #test :This is a scripted topic!\r\n"
#   sleep 0.2
#   echo -ne "QUIT :done\r\n"
# } | nc 127.0.0.1 6667


# # irc_test_unknown_command
# {
#   echo -ne "PASS 1234\r\n"
#   sleep 0.2
#   echo -ne "NICK glitch\r\n"
#   sleep 0.2
#   echo -ne "USER glitch 0 * :Curious User\r\n"
#   sleep 0.2
#   echo -ne "FOOBAR invalid command\r\n"
#   sleep 0.2
#   echo -ne "QUIT :testing done\r\n"
# } | nc 127.0.0.1 6667
