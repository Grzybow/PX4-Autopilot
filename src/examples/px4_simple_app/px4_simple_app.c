/****************************************************************************
 *
 *   Copyright (c) 2012-2019 PX4 Development Team. All rights reserved.
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
 * @file px4_simple_app.c
 * Minimal application example for PX4 autopilot
 *
 * @author Example User <mail@example.com>
 */

#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/posix.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <math.h>

#include <uORB/uORB.h>
#include <uORB/topics/vehicle_acceleration.h>
#include <uORB/topics/vehicle_attitude.h>

__EXPORT int px4_simple_app_main(int argc, char *argv[]);

int px4_simple_app_main(int argc, char *argv[])
{

// 输出信息到控制台，显示 "Hello Sky!"。
PX4_INFO("Hello Sky!");

// 订阅 vehicle_acceleration 主题，以接收加速度传感器的数据。
int sensor_sub_fd = orb_subscribe(ORB_ID(vehicle_acceleration));
// 设置数据更新间隔为 200 毫秒，即每秒更新 5 次。
orb_set_interval(sensor_sub_fd, 200);

// 定义并初始化 vehicle_attitude 结构体，用于存储飞行器的姿态信息。
struct vehicle_attitude_s att;
// 使用 memset 将 att 结构体的内存区域初始化为 0。
memset(&att, 0, sizeof(att));
// 宣告并发布 vehicle_attitude 主题，使用 att 结构体作为数据。
orb_advert_t att_pub = orb_advertise(ORB_ID(vehicle_attitude), &att);

// 定义一个 px4_pollfd_struct_t 类型的数组，用于后续的轮询操作。
px4_pollfd_struct_t fds[] = {
    { .fd = sensor_sub_fd,   .events = POLLIN }, // 将 sensor_sub_fd 文件描述符加入轮询列表，关注 POLLIN 事件。
    // 可以添加更多文件描述符和事件。
};

// 定义错误计数器，用于跟踪轮询过程中的错误次数。
int error_counter = 0;

// 开始一个循环，循环 5 次。
for (int i = 0; i < 5; i++) {
    // 调用 px4_poll 函数，等待最多 1000 毫秒来检查是否有数据更新。
    int poll_ret = px4_poll(fds, 1, 1000);

    // 根据 px4_poll 的返回值处理不同的情况。
    if (poll_ret == 0) {
        // 如果返回 0，表示在指定时间内没有任何数据更新。
        PX4_ERR("Got no data within a second");

    } else if (poll_ret < 0) {
        // 如果返回值小于 0，表示发生了错误。
        // 使用计数器限制错误消息的频率。
        if (error_counter < 10 || error_counter % 50 == 0) {
            // 输出错误信息，包括 poll 返回的具体错误值。
            PX4_ERR("ERROR return value from poll(): %d", poll_ret);
        }
        // 错误计数器递增。
        error_counter++;

    } else {
        // 如果返回值大于 0，表示有数据更新。
        if (fds[0].revents & POLLIN) {
            // 检查第一个文件描述符是否有数据更新。
            struct vehicle_acceleration_s accel; // 定义加速度数据结构体。
            // 从 ORB 中复制加速度数据到 accel 结构体中。
            orb_copy(ORB_ID(vehicle_acceleration), sensor_sub_fd, &accel);
            // 输出加速度数据。
            PX4_INFO("Accelerometer:\t%8.4f\t%8.4f\t%8.4f",
                     (double)accel.xyz[0],
                     (double)accel.xyz[1],
                     (double)accel.xyz[2]);

            // 示例操作：将加速度数据赋值给姿态数据的某些字段。
            att.q[0] = accel.xyz[0];
            att.q[1] = accel.xyz[1];
            att.q[2] = accel.xyz[2];
            // 发布更新后的姿态信息。
            orb_publish(ORB_ID(vehicle_attitude), att_pub, &att);
        }
        // 这里可以处理更多的文件描述符。
    }
}

	PX4_INFO("exiting");

	return 0;
}
