//
// Created by Nadrino on 24/08/2020.
//

#ifndef SIMPLE_CPP_LOGGER_LOGGER_H
#define SIMPLE_CPP_LOGGER_LOGGER_H


#include <string>
#include <mutex>
#include <vector>


#ifndef LOGGER_MAX_LOG_LEVEL_PRINTED
#define LOGGER_MAX_LOG_LEVEL_PRINTED   6
#endif

#ifndef LOGGER_PREFIX_LEVEL
#define LOGGER_PREFIX_LEVEL   2
#endif

#ifndef LOGGER_ENABLE_COLORS
#define LOGGER_ENABLE_COLORS   1
#endif

#ifndef LOGGER_ENABLE_COLORS_ON_USER_HEADER
#define LOGGER_ENABLE_COLORS_ON_USER_HEADER   0
#endif

#ifndef LOGGER_PREFIX_FORMAT
#define LOGGER_PREFIX_FORMAT "{TIME} {USER_HEADER} {SEVERITY} {FILELINE} {THREAD}"
#endif

#ifndef LOGGER_CLEAR_LINE_BEFORE_PRINT
#define LOGGER_CLEAR_LINE_BEFORE_PRINT 0
#endif

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LogFatal       (Logger{Logger::LogLevel::FATAL,    __FILENAME__, __LINE__})
#define LogError       (Logger{Logger::LogLevel::ERROR,    __FILENAME__, __LINE__})
#define LogAlert       (Logger{Logger::LogLevel::ALERT,    __FILENAME__, __LINE__})
#define LogWarning     (Logger{Logger::LogLevel::WARNING,  __FILENAME__, __LINE__})
#define LogInfo        (Logger{Logger::LogLevel::INFO,     __FILENAME__, __LINE__})
#define LogDebug       (Logger{Logger::LogLevel::DEBUG,    __FILENAME__, __LINE__})
#define LogTrace       (Logger{Logger::LogLevel::TRACE,    __FILENAME__, __LINE__})


namespace LoggerUtils{

  class LastCharBuffer;

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


namespace {

  class Logger {

  public:

    enum class LogLevel {
      FATAL   = 0,
      ERROR   = 1,
      ALERT   = 2,
      WARNING = 3,
      INFO    = 4,
      DEBUG   = 5,
      TRACE   = 6
    };
    enum class PrefixLevel {
      NONE        = 0,
      MINIMAL     = 1,
      PRODUCTION  = 2,
      DEBUG       = 3,
      FULL        = 4
    };

    //! Setters
    static void setMaxLogLevel(int maxLogLevel_);
    static void setMaxLogLevel(LogLevel maxLogLevel_);
    static void setEnableColors(bool enableColors_);
    static void setPropagateColorsOnUserHeader(bool propagateColorsOnUserHeader_);
    static void setPrefixLevel(PrefixLevel prefixLevel_);
    static void setUserHeaderStr(const std::string &userHeaderStr_);
    static void setPrefixFormat(const std::string &prefixFormat_);

    //! Getters
    static int getMaxLogLevelInt();
    static LogLevel getMaxLogLevel();
    static std::string getPrefixString();
    static std::string getPrefixString(Logger loggerConstructor); // Supply the logger (via macros) to update the prefix

    // User Methods
    static void quietLineJump();


    // Macro-Related Methods
    // Those intended to be called using the above preprocessor macros
    Logger(LogLevel logLevel_, char const * fileName_, int lineNumber_); // constructor used by the macros
    ~Logger();

    // For printf-style calls
    template <typename... TT> void operator()(const char *fmt_str, TT && ... args);

    // For std::cout-style calls
    template<typename T> Logger &operator<<(const T &data);
    Logger &operator<<(std::ostream &(*f)(std::ostream &));


  protected:

    static void buildCurrentPrefix();

    static void hookStreamBuffer();
    static std::string getTagColorStr(LogLevel selectedLogLevel_);
    static std::string getTagStr(LogLevel selectedLogLevel_);

    template<typename ... Args> static void printFormat(const char *fmt_str, Args ... args );


  private:

    // internal
    static LogLevel _currentLogLevel_;
    static std::string _currentFileName_;
    static int _currentLineNumber_;
    static std::string _currentPrefix_;
    static bool _isNewLine_;
    static std::ostream& _outputStream_;
    static std::mutex _loggerMutex_;

    // parameters
    static bool _enableColors_;
    static bool _disablePrintfLineJump_;
    static bool _propagateColorsOnUserHeader_;
    static bool _cleanLineBeforePrint_;
    static LogLevel _maxLogLevel_;
    static PrefixLevel _prefixLevel_;
    static std::string _userHeaderStr_;
    static std::string _prefixFormat_;

  };

}

#include "implementation/Logger.impl.h"


#endif //SIMPLE_CPP_LOGGER_LOGGER_H
