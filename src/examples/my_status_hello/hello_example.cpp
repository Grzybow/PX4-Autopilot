
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
 * @file hello_example.cpp
 * Example for Linux
 *
 * @author Mark Charlebois <charlebm@gmail.com>
 */

// 包含模块特定的头文件。这通常包含类的声明或其他相关定义。
#include "hello_example.h"

// 包含 PX4 平台的时间处理功能。用于实现延时等功能。
#include <px4_platform_common/time.h>

// 包含 unistd.h 以提供标准的系统调用和库函数，如 sleep。
#include <unistd.h>

// 包含标准输入输出库，用于 printf 等函数。
#include <stdio.h>

// 创建一个静态 AppState 实例。这个实例用于管理应用程序的状态。
px4::AppState HelloExample::appState;

// 定义主函数，这是模块的入口点。
int HelloExample::main()
{
    // 设置应用程序状态为正在运行。
    appState.setRunning(true);

    // 初始化一个计数器。
    int i = 0;

    // 使用循环执行重复工作。条件是没有退出请求并且循环次数小于5。
    while ((!appState.exitRequested()) && i < 999) {
        // 让程序休眠2秒。px4_sleep 函数提供了与操作系统无关的休眠功能。
        px4_sleep(1);

        // 打印工作信息。这里的工作是示例性的，实际工作内容可以根据需要变化。
        printf("  Doing work...\n");
        // 增加计数器。
        ++i;
    }

    // 返回 0 表示程序正常结束。这是通用的成功退出代码。
    return 0;
}
