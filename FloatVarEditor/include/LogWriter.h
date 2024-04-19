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
    void writeErr(const std::string& message, bool except = false) noexcept;

private:
    std::vector<LogMessage> mLogMessages;
    std::string mClassName;

    bool mOptionOutputLogs = false;

protected:
    size_t mLine = 0;
};



inline LogWriter::LogWriter(const std::string& className) noexcept {
    mClassName = className;
}

inline void LogWriter::setOptionOutputLogs() noexcept {
    mOptionOutputLogs = true;
}

inline void LogWriter::clearLogs() noexcept {
    mLogMessages.clear();
    mLine = 0;
}

inline void LogWriter::writeErr(const std::string& message, bool except) noexcept {
    mLogMessages.push_back(LogMessage(except ? mClassName + ":Exception" : mClassName, "In " + std::to_string(mLine) + " active line " + message));
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
