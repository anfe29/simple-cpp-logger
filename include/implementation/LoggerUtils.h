//
// Created by Adrien BLANCHET on 25/04/2021.
//

#ifndef SIMPLE_CPP_LOGGER_LOGGERUTILS_H
#define SIMPLE_CPP_LOGGER_LOGGERUTILS_H

#include <iostream>

#if defined(_WIN32)
// Windows
#include <windows.h>
#include <psapi.h>
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__APPLE__) && defined(__MACH__)
// MacOS
#include <unistd.h>
#include <sys/resource.h>
#include <mach/mach.h>
#include <sys/ioctl.h>
#include <Logger.h>

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
// Linux
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <sys/ioctl.h>

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
// AIX and Solaris
#include <unistd.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <procfs.h>
#include <sys/ioctl.h>

#else
// Unsupported
#endif

// stripping the full path
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

namespace LoggerUtils{

  class LastCharBuffer : public std::streambuf {
    // An external class is need to keep track of the last char printed.
    // It can't be handled by the Logger since each time a logger is called, it is deleted after the ";"
  public:
    LastCharBuffer() = default;
    void setStreamBuffer(std::streambuf* buf_){
      _streamBufferPtr_ = buf_;
      // no buffering, overflow on every char
      setp(nullptr, nullptr);
    }
    const char& getLastChar() const { return _lastChar_; }
    int_type overflow(int_type c) override {
      if( _streamBufferPtr_ != nullptr ) _streamBufferPtr_->sputc(char(c));
      _lastChar_ = char(c);
      return c;
    }

  private:
    std::streambuf* _streamBufferPtr_{nullptr};
    char _lastChar_{traits_type::eof()};
  };

  //! String Utils
  inline bool doesStringContainsSubstring(std::string string_, std::string substring_, bool ignoreCase_ = false);
  inline std::string toLowerCase(std::string& inputStr_);
  inline std::string stripStringUnicode(const std::string &inputStr_);
  inline std::string repeatString(const std::string &inputStr_, int amount_);
  inline std::string removeRepeatedCharacters(const std::string &inputStr_, const std::string &doubledChar_);
  inline std::string replaceSubstringInString(const std::string &input_str_, const std::string &substr_to_look_for_, const std::string &substr_to_replace_);
  inline std::vector<std::string> splitString(const std::string& input_string_, const std::string& delimiter_);
  inline std::string formatString( const std::string& strToFormat_ ); // 0 args overrider
  template<typename ... Args> inline std::string formatString( const std::string& strToFormat_, const Args& ... args );

  // Hardware Utils
  inline int getTerminalWidth();

}

// Implementation:
namespace LoggerUtils{

// String Utils
  inline bool doesStringContainsSubstring(std::string string_, std::string substring_, bool ignoreCase_){
    if(substring_.size() > string_.size()) return false;
    if(ignoreCase_){
      string_ = toLowerCase(string_);
      substring_ = toLowerCase(substring_);
    }
    if(string_.find(substring_) != std::string::npos) return true;
    else return false;
  }
  inline std::string toLowerCase(std::string& inputStr_){
    std::string output_str(inputStr_);
    std::transform(output_str.begin(), output_str.end(), output_str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return output_str;
  }
  inline std::string stripStringUnicode(const std::string &inputStr_){
    std::string outputStr(inputStr_);

    if(LoggerUtils::doesStringContainsSubstring(outputStr, "\033")){
      // remove color
      std::string tempStr;
      auto splitOuputStr = LoggerUtils::splitString(outputStr, "\033");
      for(const auto& sliceStr : splitOuputStr){
        if(sliceStr.empty()) continue;
        if(tempStr.empty()){
          tempStr = sliceStr;
          continue;
        }
        // look for a 'm' char that determines the end of the color code
        bool mCharHasBeenFound = false;
        for(const char& c : sliceStr){
          if(not mCharHasBeenFound){
            if(c == 'm'){
              mCharHasBeenFound = true;
            }
          }
          else{
            tempStr += c;
          }
        }
      }
      outputStr = tempStr;
    }

    outputStr.erase(
        remove_if(
            outputStr.begin(), outputStr.end(),
            [](const char& c){return !isprint( static_cast<unsigned char>( c ) );}
        ),
        outputStr.end()
    );

    return outputStr;
  }
  inline std::string repeatString(const std::string &inputStr_, int amount_){
    std::string outputStr;
    if(amount_ <= 0) return outputStr;
    for(int i_count = 0 ; i_count < amount_ ; i_count++){
      outputStr += inputStr_;
    }
    return outputStr;
  }
  inline std::string removeRepeatedCharacters(const std::string &inputStr_, const std::string &doubledChar_) {
    std::string doubledCharStr = doubledChar_+doubledChar_;
    std::string outStr = inputStr_;
    std::string lastStr;
    do{
      lastStr = outStr;
      outStr = LoggerUtils::replaceSubstringInString(outStr, doubledCharStr, doubledChar_);
    } while( lastStr != outStr );
    return outStr;
  }
  inline std::string replaceSubstringInString(const std::string &input_str_, const std::string &substr_to_look_for_, const std::string &substr_to_replace_) {
    std::string stripped_str = input_str_;
    size_t index = 0;
    while ((index = stripped_str.find(substr_to_look_for_, index)) != std::string::npos) {
      stripped_str.replace(index, substr_to_look_for_.length(), substr_to_replace_);
      index += substr_to_replace_.length();
    }
    return stripped_str;
  }
  inline std::vector<std::string> splitString(const std::string& input_string_, const std::string& delimiter_) {

    std::vector<std::string> output_splited_string;

    const char *src = input_string_.c_str();
    const char *next;

    std::string out_string_piece;

    while ((next = std::strstr(src, delimiter_.c_str())) != nullptr) {
      out_string_piece = "";
      while (src != next) {
        out_string_piece += *src++;
      }
      output_splited_string.emplace_back(out_string_piece);
      /* Skip the delimiter_ */
      src += delimiter_.size();
    }

    /* Handle the last token */
    out_string_piece = "";
    while (*src != '\0')
      out_string_piece += *src++;

    output_splited_string.emplace_back(out_string_piece);

    return output_splited_string;

  }
  inline std::string formatString( const std::string& strToFormat_ ){
    return strToFormat_;
  }
  template<typename ... Args> inline std::string formatString( const std::string& strToFormat_, const Args& ... args ) {
    size_t size = snprintf(nullptr, 0, strToFormat_.c_str(), args ...) + 1; // Extra space for '\0'
    if (size <= 0) { throw std::runtime_error("Error during formatting."); }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, strToFormat_.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
  }

  // Hardware related tools
  inline int getTerminalWidth(){
    int outWith;
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
      outWith = (int)(csbi.dwSize.X);
  //    outWith = (int)(csbi.dwSize.Y);
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__) \
    || (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__))) \
    || (defined(__APPLE__) && defined(__MACH__))
    struct winsize w{};
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    outWith = (int)(w.ws_col);
    //    outWith = (int)(w.ws_row);
#endif // Windows/Linux
    return outWith;
  }

}

#endif //SIMPLE_CPP_LOGGER_LOGGERUTILS_H
