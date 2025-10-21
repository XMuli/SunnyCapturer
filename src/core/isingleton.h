// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2021-2024 XMuli
// SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
// SPDX-Author: XMuli <xmulitech@gmail.com>

#ifndef TSINGLETON_H
#define TSINGLETON_H

template<typename T>
class ISingleton
{
public:
    static T& instance() {
        static T instance;
        return instance;
    }

    ISingleton(const T&) = delete;
    T& operator=(const T&) = delete;
    ISingleton(T&&) = delete;
    T& operator=(T&&) = delete;

protected:
    ISingleton() = default;
    virtual ~ISingleton() = default;
};

#define FRIEND_CREAT_SINGLETION(_name)  friend class ISingleton<_name>;

#endif // TSINGLETON_H
