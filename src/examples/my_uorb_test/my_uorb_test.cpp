/****************************************************************************
 *
 *   Copyright (c) 2012-2022 PX4 Development Team. All rights reserved.
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

#include <px4_platform_common/px4_config.h>  // 包含 PX4 配置头文件
#include <px4_platform_common/tasks.h>      // 包含 PX4 任务头文件
#include <px4_platform_common/posix.h>      // 包含 POSIX 兼容头文件
#include <unistd.h>                         // 包含 UNIX 标准头文件
#include <stdio.h>                          // 包含标准输入输出头文件
#include <poll.h>                           // 包含 poll 函数头文件
#include <string.h>                         // 包含字符串操作头文件
#include <math.h>                           // 包含数学函数头文件

#include <px4_platform_common/log.h>
#include <px4_platform_common/app.h>

#include <uORB/uORB.h>                      // 包含 uORB（微消息传递）头文件
#include <uORB/topics/sensor_combined.h>    // 包含传感器组合话题头文件
#include <uORB/topics/vehicle_attitude.h>   // 包含飞行器姿态话题头文件

extern "C" int my_uorb_test_main(int argc, char *argv[]);  // 声明主函数并导出

int my_uorb_test_main(int argc, char *argv[])
{
	PX4_INFO("Hello Sky!");  // 输出 "Hello Sky!" 信息

	/* 订阅 sensor_combined 话题 */
	int sensor_sub_fd = orb_subscribe(ORB_ID(sensor_combined));
	/* 将更新频率限制为 5 Hz */
	orb_set_interval(sensor_sub_fd, 200);

	/* 广播姿态话题 */
	struct vehicle_attitude_s att;  // 定义姿态结构体
	memset(&att, 0, sizeof(att));   // 初始化姿态结构体

	orb_advert_t att_pub = orb_advertise(ORB_ID(vehicle_attitude), &att);  // 广播姿态话题

	/* 使用此技术可以等待多个话题，这里只使用一个 */
	px4_pollfd_struct_t fds[] = {
		{ .fd = sensor_sub_fd,   .events = POLLIN },  // 定义文件描述符数组，监听 sensor_combined 话题
		/* 这里可以有更多的文件描述符，例如：
		 * { .fd = other_sub_fd,   .events = POLLIN },
		 */
	};

	int error_counter = 0;  // 错误计数器

	for (int i = 0; i < 5; i++) {
		/* 等待一个文件描述符的传感器更新，超时时间为 1000 毫秒（1 秒） */
		int poll_ret = px4_poll(fds, 1, 1000);

		/* 处理 poll 的结果 */
		if (poll_ret == 0) {
			/* 这意味着没有数据提供者向我们提供数据 */
			PX4_ERR("Got no data within a second");

		} else if (poll_ret < 0) {
			/* 这是非常严重的错误，应该是紧急情况 */
			if (error_counter < 10 || error_counter % 50 == 0) {
				/* 使用计数器防止信息泛滥（并减慢我们的速度） */
				PX4_ERR("ERROR return value from poll(): %d", poll_ret);
			}

			error_counter++;  // 错误计数器递增

		} else {

			if (fds[0].revents & POLLIN) {
				/* 获取第一个文件描述符的数据 */
				struct sensor_combined_s raw;  // 定义传感器组合数据结构体
				/* 将传感器原始数据复制到本地缓冲区 */
				orb_copy(ORB_ID(sensor_combined), sensor_sub_fd, &raw);
				PX4_INFO("Accelerometer:\t%8.4f\t%8.4f\t%8.4f",
					 (double)raw.accelerometer_m_s2[0],
					 (double)raw.accelerometer_m_s2[1],
					 (double)raw.accelerometer_m_s2[2]);

				/* 设置姿态并广播此信息供其他应用程序使用
				 下面的代码没有实际意义，只是一个例子
				*/
				att.q[0] = raw.accelerometer_m_s2[0];
				att.q[1] = raw.accelerometer_m_s2[1];
				att.q[2] = raw.accelerometer_m_s2[2];

				orb_publish(ORB_ID(vehicle_attitude), att_pub, &att);  // 广播姿态数据
			}

			/* 这里可以有更多的文件描述符，例如：
			 * if (fds[1..n].revents & POLLIN) {}
			 */
		}
	}

	PX4_INFO("exiting");  // 输出退出信息

	return 0;  // 返回 0 表示程序正常结束
}



int my_uorb_test_begin(int argc, char *argv[])
{
	PX4_INFO("now begining uorb test");


	return OK;
}
