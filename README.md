# 一个可以不使用ARM-MDK并使用标准外设库进行开发的STM32F4工程样例

>本工程为参考模板，使用 **STM32F429IGT6** 基于**VSCode** + **CMake** + **Ninja** + **ARM-GCC** + **OpenOCD**开发环境下，使用**STM32标准外设库**进行开发的一个FreeRTOS工程样例。



## 写在前面

> 首先，感谢43512同学对本项目的大力支持，他完成了艰难的从0到1的第一步，是本项目重要的奠基者，笔者只是站在巨人（存疑）的肩膀上。

**ARM-MDK**是一个成熟的开发环境，具有高度集成、易于上手等优势，为什么笔者要抛弃这样一个完善的开发流程，转而自行构建一套新流程呢？
> **ARM-MDK**非开源，非跨平台（❌）

> 笔者周末闲的没事，想折腾了（✔）

在‌‌**Vibe Coding**盛行的年代，不会使用AI辅助编程简直就是傻逼，而**ARM-MDK**过于老旧的界面以及对AI工具的0支持导致笔者从开学以来一直都不想用它，已有的解决方案，使用‌**VSCode‌**+拓展‌**Keil Assistant‌**的方案非常鸡肋，基于此，笔者决定是时候拥抱更开源的工具了（

相比传统的 IDE，本项目具有以下优势：
 - **跨平台与完全开源**

    ARM-GCC、CMake、Ninja、OpenOCD 均为开源工具，避免商业授权限制。

    能在 Windows、Linux、macOS 上保持一致的构建方式，使开发环境更灵活、更可控。

 - **简洁高效的构建系统**
   
    CMake 作为配置层，使项目结构更加模块化、可维护。

    Ninja 作为构建器，拥有极快的增量构建速度，显著提升开发效率。
 - **更适合自动化与持续集成（CI/CD）**
   
    相比 Keil 的工程文件格式不透明，CMake 可直接整合进 GitHub Actions、GitLab CI、Jenkins 等自动化工作流。

    构建过程脚本化，可轻松复现，减少“环境不一致”问题。

 - **最重要的**，在VSCode上，你可以感受到最完美的AI Agent支持，虽然有时候AI也是傻逼（

> 那么有人就要问了，哎你为什么不用‌**STM32CubeMX‌**+‌**CMake‌**的解决方案呢？这也是STM官方大力推广的做法。

> 那当然是因为‌**STM32CubeMX‌**可能好用，但好用不太可能，且‌**STM32CubeMX‌**生成的代码模板更适合搭配‌**CLion‌**，在‌**VSCode‌**上的拓展冗余繁复，编译/烧录/调试很难受。

> 另外，‌**STM32CubeMX‌**生成的代码模板臃肿不堪，笔者极其讨厌他的代码风格以及工程结构。

> 2025-12-7 更新：完成了基于 **STM32F4xx_StdPeriph_Driver** 的 CMake 工程模板，这下标准库/HAL库任君选择了。

