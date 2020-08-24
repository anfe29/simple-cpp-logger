//
// Created by Nadrino on 24/08/2020.
//

#include "Logger.h"

int main(){

  LogInfo << "-----------------------------------------" << std::endl;
  LogInfo << "Welcome to the simple-cpp-logger example." << std::endl;
  LogInfo << "-----------------------------------------" << std::endl;

  Logger::quietLineJump();

  LogInfo << "First, let's add a prefix for all calls in this source file..." << std::endl;
  Logger::setUserPrefixStr("[LoggerExample]");
  LogInfo << "There you go :)." << std::endl;

  LogInfo.quietLineJump();

  int *i_ptr = new int();
  LogInfo("You can use %s style calls: here is a pointer \"%p\"", "printf()", i_ptr);
  delete i_ptr;

  LogInfo.quietLineJump();

  LogInfo << "-----------------------------------------" << std::endl;
  LogInfo << "Here are all the different log levels:" << std::endl;
  LogInfo << "-----------------------------------------" << std::endl;

  LogFatal << 0 << ": Is a Logger::LogLevel::FATAL" << std::endl;
  LogError << 1 << ": Is a Logger::LogLevel::ERROR" << std::endl;
  LogAlert << 2 << ": Is a Logger::LogLevel::ALERT" << std::endl;
  LogWarning << 3 << ": Is a Logger::LogLevel::WARNING" << std::endl;
  LogInfo << 4 << ": Is a Logger::LogLevel::INFO" << std::endl;
  LogDebug << 5 << ": Is a Logger::LogLevel::DEBUG" << std::endl;
  LogTrace << 6 << ": Is a Logger::LogLevel::TRACE" << std::endl;

  LogInfo.quietLineJump();

  LogInfo << "-----------------------------------------" << std::endl;
  LogInfo << "You can set the maximum log level of a given source file." << std::endl;
  LogInfo << "-----------------------------------------" << std::endl;

  Logger::setMaxLogLevel(Logger::LogLevel::INFO);
  LogInfo << "Here for example, we have set the max log level to INFO:" << std::endl;
  LogInfo << "It means than all printouts from FATAL to INFO will be printed," << std::endl;
  LogInfo << "but not the DEBUG and TRACE." << std::endl;
  LogWarning << "-> You can see me :) (but not the next message which is a DEBUG)" << std::endl;
  LogDebug << "-> You cannot see me :(" << std::endl;
  LogAlert << "* Note that Logger::setMaxLogLevel will only set the max log level for this source file." << std::endl;
  LogAlert << "* This is because simple-cpp-logger is a header-only library." << std::endl;
  LogAlert << "* In fact, all (static) members of the Logger are defined within a given source file." << std::endl;
  LogAlert << "* To apply this parameter globally, set the variable in your cmake file." << std::endl;
  Logger::setMaxLogLevel(Logger::LogLevel::TRACE);

  LogInfo.quietLineJump();

  LogInfo << "-----------------------------------------" << std::endl;
  LogInfo << "You can also specify the header print level:" << std::endl;
  LogInfo << "-----------------------------------------" << std::endl;
  Logger::setPrefixLevel(Logger::PrefixLevel::NONE);
  LogDebug << "NONE -> No prefix at all. (or the userPrefixStr if you set it before)" << std::endl;
  Logger::setPrefixLevel(Logger::PrefixLevel::MINIMAL);
  LogDebug << "MINIMAL -> Only the log level." << std::endl;
  Logger::setPrefixLevel(Logger::PrefixLevel::PRODUCTION);
  LogDebug << "PRODUCTION -> Add the clock." << std::endl;
  Logger::setPrefixLevel(Logger::PrefixLevel::DEBUG);
  LogDebug << "DEBUG -> Add the filename and the line number." << std::endl;
  Logger::setPrefixLevel(Logger::PrefixLevel::FULL);
  LogDebug << "FULL -> Add the current thread id." << std::endl;

  Logger::quietLineJump();

  LogInfo << "-----------------------------------------" << std::endl;
  LogInfo << "Colors can also be disabled:" << std::endl;
  LogInfo << "-----------------------------------------" << std::endl;

  Logger::setEnableColors(false);
  LogInfo << "Here is a colorless message" << std::endl;
  Logger::setEnableColors(true);

  Logger::quietLineJump();

  LogInfo("test lol \nnew line ?");
  LogInfo << std::string("test lol \nnew line ?") << std::endl;
  LogInfo << "test lol \nnew line ?" << std::endl;

  Logger::quietLineJump();

  LogInfo << "-----------------------------------------" << std::endl;
  LogInfo << "Have Fun! :)" << std::endl;
  LogInfo << "-----------------------------------------" << std::endl;

}