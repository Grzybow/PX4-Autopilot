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
 * @file hello_start_posix.cpp
 *
 * @author Thomas Gubler <thomasgubler@gmail.com>
 * @author Mark Charlebois <mcharleb@gmail.com>
 */
#include "hello_example.h"

#include <px4_platform_common/log.h>
#include <px4_platform_common/app.h>
#include <px4_platform_common/tasks.h>
#include <stdio.h>
#include <string.h>
#include <sched.h>

static int daemon_task;             /* Handle of deamon task / thread */


// 这是模块的主函数，用于处理命令行参数。
extern "C" __EXPORT int my_status_hello_main(int argc, char *argv[]);

int my_status_hello_main(int argc, char *argv[])
{
    // 检查是否有足够的命令行参数。如果参数少于2个，则显示用法并返回错误代码1。
    if (argc < 2) {
        PX4_WARN("usage: my_status_hello_main {start|stop|status}\n");
        return 1;
    }

    // 如果第一个参数是 "start"，则执行启动操作。help
    if (!strcmp(argv[1], "start")) {

        // 检查应用程序是否已经在运行。如果是，则打印信息并返回。
        if (HelloExample::appState.isRunning()) {
            PX4_INFO("already running\n");
            return 0;
        }

        // 启动一个新任务。这里 "hello" 是任务名，后面是任务的优先级、堆栈大小和入口函数。
        // argv[2] 及其后的参数传递给新任务。
        daemon_task = px4_task_spawn_cmd("my_status_hello_main",
                                         SCHED_DEFAULT,
                                         SCHED_PRIORITY_MAX - 5,
                                         2000,
                                         PX4_MAIN,
                                         (argv) ? (char *const *)&argv[2] : (char *const *)nullptr);

        return 0;



/*
`px4_task_spawn_cmd` 函数在 PX4 Autopilot 代码库中用于创建和启动一个新的任务（通常是一个线程或进程）。这个函数的参数通常包括：

1. **任务名称** (`"hello"`): 这是新任务的名称。在这个例子中，任务被命名为 "hello"。

2. **调度策略** (`SCHED_DEFAULT`): 定义任务的调度策略。`SCHED_DEFAULT` 通常表示使用系统默认的调度策略。

3. **调度优先级** (`SCHED_PRIORITY_MAX - 5`): 设置任务的优先级。`SCHED_PRIORITY_MAX - 5` 表示任务的优先级是最大可能优先级减去 5。这在多任务处理中用来确定任务的执行顺序。

4. **栈大小** (`2000`): 指定任务的栈大小（以字节为单位）。在这个例子中，栈大小被设置为 2000 字节。

5. **入口函数** (`PX4_MAIN`): 指向任务的入口函数。这个函数在任务启动时被调用。

6. **参数** (`(argv) ? (char *const *)&argv[2] : (char *const *)nullptr`):
传递给任务的参数。这里的表达式是一个三元运算符，
用于判断 `argv` 是否为非空。如果 `argv` 非空，则传递 `argv[2]`
开始的参数；如果为空，则传递 `nullptr`。

这个函数通常在 PX4 Autopilot 项目中用于创建后台任务或服务，例如数据采集、设备控制等。
每个参数都是为了确保任务可以按照预定的方式被正确创建和管理。
*/

    }
    // 如果第一个参数是 "stop"，请求应用程序退出。
    if (!strcmp(argv[1], "stop")) {

        HelloExample::appState.requestExit();

        PX4_INFO("stop!!!\n");
        HelloExample::appState.setRunning(false);
         return 0;
    }

    // 如果第一个参数是 "status"，显示应用程序的运行状态。
    if (!strcmp(argv[1], "status")) {
        if (HelloExample::appState.isRunning()) {
            PX4_INFO("is running\n");
        } else {
            PX4_INFO("not started\n");
        }

        return 0;
    }

    // 如果参数不是 "start"、"stop" 或 "status"，则打印用法并返回错误代码1。
    PX4_WARN("usage: my_status_hello_main {start|stop|status}\n");
    return 1;

}

