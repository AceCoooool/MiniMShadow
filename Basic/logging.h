#ifndef MINIMSHADOW_LOGGING_H
#define MINIMSHADOW_LOGGING_H

#include <iostream>
#include <ctime>

namespace MiniMShadow {
    /**
     * 当前时间信息
     */
    class DateLogger {
    private:
        char buffer_[9];

    public:
        DateLogger() {}
        // 返回当前时间char*
        const char *HumanDate() {
            time_t time_value = time(nullptr);
            struct tm now;
            localtime_r(&time_value, &now);
            snprintf(buffer_, sizeof(buffer_), "%02d:%02d:%02d", now.tm_hour, now.tm_min, now.tm_sec);
            return buffer_;
        }
    };

    /**
     * 信息类: 返回当前时间+file+line信息
     */
    class LogMessage {
    private:
        DateLogger pretty_date_;

    protected:
        std::ostream &log_stream_;

    public:
        LogMessage(const char *file, int line) : log_stream_(std::cerr) {
            log_stream_ << "[" << pretty_date_.HumanDate() << "] " << file << ":" << line << ": ";
        }
        ~LogMessage() { log_stream_ << "\n"; }
        LogMessage(const LogMessage &) = delete;
        void operator=(const LogMessage &) = delete;
        std::ostream &stream() { return log_stream_; }
    };

    /**
     * 错误信息类:  返回当前时间+file+line信息 (并中断程序)
     */
    class LogMessageFatal : public LogMessage {
    public:
        LogMessageFatal(const char *file, int line) : LogMessage(file, line) {}
        ~LogMessageFatal() {
            log_stream_ << "\n";
            abort();
        }
        LogMessageFatal(const LogMessageFatal &) = delete;
        void operator=(const LogMessageFatal &) = delete;
    };

    /* ---------检查是否满足条件, 不满足返回错误信息并中断程序------------ */
#define CHECK(x)                                           \
  if (!(x))                                                \
      LogMessageFatal(__FILE__, __LINE__).stream() << "Check "  \
      "failed: " #x << ' '

// 错误检查
#define DCHECK(x) CHECK(x)
#define CHECK_LT(x, y) CHECK((x) < (y))
#define CHECK_GT(x, y) CHECK((x) > (y))
#define CHECK_LE(x, y) CHECK((x) <= (y))
#define CHECK_GE(x, y) CHECK((x) >= (y))
#define CHECK_EQ(x, y) CHECK((x) == (y))
#define CHECK_NE(x, y) CHECK((x) != (y))

// 错误信息
#define LOG_INFO LogMessage(__FILE__, __LINE__)
#define LOG_ERROR LOG_INFO
#define LOG_WARNING LOG_INFO
#define LOG_FATAL LogMessageFatal(__FILE__, __LINE__)
#define LOG_QFATAL LOG_FATAL

#define VLOG(x) LOG_INFO.stream()
#define LOG(severity) LOG_##severity.stream()   // 视为调用删除错误信息---severity为INFO, FATAL等
#define LG LOG_INFO.stream()



}

#endif //MINIMSHADOW_LOGGING_H
