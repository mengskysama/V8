/// @file log.h
/// @brief 日志模块

#ifndef _LOG_H_
#define _LOG_H_
#undef _LOG_H_

#include <stdio.h>
#include <time.h>

namespace utils
{

    #ifndef MAX_PATH
    /// 支持的最大文件路径长度
    #define MAX_PATH    1024
    #endif

    /// 日志级别
    enum LogLevel
    {
	    L_FATAL = 0,    ///< 致命错误，程序不得不停止
	    L_ERROR,        ///< 故障，如网络连接错误
	    L_WARN,         ///< 警告，不影响业务的故障
	    L_INFO,         ///< 业务信息记录
	    L_TRACE,        ///< 程序流程跟踪
	    L_DEBUG,        ///< 调试信息
        L_LEVEL_MAX
    };

    /**
        日志类

        
        日志文件名格式：
            - xxx-20081105-090000.log

        日志文件分割：
            - 新的一天开始，或者文件大小超过设定值，打开新文件写入

        用法：
            <CODE>
            #include <log.h>

            LOG_INIT("xxx", 400000, L_TRACE);

            LOG_OPEN();

            LOG(3)("warn info %d", 3);

            LOG_CLOSE();

            </CODE>
        */
    class Log
    {
    public:
        /**
            构造Log对象
        */
	    Log();

        /**
            析构Log对象
            
            析构前将关闭日志文件
            @see close()
        */
	    ~Log();

    public:
        /**
            设置日志文件名
            
            如果已经打开了就不能再设置了
            @param filename 新的日志文件名
            @return 0 成功 -1 出现错误
        */
	    int set_file_name(const char *filename);

        /**
            设置日志文件规模上限
            
            不立即生效，在下次输出日志时才检查
            @param maxsize 新的规模上限
            @return 0 成功 -1 出现错误
        */
	    int set_max_size(int maxsize);

        /**
            设置最大日志等级         
              
            只对后续输出的日志有效
            @param level 新的级别上限
            @return 0 成功 -1 出现错误
        */
	    int set_max_level(LogLevel level);

        /**
            获取日志文件规模上限
            
            @return 日志文件规模上限
        */
	    size_t get_max_size() {return max_size_;}

        /**
            获取最大日志等级           
            
            @return 最大日志等级
        */
        LogLevel get_max_level() {return max_level_;}

        /**
            打开日志文件

            打开前需先设置文件名
            @return 0 成功 -1 出现错误
            @see set_file_name()
        */
	    int open();

        /**
            关闭日志文件

            关闭后将重命名日志文件
            @return 0 成功 -1 出现错误
            @see set_file_name()
        */
	    int close();

    #ifdef WIN32	// for windows
    #define CHECK_FORMAT(i, j)
    #else			// for linux(gcc)
    #define CHECK_FORMAT(i, j) __attribute__((format(printf, i, j)))
    #endif

        /**
            输出一条日志记录

            @param level 日志等级
            @param fmt 格式化字符串
            @return 0 成功 -1 出现错误
        */
        int log(LogLevel level, const char * fmt, ...) CHECK_FORMAT(3, 4);

        /// 输出一条FATAL日志记录
	    int log_fatal(const char * fmt, ...) CHECK_FORMAT(2, 3);

        /// 输出一条ERROR日志记录
        int log_error(const char * fmt, ...) CHECK_FORMAT(2, 3);

        /// 输出一条WARN日志记录
        int log_warn(const char * fmt, ...) CHECK_FORMAT(2, 3);

        /// 输出一条INFO日志记录
        int log_info(const char * fmt, ...) CHECK_FORMAT(2, 3);

        /// 输出一条TRACE日志记录
        int log_trace(const char * fmt, ...) CHECK_FORMAT(2, 3);

        /// 输出一条DEBUG日志记录
        int log_debug(const char * fmt, ...) CHECK_FORMAT(2, 3);

    #undef CHECK_FORMAT

        /**
            用十六进制dump一段数据

            @param data 数据首地址
            @param len 数据长度
            @param level 日志等级
            @return 0 成功 -1 出现错误
        */
        int log_hex(unsigned char * data, size_t len, LogLevel level);

    public:
        /// 全局日志对象
        static Log global_log;

    private:
        class Lock;

        /**
            输出一条日志记录

            @param level 日志等级
            @param fmt 格式化字符串
            @param ap 格式化参数
            @return 0 成功 -1 出现错误
            @see log()
        */
	    int vlog(int level, const char* fmt, va_list ap);

    private:
	    /// 不同日志级别的颜色以及关键字描述
	    static char level_str_[L_LEVEL_MAX][64];

    private:
	    /// 日志文件名
	    char file_name_[MAX_PATH];

	    /// 单个日志文件最大文件大小
	    size_t max_size_;

        /// 日志级别
        LogLevel max_level_;

	    /// 日志文件文件描述符
	    FILE * file_;

	    /// 线程锁
	    Lock * lock_;

	    /// 今天开始时刻
        time_t mid_night_;

    };

}

#define LOG(level) LOG_##level

#define LOG_0 LOG_FATAL
#define LOG_1 LOG_ERROR
#define LOG_2 LOG_WARN
#define LOG_3 LOG_INFO
#define LOG_4 LOG_TRACE
#define LOG_5 LOG_DEBUG

#define LOG_FATAL //utils::Log::global_log.log_fatal
#define LOG_ERROR //utils::Log::global_log.log_error
#define LOG_WARN //utils::Log::global_log.log_warn
#define LOG_INFO //utils::Log::global_log.log_info
#define LOG_TRACE //utils::Log::global_log.log_trace
#define LOG_DEBUG //utils::Log::global_log.log_debug

#define LOG_HEX(data, len, level) utils::Log::global_log.log_hex((unsigned char *)(data), (len), (level))

#define LOG_INIT(name, size, level) \
    utils::Log::global_log.set_file_name(name); \
    utils::Log::global_log.set_max_size(size); \
    utils::Log::global_log.set_max_level((utils::LogLevel)(level))

#define LOG_OPEN() utils::Log::global_log.open()

#define LOG_CLOSE() utils::Log::global_log.close()

#define LOG_SET_LEVEL(level) utils::Log::global_log.set_max_level((utils::LogLevel)(level))

#define LOG_GET_LEVEL() utils::Log::global_log.get_max_level()

#endif
