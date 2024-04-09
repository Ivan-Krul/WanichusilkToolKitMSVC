#pragma once
#include <string>
#include <vector>

#include "LogMessage.h"

class LogWriter {
protected:
    LogWriter(const std::string& className) noexcept;

public:
    void setOptionOutputLogs() noexcept;
    void outputLogs() noexcept;

protected:
    void clearLogs() noexcept;
    void writeErr(const std::string& message, size_t line, bool except = false) noexcept;

private:
    std::vector<LogMessage> mLogMessages;
    std::string mClassName;

    bool mOptionOutputLogs = false;
};



inline LogWriter::LogWriter(const std::string& className) noexcept {
    mClassName = className;
}

inline void LogWriter::setOptionOutputLogs() noexcept {
    mOptionOutputLogs = true;
}

inline void LogWriter::clearLogs() noexcept {
    mLogMessages.clearLogs();
}

inline void LogWriter::writeErr(const std::string& message, size_t line, bool except) noexcept {
    mLogMessages.push_back(LogMessage(except ? mClassName + ":Exception" : mClassName, "In " + std::to_string(line) + " active line " + message));
}

inline void LogWriter::outputLogs() noexcept {
    if (mOptionOutputLogs) {
        if (mLogMessages.empty())
            mLogMessages.push_back(LogMessage(mClassName, "Maybe everything is okay"));
        for (auto& message : mLogMessages) {
            message.print();
        }
    }
}
