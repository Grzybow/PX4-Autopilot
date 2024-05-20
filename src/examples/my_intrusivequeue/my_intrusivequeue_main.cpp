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
 * @file hello_main.cpp
 * Example for Linux
 *
 * @author Mark Charlebois <charlebm@gmail.com>
 */

#include "my_intrusivequeue.h"

#include <px4_platform_common/app.h>
#include <px4_platform_common/init.h>
#include <stdio.h>
#include <iostream>
#include <iostream>
#include <queue>
#include <chrono>

void testQueuePerformance() {
    const int numElements = 100000000; // 测试的元素数量

    // 测试 IntrusiveQueue
    IntrusiveQueue<Task*> intrusiveQueue;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numElements; ++i) {
        intrusiveQueue.push(new Task(i, "Task test " + std::to_string(i)));
    }

    intrusiveQueue.size();

    while (!intrusiveQueue.empty()) {
        delete intrusiveQueue.pop();
    }

    intrusiveQueue.size();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "IntrusiveQueue time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms" << std::endl;

    // 测试 std::queue
    std::queue<Task*> stdQueue;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numElements; ++i) {
        stdQueue.push(new Task(i, "Task test " + std::to_string(i)));
    }


    while (!stdQueue.empty()) {
        delete stdQueue.front();
        stdQueue.pop();
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "std::queue time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms" << std::endl;
}



extern "C" __EXPORT int my_intrusivequeue_main(int argc, char *argv[]);


// 定义 PX4 主函数。这是应用程序的入口点。
int my_intrusivequeue_main(int argc, char **argv)
{
    // 初始化 PX4 应用程序。这通常包括设置任务名称和解析命令行参数。
    // "hello" 是这个应用程序的名称。
    px4::init(argc, argv, "hello");

    // 打印 "hello" 到控制台。这是一个简单的消息，表明程序已开始执行。
    printf("hello\n");

    IntrusiveQueue<Task*> taskQueue;



    Task *task1 = new Task(1, "Task 1");
    Task *task2 = new Task(2, "Task 2");
    Task *task3 = new Task(3, "Task 3");

    taskQueue.push(task1);
    taskQueue.push(task2);
    taskQueue.push(task3);

    printf("push DONE!\n");

    task1->main(); // 测试类的main方法

    taskQueue.size(); // Queue大小的遍历

    for (auto it = taskQueue.begin(); it != taskQueue.end(); ++it) { // Queue内容的遍历
        Task* currentTask = *it;
        std::cout << "Task ID: " << currentTask->taskId << std::endl;
    }

    // 执行完 main 方法后，打印 "goodbye" 消息，表明程序即将结束。

    testQueuePerformance(); //与普通链表对比



    printf("all done goodbye\n");

    // 返回 0，表示程序正常退出。
    return 0;
}
