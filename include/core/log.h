#ifndef CORE_LOG_H
#define CORE_LOG_H
#include <ctime>

namespace bib
{

template<typename T>
void message(T msg)
{
  std::cout<<msg<<"\n";
}

template <typename T>
void warning(T msg)
{
  std::cout<<"\033[1;31m"<< msg<<"\033[0m\n";
}

template<typename T>
void error(T msg)
{
  message(msg);
  throw std::runtime_error(msg);
}

enum log_level {DEBUG, INFO, WARNING, ERROR};
class logger
{
  public:
    logger(std::string filename)
    {
      logfile.open(filename, std::ios::app);
      if(!logfile){error("file not found");}
    }

    ~logger(){logfile.close();}

    void log_message(log_level level, std::string msg)
    {
        // Get current timestamp
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp),
                 "%Y-%m-%d %H:%M:%S", timeinfo);

        // Create log entry
        std::ostringstream log_entry;
        log_entry << "[" << timestamp << "] "
                 << levelToString(level) << ": " << msg
                 << std::endl;

        // Output to console
        std::cout << log_entry.str();

        // Output to log file
        if (logfile.is_open()) {
            logfile << log_entry.str();
            logfile
                .flush(); // Ensure immediate write to file
        }
    }
  private:
    std::ofstream logfile;
        // Converts log level to a string for output
    std::string levelToString(log_level level)
    {
        switch (level) {
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARNING:
            return "WARNING";
        case ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }
};
}

#endif