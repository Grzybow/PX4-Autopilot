// 包含 PX4 平台的日志功能头文件。这允许模块打印信息到控制台。
#include <px4_platform_common/log.h>

// 包含 PX4 应用程序接口的头文件，用于开发基于 PX4 平台的应用程序。
#include <px4_platform_common/app.h>

// 声明一个符合 PX4 模块接口的函数。__EXPORT 确保在不同编译环境下都可以正确导出此函数。
extern "C" __EXPORT int hello_main(int argc, char *argv[]);

// 定义主函数。这是模块的入口点。
int hello_main(int argc, char *argv[])
{
    // 使用 PX4_INFO 宏打印一条信息。这将显示在控制台上。
    PX4_INFO("Hello, I am a dynamically loaded module.");

    // 打印“Argv:”，表示接下来将输出命令行参数。
    PX4_INFO("Argv:");

    // 循环遍历传递给程序的所有命令行参数。
    for (int i = 0; i < argc; ++i) {
        // 对于每个参数，打印其索引和值。这有助于用户了解如何传递参数给模块。
        PX4_INFO("  %d: %s", i, argv[i]);
    }

    // 返回 0 表示程序正常结束。在 PX4 模块中，返回值通常用于指示成功或错误状态。
    return 0;
}

