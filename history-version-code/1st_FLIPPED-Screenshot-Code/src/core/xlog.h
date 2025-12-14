// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef XLOG_H
#define XLOG_H

#include <iostream>
#ifdef _WIN32
    #define SPDLOG_WCHAR_TO_UTF8_SUPPORT  // needs to be defined before spdlog to support the output of wchar_t
#endif

#include <QStandardPaths>
#include <QApplication>
#include <QDebug>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/rotating_file_sink.h"

class XLog
{
public:
    static XLog* instance() {
        static XLog instance;
        return &instance;
    }  

    std::shared_ptr<spdlog::logger> getLogger() { return m_logger; }
    void setLevel(const std::string val) { m_level = val; }

private:
    XLog() {
        std::string path = QStandardPaths::standardLocations(QStandardPaths::TempLocation).first().toStdString() + "/flipped_debug.log";

        qDebug()<< "Log Path:" << QString::fromStdString(path);
        m_logger = spdlog::rotating_logger_mt("fileLogger", path, 1024 * 1024 * 20, 10, true);
        m_logger->set_pattern("%Y-%m-%d %H:%M:%S %t %l [%!] %v");

        setLevel("debug");
        spdlog::level::level_enum logLevel(spdlog::level::n_levels);
        if (m_level == "trace") {
            logLevel = spdlog::level::trace;
        } else if (m_level == "debug") {
            logLevel = spdlog::level::debug;
        } else if (m_level == "info") {
            logLevel = spdlog::level::info;
        } else if (m_level == "warn") {
            logLevel = spdlog::level::warn;
        } else if (m_level == "error") {
            logLevel = spdlog::level::err;
        } else if (m_level == "critical") {
            logLevel = spdlog::level::critical;
        } else if (m_level == "off") {
            logLevel = spdlog::level::off;
        } else {
            m_logger->set_level(spdlog::level::n_levels);
        }

        m_logger->set_level(logLevel);
        m_logger->flush_on(logLevel);
    }

    ~XLog() { spdlog::drop_all();}          // must do this
    XLog(XLog&&) = delete;
    XLog(const XLog&) = delete;
    void operator= (const XLog&) = delete;

private:
    std::string m_level;
    std::shared_ptr<spdlog::logger> m_logger;
};

// use embedded macro to support file and line number
#define XLOG_TRACE(...)    SPDLOG_LOGGER_CALL(XLog::instance()->getLogger().get(), spdlog::level::trace, __VA_ARGS__)
#define XLOG_DEBUG(...)    SPDLOG_LOGGER_CALL(XLog::instance()->getLogger().get(), spdlog::level::debug, __VA_ARGS__)
#define XLOG_INFO(...)     SPDLOG_LOGGER_CALL(XLog::instance()->getLogger().get(), spdlog::level::info, __VA_ARGS__)
#define XLOG_WARN(...)     SPDLOG_LOGGER_CALL(XLog::instance()->getLogger().get(), spdlog::level::warn, __VA_ARGS__)
#define XLOG_ERROR(...)    SPDLOG_LOGGER_CALL(XLog::instance()->getLogger().get(), spdlog::level::err, __VA_ARGS__)
#define XLOG_CRITICAL(...) SPDLOG_LOGGER_CALL(XLog::instance()->getLogger().get(), spdlog::level::critical, __VA_ARGS__)
#define XLOG_OFF(...)      SPDLOG_LOGGER_CALL(XLog::instance()->getLogger().get(), spdlog::level::off, __VA_ARGS__)

#endif // XLOG_H
