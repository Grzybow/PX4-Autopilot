/****************************************************************************
 *
 *   Copyright (C) 2015 Mark Charlebois. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file hello_example.h
 * Example app for Linux
 *
 * @author Mark Charlebois <charlebm@gmail.com>
 */

// 防止同一个头文件被多次包含。这是一个常见的 C++ 技巧，确保编译器只处理一次头文件。
#pragma once

// 包含 PX4 平台的应用程序接口头文件。这允许使用 PX4 特定的应用程序功能。
#include <px4_platform_common/app.h>

// 定义一个名为 HelloExample 的类。
class HelloExample
{
public:
    // 类构造函数。此处为空，但可用于初始化代码。
    HelloExample() {}

    // 类析构函数。此处为空，但可用于清理代码。
    ~HelloExample() {}

    // 主函数声明。这是类的主要入口点。
    int main();

    // 静态成员变量，用于跟踪应用程序状态。这是 PX4 应用程序的常见模式，
    // 允许在整个应用程序中访问和修改应用程序状态。
    static px4::AppState appState; /* track requests to terminate app */
};
