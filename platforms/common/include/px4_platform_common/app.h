/****************************************************************************
 *
 * Copyright (c) 2015 Mark Charlebois. All rights reserved.
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
 * @file app.h
 *
 * PX4 app template classes, functions and defines. Apps need to call their
 * main function PX4_MAIN.
 */


// 确保这个头文件只被包含一次
#pragma once

// 定义一个命名空间 px4
namespace px4
{

// 声明一个名为 AppState 的类
class AppState
{
public:
    // 析构函数，当对象被销毁时调用
    ~AppState() {}

    // 默认构造函数，初始化_exitRequested和_isRunning为false
    AppState() : _exitRequested(false), _isRunning(false) {}

    // 一个公共方法，用来检查是否请求退出
    bool exitRequested() { return _exitRequested; }

    // 一个公共方法，用来设置退出请求的状态
    void requestExit() { _exitRequested = true; }

    // 一个公共方法，用来检查应用程序是否正在运行
    bool isRunning() { return _isRunning; }

    // 一个公共方法，用来设置应用程序的运行状态
    void setRunning(bool running) { _isRunning = running; }

protected:
    // 一个保护成员变量，用来跟踪是否有退出请求
    bool _exitRequested;

    // 一个保护成员变量，用来跟踪应用程序是否正在运行
    bool _isRunning;

private:
    // 禁止使用复制构造函数
    AppState(const AppState &);

    // 禁止使用赋值运算符
    const AppState &operator=(const AppState &);
};

} // namespace px4 结束

// 用于基于任务/进程的构建的宏定义
#ifdef PX4_MAIN
extern int PX4_MAIN(int argc, char *argv[]);
#endif
