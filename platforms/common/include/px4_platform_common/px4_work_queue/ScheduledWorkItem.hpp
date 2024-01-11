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

#include "WorkItem.hpp"

#include <drivers/drv_hrt.h>

namespace px4
{
// 定义一个名为 ScheduledWorkItem 的类，继承自 WorkItem 类。
class ScheduledWorkItem : public WorkItem
{
public:

    // 检查是否已经安排了工作项。如果没有调用，则返回 true。
    bool Scheduled() { return !hrt_called(&_call); }

    /**
     * ScheduleDelayed 方法：
     * 安排下一次运行，并延迟一定的微秒数。
     *
     * @param delay_us		延迟的时间，以微秒为单位。
     */
    void ScheduleDelayed(uint32_t delay_us);

    /**
     * ScheduleOnInterval 方法：
     * 安排重复运行，可选延迟。
     *
     * @param interval_us	运行间隔时间，以微秒为单位。
     * @param delay_us		延迟的时间（可选），以微秒为单位，默认为 0。
     */
    void ScheduleOnInterval(uint32_t interval_us, uint32_t delay_us = 0);

    /**
     * ScheduleAt 方法：
     * 在特定时间安排下一次运行。
     *
     * @param time_us		特定的时间，以微秒为单位。
     */
    void ScheduleAt(hrt_abstime time_us);

    /**
     * ScheduleClear 方法：
     * 清除所有已安排的工作。
     */
    void ScheduleClear();

protected:

    // ScheduledWorkItem 的构造函数。初始化 WorkItem。
    ScheduledWorkItem(const char *name, const wq_config_t &config) : WorkItem(name, config) {}

    // ScheduledWorkItem 的虚析构函数。
    virtual ~ScheduledWorkItem() override;

    // 用于打印运行状态的虚函数。
    virtual void print_run_status() override;

private:

    // 纯虚函数 Run，子类必须实现这个方法。
    virtual void Run() override = 0;

    // 静态函数，用作调度的跳板函数。
    static void	schedule_trampoline(void *arg);

    // hrt_call 类型的成员变量，用于管理调度。
    hrt_call	_call{};
};

} // namespace px4