> 如果你习惯使用 **HAL** 库，可以参考本项目的 **HAL 库版本** ：[GitLab克隆](ssh://git@git.caty.io:8846/co_regiontechrc/aiit/23th_2025aut/23th-emt/personal_project/3233011627/template.git)


## 一、安装软件

首先，需要安装以下软件

### 1. 安装 vscode

> vscode 是一款开源的、跨平台的代码编辑器。其本身并不具备代码编译、纠错等功能，但是提供了大量的插件以完成该目的。

   - **Windows**
     
      ```URL
      https://code.visualstudio.com/Download
      ```
      浏览器里输入以上网址，下载windows安装包并安装。
      
   - **Debian系**，apt/deb 包管理器
     
      ```bash
      sudo apt update
      
      sudo apt install wget gpg
      
      wget -qO- https://packages.microsoft.com/keys/microsoft.asc | sudo gpg --dearmor -o /usr/share/keyrings/packages.microsoft.gpg
      
      echo "deb [arch=amd64 signed-by=/usr/share/keyrings/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main" | sudo tee /etc/apt/sources.list.d/vscode.list
      
      sudo apt update
      
      sudo apt install code
      ```
      依次在终端输入以上命令，即可完成安装。
      
   - **Arch系**，pacman包管理器
      ```bash
      sudo pacman -S code
      ```
      在终端输入以上命令，即可完成安装。

> **注意：** windows系统需要自行配置环境变量

### 2. 安装CMake

> CMake 是一个开源的、跨平台构建系统生成器（build system generator）。它本身不是编译器，也不直接编译代码，而是帮你生成“构建系统”。

> CMake 是建筑设计师，不是施工队。它画出一份蓝图（Makefile / Ninja file），告诉编译器该怎么干活。

CMake 通过读取 **CMakeLists.txt（项目配置）**、解析依赖、编译选项、目标文件、链接规则，最终生成一个给编译器使用的蓝图（本项目是build.ninja）文件。

   - windows
      ```URL
      https://cmake.org/download/
      ```
      浏览器里输入以上网址，下载 windows 版本的安装包并安装。

   - Debian系，apt/deb 包管理器
      ```bash
      sudo apt install cmake
      ```
      终端输入以上命令安装CMake。
      >**注意：** apt 安装的cmake版本大概率为 v3.28.3 左右的版本，而本项目使用 v4.1.2 版本完成构建。请确认版本是否具有差异。若有请自行升级或 **在 CMakeLists.txt 文件里更改版本号**

   - Arch系，pacman包管理器
      ```bash
      sudo pacman -S cmake
      ```
      在终端输入以上命令，即可完成安装。

> **注意：** windows系统需要自行配置环境变量

### 3. 安装ninja

> Ninja 是一个开源的、跨平台的、小而快（相较于Makefile）的构建系统执行器，用来根据依赖关系高效执行编译任务。

> Ninja 就像建筑施工队，它拿着 CMake 的蓝图（build.ninja）来盖房子（编译项目）。

Ninja 先读取并对比出 **build.ninja** 中哪些文件发生了变化，再按需进行执行编译、链接任务。最终生成二进制的目标程序。

   - windows中
      ```URL
      https://github.com/ninja-build/ninja/releases
      ```
      浏览器里输入以上网址，下载 windows 版本的压缩包并解压到软件目录。

   - Debian系，apt/deb 包管理器
      ```bash
      sudo apt install ninja-build
      ```
      终端输入以上命令安装。
   
   - Arch系，pacman包管理器
      ```bash
      sudo pacman -S ninja
      ```
      在终端输入以上命令，即可完成安装。

>**注意：** windows系统需要自行配置环境变量

### 4. 安装arm-gnu

> “arm-gnu” 实际上指的是一整套为 ARM 处理器编译代码的 GNU 工具链（GNU Toolchain for ARM）。它是由 ARM 官方 或 Linaro（ARM 生态公司） 发布和维护的开源的、跨平台的交叉编译器套件。

> 最后是“工人” —— 也就是编译器工具链。在 ARM 的世界里，这个工人叫 arm-none-eabi-gcc 或简称 arm-gnu。

这里使用的是 **arm-none-eabi-gcc**：
```
arm-none-eabi
│   │    └── eabi = Embedded Application Binary Interface（嵌入式ABI）
│   └────── none = 无操作系统（裸机环境）
└────────── arm = 目标架构
```

   - windows
        ```URL
        https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
        ```
        浏览器里输入以上网址，下载 windows 版本的压缩包并解压到软件目录。

   - Debian系，apt/deb 包管理器
      ```bash
      sudo apt install gcc-arm-none-eabi gdb-multiarch arm-none-eabi-gdb
      ```
      终端输入以上命令安装。
   
   - Arch系，pacman包管理器
      ```bash
      yay -S arm-gnu-toolchain-arm-none-eabi-bin
      ```
      在终端输入以上命令，即可完成安装。
      >:memo: **注意：** 请自行确保 yay 以及 aur 可以正常使用

> **注意：** windows系统需要自行配置环境变量

>其中，-gdb结尾的工具是用来实现debug功能的工具

### 5. 安装OpenOCD

> OpenOCD 是完全开源的、跨平台的 JTAG/SWD 调试器中枢软件。

> OpenOCD 就像一座“桥”，把电脑（GDB）和目标芯片（ARM Cortex-M、RISC-V 等）连接起来，实现在真实硬件上进行调试、烧录、单步执行、查看寄存器等操作。

   - windows
      ```URL
      https://gnutoolchains.com/arm-eabi/openocd/
      ```
      浏览器里输入以上网址，下载 windows 版本的压缩包并解压到软件目录。

   - Debian系，apt/deb 包管理器
      ```bash
      sudo apt install openocd
      ```
      终端输入以上命令安装。
   
   - Arch系，pacman包管理器
      ```bash
      sudo pacman -S openocd
      ```
      在终端输入以上命令，即可完成安装。

> **注意：** windows系统需要自行配置环境变量



## 二、安装 vscode 拓展

1. **Tasks**

   Tasks 可以将 `./.vscode/tasks.json` 中定义的指令在 vscode 的底栏中封装为一个按键。

2. **C/C++**
   
    实现代码的智能补全与纠错。

3. **CMake Tools**
   
   用于在 vscode 中提供对 CMake 文件的代码提示与纠错功能。

4. **Cortex-Debug**
   
   用于在 vscode 中提供一套对代码的调试功能，其中阅读以及修改寄存器功能需要使用工具：arm-none-eabi-gdb。



## 三、项目编译过程

整个流程像一支交响乐：每个角色分工明确，但必须协奏才能让你的固件最终在 MCU 上跳起来。

>**从水下第一个生命的萌芽开始……到石器时代的巨型野兽……再到人类第一次直立行走，你已经历许多。**

---

### 故事从写代码开始

你敲代码时，如同一个作曲家。

IDE（VSCode、CLion…）背后调的是 **C/C++**：

* 它读取 C/C++ 代码
* 理解头文件依赖
* 做语法分析
* 提供智能补全、静态分析、错误提示

但
**C/C++ 不参与真正的编译和烧录**。

它只是谱务，将你创造的乐曲美观、简洁的整理出来。

---

### 工程构建：CMake 和 Ninja 开始运转

你要真正构建固件时，依靠以下的链条：

```
CMake → Ninja → arm-none-eabi-gcc
```

它们分别扮演着各自的角色：

#### CMake

严格的编曲家。
它读取你的 CMakeLists.txt，理解你的乐曲：

* 有哪些源文件
* 目标 MCU 的型号
* 编译的选项
* 链接脚本
* 包含的路径

然后它生成一个 **Ninja 构建脚本（build.ninja）** ，告诉指挥家如何安排人员奏乐。

#### Ninja

超级强大的指挥。
它不懂 MCU，不懂 C。
它只知道：

* 哪些文件要编译
* 调哪个命令
* 先后依赖是什么

于是它挥动指挥棒，把每个源文件交给真正奏曲的演员：

#### arm-none-eabi-gcc

这才是演奏的演奏者，负责演出 MCU 能欣赏的音乐。
它：

* 编译 C/C++ 代码变成 `.o`
* 链接它们变成 `.elf`
* 用 objcopy 变成 `.bin` 或 `.hex`

这里才真正生成 MCU 能读的机器码。

---

### 构建结果：得到固件

最终你得到了：

* `project.elf`（带符号，用于调试）
* `project.hex`（用于烧录）

这就是烧写进入 MCU 的固件。 

---

### 接下来是调试/烧录：OpenOCD 登场

OpenOCD 是桥梁，是 MCU 可以欣赏乐曲的席位。

```
PC 上的软件  ↔  OpenOCD  ↔  JTAG/SWD 调试器  ↔  MCU
```

它的作用：

* 通过 USB 控制你的调试器（J-Link/ST-LINK/CMSIS-DAP）
* 让你可以 debug
* 让你可以烧录 bin/elf
* 提供 GDB Server 接口

如果你要烧录：

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
    -c "program build/project.elf verify reset exit"
```

OpenOCD 把你的 `.elf` 转换成 Flash 操作命令，通过 SWD/JTAG 写进 MCU 的闪存。

---

### 最终：你的 firmware 在 MCU 上跑起来

流程就此完成。

```bash
你写的代码  
↓  
C/C++ 给你智能提示  
↓  
CMake 生成构建描述  
↓  
Ninja 按描述执行编译  
↓  
arm-none-eabi-gcc 生成最终固件  
↓  
OpenOCD 将固件烧进 MCU  
↓  
芯片开始执行你的程序
```



## 四、工程结构

当前工程的根目录结构如下：

```Tree 
根目录
.
├── /.vscode            // vscode 配置文件夹
├── /crm                // 移植后的FreeRTOS源码
├── /build              // 项目生成文件
├── /mcu                // 项目源码
├── /project            // 项目构建文件
└── README.md           // 项目自述文件
```

### **1. .vscode配置文件夹**

   ```
   .
   ├── settings.json            // vscode 关于本项目的设置
   ├── c_cpp_properties.json    // 用于配置 vscode 的代码智能提示、语法检查和代码导航功能
   └── tasks.json               // tasks 的配置文件
   ```
   - settings.json 是 vscode 中关于该 workspace 的配置文件。 vscode 打开一个文件夹后会将该文件夹视为一个 工作空间(workspace)，不同 workspace 共用一个相同的 user settings 的配置，而 workspace 里的 settings.json 中的配置会覆盖公共配置，实现 vscode 的配置隔离。

   - tasks.json 是 用来定义该 workspace 中的任务。**Tasks** 拓展 可以将一段指令封装进左下角的一个按钮里。在本工程的开发环境里，所有的编译，烧录都是在终端里输入指令实现的，而 **Tasks** 拓展可以将具体的指令封装为一个个按钮，简化开发过程。

   - c_cpp_properties.json 是用于 **C/C++** 拓展的 C/C++ IntelliSense 配置文件，使得代码智能提示、语法检查和代码导航功能等功能可以找到 ./mcu/Libraries/ 路径下的HAL库头文件、ARM GCC 编译器内置头文件、以及用户自定义的头文件。
   > 有点傻逼，路径不支持递归通配符，由于本人不喜欢clangd，那就先凑合用吧。

### **2. mcu 文件夹**

```Tree
.
├── /bsp            // 存储片上外设的源文件与头文件
├── /Libraries                   // 存储所有库文件
│   ├── /CMSIS                   // 存储核心库文件、启动文件等
│   ├── /HAL_Driver              // 存储HAL库文件
│   └── /ld                      // 存储链接文件
└── /user           // 用户文件
    ├── main.c                   // 主文件
    ├── stm32f4xx_it.c           // 存储中断函数定义
    └── stm32f4xx_it.h           // 存储中断函数声明
```

### **3. project 文件夹**

```Tree
.
├── /project        // 存储项目构建文件           
│   ├── arm-gnu-none-eabi.cmake  // 指定项目运行环境
│   └── CMakeLists.txt           // 指定项目结构
```



## 五、开启你最伟大的探索吧

> 抛下那些更遥远的事吧！演员们已将最好的样子留在台前，艺术正于此刻冉冉升起，您如何舍得挪开自己的目光？

### 1、准备工作
1. 确保在终端中输入：
```bash
code --version
cmake --version
ninja --version
arm-none-eabi-gcc --version
arm-none-eabi-gdb --version
openocd --version
```
均成功显示每个版本号信息则进入下一步。如果不行，请安装对应软件。

2. 确保vscode中已安装如下拓展：

```bash
C/C++           by      Microsoft
Tasks           by      actboy168
CMake Tools     by      Microsoft
Cortex-Debug    by      marus25
```

### 2、**Tasks**拓展介绍
成功安装拓展 **Tasks** 后，此时使用 VSCode 打开工程文件夹，可以看到 vscode 底栏有如下四个按键：

- **CMake Configure:** 调用 Cmake 生成项目构建目录.
- **CMake Build:** 通过 CMake 调用底层的构建工具（本模板使用 Ninja ）去编译、汇编、链接出目标文件和固件。
- **Flash:** 使用用 OpenOCD 工具把编译好的 .elf 固件烧录到开发板的 Flash 中，然后校验、复位并退出。
- **Clean:** 清除指令,删除 build 文件夹。当 CMake Configure 或 Cmake Build 没有通过时，请务必使用此按键清除构建目录！！！

### 3、使用流程

1. 写完一版代码后，先点击 **CMake Configure** 按键。
   一切正常的情况下，VSCode终端会显示如下内容：

```bash
 *  正在执行任务: cmake "-DCMAKE_TOOLCHAIN_FILE=E:\Code Warehouse\STM32\13-FreeRTOS-LED/project/arm-gnu-none-eabi.cmake" -DCMAKE_SYSTEM_NAME=Generic -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -GNinja -S project -B build 

-- The C compiler identification is GNU 13.3.1
-- The CXX compiler identification is GNU 13.3.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: D:/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: D:/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-g++.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- The ASM compiler identification is GNU
-- Found assembler: D:/STM32CubeCLT_1.18.0/GNU-tools-for-STM32/bin/arm-none-eabi-gcc.exe
-- Configuring done (1.2s)
-- Generating done (0.0s)
-- Build files have been written to: E:/Code Warehouse/STM32/13-FreeRTOS-LED/build
 *  终端将被任务重用，按任意键关闭。 
```
   同时 mcu 文件夹中出现一个新文件夹 build。
   >:waring: 注意：失败的构建也会产生一个 build 文件夹，请从终端信息里判断是否构建成功！！！

2. 点击 **CMake Build** 按钮。  
   一切正常的情况下，vscode的终端会显示如下内容：

```bash
 *  正在执行任务: cmake --build "E:\Code Warehouse\STM32\12-CMake-FreeRTOS/build" --target all 

[96/111] Building ASM object CMakeFiles/template.elf.dir/E_/Code_Warehouse/STM32/12-CMake-FreeRTOS/mcu/Libraries/CMSIS/startup_stm32f429xx.s.obj
E:/Code Warehouse/STM32/12-CMake-FreeRTOS/mcu/Libraries/CMSIS/startup_stm32f429xx.s: Assembler messages:
E:/Code Warehouse/STM32/12-CMake-FreeRTOS/mcu/Libraries/CMSIS/startup_stm32f429xx.s: Warning: end of file not at end of a line; newline inserted
[111/111] Linking C executable template.elf
 *  终端将被任务重用，按任意键关闭。
```

   此刻，build 文件夹的根目录中，出现 **template.bin** 、**template.elf** 、**template.hex** 三个文件

   >修改 CMakeLists.txt 中的 set(CMAKE_C_FLAGS "-mcpu=${MCU} -mthumb ${FPU_FLAGS} -O2 -ffunction-sections -fdata-sections -w -g") 禁用所有警告（只显示错误）

   >我喜欢清爽的终端，至于警告，能用就行(

   > :warning: 注意：build的过程很有可能产生大量报错，请保持耐心和冷静，逐个自行处理。其中有几个汇编文件的警告，本人目前无法处理。不过仍然可以成功生成固件。

3. 点击 **Flash** 按钮。
   一切正常的情况下，vscode的终端会显示如下内容：

```bash
正在执行任务: openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg -c 'program build/template.elf verify reset exit' 

Open On-Chip Debugger 0.12.0 (2025-07-10) [https://github.com/sysprogs/openocd]
Licensed under GNU GPL v2
libusb1 d52e355daa09f17ce64819122cb067b8a2ee0d4b
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Warn : DEPRECATED: auto-selecting transport "swd". Use 'transport select swd' to suppress this message.
Info : CMSIS-DAP: SWD supported
Info : CMSIS-DAP: JTAG supported
Info : CMSIS-DAP: Atomic commands supported
Info : CMSIS-DAP: Test domain timer supported
Info : CMSIS-DAP: FW Version = 2.0.0
Info : CMSIS-DAP: Interface Initialised (SWD)
Info : SWCLK/TCK = 0 SWDIO/TMS = 1 TDI = 1 TDO = 1 nTRST = 0 nRESET = 0
Info : CMSIS-DAP: Interface ready
Info : clock speed 2000 kHz
Info : SWD DPIDR 0x2ba01477
Info : [stm32f4x.cpu] Cortex-M4 r0p1 processor detected
Info : [stm32f4x.cpu] target has 6 breakpoints, 4 watchpoints
Info : [stm32f4x.cpu] Examination succeed
Info : [stm32f4x.cpu] starting gdb server on 3333
Info : Listening on port 3333 for gdb connections
[stm32f4x.cpu] halted due to debug-request, current mode: Thread 
xPSR: 0x01000000 pc: 0x08004b5c msp: 0x20030000
** Programming Started **
Info : device id = 0x20036419
Info : flash size = 1024 KiB
Info : Single Bank 1024 kiB STM32F42x/43x/469/479 found
** Programming Finished **
** Verify Started **
** Verified OK **
** Resetting Target **
shutdown command invoked
 *  终端将被任务重用，按任意键关闭。
```

**此时，程序已经成功烧录到芯片里。**
