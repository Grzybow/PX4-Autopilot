/****************************************************************************
 *
 *   Copyright (c) 2019 PX4 Development Team. All rights reserved.
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

#pragma once

#include "WorkQueueManager.hpp"
#include "WorkQueue.hpp"

#include <containers/IntrusiveQueue.hpp>
#include <containers/IntrusiveSortedList.hpp>
#include <px4_platform_common/defines.h>
#include <drivers/drv_hrt.h>
#include <lib/mathlib/mathlib.h>
#include <lib/perf/perf_counter.h>

#include <string.h>
// 命名空间 px4
namespace px4
{

    // 定义 WorkItem 类，继承自 IntrusiveSortedListNode 和 IntrusiveQueueNode
    class WorkItem : public IntrusiveSortedListNode<WorkItem *>, public IntrusiveQueueNode<WorkItem *>
    {
    public:

        // 删除默认构造函数
        WorkItem() = delete;

        // 禁止拷贝构造、赋值、移动构造和移动赋值
        WorkItem(const WorkItem &) = delete;
        WorkItem &operator=(const WorkItem &) = delete;
        WorkItem(WorkItem &&) = delete;
        WorkItem &operator=(WorkItem &&) = delete;

        // 重载 <= 操作符，用于根据名称排序 WorkItems
        bool operator<=(const WorkItem &rhs) const { return (strcmp(ItemName(), rhs.ItemName()) <= 0); }

        // 立即安排工作项执行的函数
        inline void ScheduleNow()
        {
            if (_wq != nullptr) {
                _wq->Add(this);
            }
        }

        // 用于打印运行状态的虚函数
        virtual void print_run_status();

        /**
         * 改变工作队列的函数。
         * 注意：调用者负责同步。
         *
         * @param config 工作队列配置（见 WorkQueueManager.hpp）。
         * @return 如果初始化成功返回 true
         */
        bool ChangeWorkQueue(const wq_config_t &config) { return Init(config); }

        // 获取工作项名称的函数
        const char *ItemName() const { return _item_name; }

    protected:

        // 有参构造函数
        explicit WorkItem(const char *name, const wq_config_t &config);

        // 复制构造函数
        explicit WorkItem(const char *name, const WorkItem &work_item);

        // 虚析构函数
        virtual ~WorkItem();

        /**
         * 从可运行队列中移除工作项的函数（如果存在）
         */
        void ScheduleClear();
    protected:

        // 运行前的准备函数
        void RunPreamble()
        {
            if (_run_count == 0) {
                _time_first_run = hrt_absolute_time();
                _run_count = 1;

            } else {
                _run_count++;
            }
        }

        // 友元函数，允许 WorkQueue 调用 Run
        friend void WorkQueue::Run();
        // 纯虚函数 Run，派生类必须实现
        virtual void Run() = 0;

        /**
         * 根据工作队列配置初始化 WorkItem 的函数。
         * 也可以用于切换到不同的工作队列。
         * 注意：调用者负责同步。
         *
         * @param config 工作队列配置（见 WorkQueueManager.hpp）。
         * @return 如果初始化成功返回 true
         */
        bool Init(const wq_config_t &config);
        void Deinit();

        // 计算已运行时间的函数
        float elapsed_time() const;
        // 计算平均速率的函数
        float average_rate() const;
        // 计算平均间隔时间的函数
        float average_interval() const;

        // 第一次运行时间
        hrt_abstime	_time_first_run{0};
        // 工作项名称
        const char 	*_item_name;
        // 运行次数
        uint32_t	_run_count{0};

    private:

        // 指向工作队列的指针
        WorkQueue	*_wq{nullptr};

    };
}

