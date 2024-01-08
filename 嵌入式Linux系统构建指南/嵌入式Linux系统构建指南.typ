#import "simplepaper.typ": *

#show: project.with(
  title: "嵌入式Linux构建指南",
  authors: (
    "尚硅谷",
  ),
)

= 环境准备

本次实验宿主机使用全新的Ubuntu20.04环境，并在完成实验后用户目录下有以下结构的文件夹。

首先需要创建架构。

```
v3s-workspace
- linux
- partitions
-- boot
--- boot.scr
--- sun8i-v3s-licheepi-zero-dock.dtb
--- zImage
-- root
- u-boot
- modules
- buildroot
- boot.cmd
```

然后下载所有本次实验会使用到的代码和依赖。

```sh
# 安装依赖
sudo apt-get install flex bison gcc make gcc-arm-linux-gnueabihf libncurses-dev swig python-dev device-tree-compiler python3-setuptools python3-dev libssl-dev u-boot-tools g++ patch

# 下载Mainline Linux,你可以在https://www.kernel.org寻找最新的LTS版本
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.19.tar.xz
# extract是zsh提供的自动解压指令，可以替换成tar的对应格式解压指令
extract linux-6.1.19.tar.xz
mv linux-6.1.19 linux

# 下载U-Boot，我们这里没有使用LTS版本，你可以进入cd进去后切换到LTS分支
git clone git://git.denx.de/u-boot.git

# 下载Buildroot，你可以在https://buildroot.org/downloads寻找最新的版本
wget https://buildroot.org/downloads/buildroot-2023.02.tar.xz
extract buildroot-2023.02.tar.xz
mv buildroot-2023.02 buildroot
```

每一章节结束后请返回```sh v3s-workspace```目录。

= 编译U-Boot

```sh
cd u-boot
# 使用荔枝派Nano的默认配置
make CROSS_COMPILE=arm-linux-gnueabihf- LicheePi_Zero_defconfig
# 编译
make CROSS_COMPILE=arm-linux-gnueabihf-
# 拷贝U-Boot镜像
cp u-boot-sunxi-with-spl.bin ../partitions
```

接下来我们需要准备U-Boot启动所需的配置文件，将以下内容写入boot.cmd。

```sh
setenv bootargs console=tty0 console=ttyS0,115200 panic=5 rootwait root=/dev/mmcblk0p2 rw
load mmc 0:1 0x43000000 sun8i-v3s-licheepi-zero-dock.dtb
load mmc 0:1 0x42000000 zImage
bootz 0x42000000 - 0x43000000
```

接着编译配置文件。

```sh
mkimage -C none -A arm -T script -d boot.cmd ./partitions/boot/boot.scr
```

= 编译Linux Kernel

```sh
cd linux
# 使用linux-sunxi项目的默认配置，该项目主要包含全志各芯片的硬件支持文档和手册
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- sunxi_defconfig
# 进入内核配置菜单
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
```

#highlight[在内核配置菜单中，将 `Networking support > Wireless` 中的选项全部选中。]

我们需要修改```sh arch/arm/boot/dts/sun8i-v3s-licheepi-zero.dts```以启用以太网和USB支持。

```
/*
 * Copyright (C) 2016 Icenowy Zheng <icenowy@aosc.xyz>
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 *  a) This file is free software; you can redistribute it and/or
 *     modify it under the terms of the GNU General Public License as
 *     published by the Free Software Foundation; either version 2 of the
 *     License, or (at your option) any later version.
 *
 *     This file is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 * Or, alternatively,
 *
 *  b) Permission is hereby granted, free of charge, to any person
 *     obtaining a copy of this software and associated documentation
 *     files (the "Software"), to deal in the Software without
 *     restriction, including without limitation the rights to use,
 *     copy, modify, merge, publish, distribute, sublicense, and/or
 *     sell copies of the Software, and to permit persons to whom the
 *     Software is furnished to do so, subject to the following
 *     conditions:
 *
 *     The above copyright notice and this permission notice shall be
 *     included in all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *     EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *     OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *     NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *     HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *     WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *     FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *     OTHER DEALINGS IN THE SOFTWARE.
 */

/dts-v1/;
#include "sun8i-v3s.dtsi"
#include "sunxi-common-regulators.dtsi"

/ {
	model = "Lichee Pi Zero";
	compatible = "licheepi,licheepi-zero", "allwinner,sun8i-v3s";

	aliases {
		serial0 = &uart0;
		ethernet0 = &emac; /* 添加这一行 */
	};

	chosen {
		stdout-path = "serial0:115200n8";
	};

	leds {
		compatible = "gpio-leds";

		blue_led {
			label = "licheepi:blue:usr";
			gpios = <&pio 6 1 GPIO_ACTIVE_LOW>; /* PG1 */
		};

		green_led {
			label = "licheepi:green:usr";
			gpios = <&pio 6 0 GPIO_ACTIVE_LOW>; /* PG0 */
			default-state = "on";
		};

		red_led {
			label = "licheepi:red:usr";
			gpios = <&pio 6 2 GPIO_ACTIVE_LOW>; /* PG2 */
		};
	};

  /* 添加以下soc部分 */
	soc {
		ehci0: usb@01c1a000 {
			compatible = "allwinner,sun8i-v3s-ehci", "generic-ehci";
			reg = <0x01c1a000 0x100>;
			interrupts = <GIC_SPI 72 IRQ_TYPE_LEVEL_HIGH>;
			clocks = <&ccu CLK_BUS_EHCI0>, <&ccu CLK_BUS_OHCI0>;
			resets = <&ccu RST_BUS_EHCI0>, <&ccu RST_BUS_OHCI0>;
			status = "okay";
		};

		ohci0: usb@01c1a400 {
			compatible = "allwinner,sun8i-v3s-ohci", "generic-ohci";
			reg = <0x01c1a400 0x100>;
			interrupts = <GIC_SPI 73 IRQ_TYPE_LEVEL_HIGH>;
			clocks = <&ccu CLK_BUS_EHCI0>, <&ccu CLK_BUS_OHCI0>,
			<&ccu CLK_USB_OHCI0>;
			resets = <&ccu RST_BUS_EHCI0>, <&ccu RST_BUS_OHCI0>;
			status = "okay";
		};
	};
};

&mmc0 {
	broken-cd;
	bus-width = <4>;
	vmmc-supply = <&reg_vcc3v3>;
	status = "okay";
};

&uart0 {
	pinctrl-0 = <&uart0_pb_pins>;
	pinctrl-names = "default";
	status = "okay";
};

&usb_otg {
	dr_mode = "host";
	status = "okay";
};

&usbphy {
	usb0_id_det-gpios = <&pio 5 6 GPIO_ACTIVE_LOW>;
	status = "okay";
};

/* 添加emac部分 */
&emac {
	allwinner,leds-active-low;
	status = "okay";
};
```

接着执行以下指令。

```sh
# 编译内核，-j4的4可以修改为你的CPU核心数
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -j4 zImage
# 编译DTB文件，本文件用于Kernel识别外设，是 Mainline Kernel不可缺少的部分，-j4的4可以修改为你的CPU核心数
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -j4 dtbs
# 编译Modules，-j4的4可以修改为你的CPU核心数
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -j4 modules
# 安装模块
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=../modules  make modules modules_install
# 拷贝生成的zImage内核镜像和DTB文件
cp arch/arm/boot/zImage ../partitions/boot
cp arch/arm/boot/dts/sun8i-v3s-licheepi-zero-dock.dtb ../partitions/boot
```

= 编译Buildroot

我们使用Buildroot默认的BusyBox程序和GLIBC，如果需要剪裁大小，可以选择其他的C支持库。

```sh
cd buildroot
# 进入配置菜单
make menuconfig
```

#figure(
  table(
    columns: (1fr, 1fr, 1fr),
    inset: 10pt,
    align: horizon,
    [配置位置], [操作], [用途],
    "Target options", "Target Arch设置为ARM (little endian)", "设置大小端",
    "Target options", "Target Arch Variant设置为Cortex-A7", "设置CPU架构",
    "Toolchain", "Kernel Headers设置为你下载的LTS版本内核对应的版本号", "匹配内核版本",
    "Target packages > Networking applications", "hostapd", "开启",
    "Target packages > Networking applications", "hostapd > Enable hostap driver", "开启",
    "Target packages > Networking applications", "hostapd > Enable nl80211 driver", "开启",
    "Target packages > Networking applications", "hostapd > Enable ACS", "开启",
    "Target packages > Networking applications", "hostapd > Enable EAP", "开启",
    "Target packages > Networking applications", "hostapd > Enable WPS", "开启",
  ),
  caption: "Buildroot相关配置"
)

#figure(
  table(
    columns: (1fr, 1fr, 1fr),
    inset: 10pt,
    align: horizon,
    [配置位置], [操作], [用途],
    "Target packages > Networking applications", "openssh", "开启",
    "Target packages > Networking applications", "openssh > client", "开启",
    "Target packages > Networking applications", "openssh > server", "开启",
    "Target packages > Networking applications", "openssh > key utilities", "开启",
    "Target packages > Networking applications", "openssh > use sandboxing", "开启",
    "Target packages > Networking applications", "wireless tools", "开启",
    "Target packages > Networking applications", "wireless tools > Install shared library", "开启",
    "Target packages > Networking applications", "wpa_supplicant", "开启",
    "Target packages > Libraries > Crypto", "openssl support", "openssl support的子菜单部件全部选中",
  ),
  caption: "Buildroot相关配置"
)

同时修改```sh partitions/root/etc/fstab```文件中的ext2为ext4。

= 写入SD卡

我的SD卡路径是```sh /dev/sdb```，可以通过```sh sudo fdisk -l```查看SD卡的路径。

```sh
cd partitions
# 清空分区表
sudo dd if=/dev/zero of=/dev/sdb bs=1M count=1
# 写入U-Boot
sudo dd if=u-boot-sunxi-with-spl.bin of=/dev/sdb bs=1024 seek=8
```

```sh
# 写入分区表，请复制除了本行内的内容并执行
sudo blockdev --rereadpt /dev/sdb
cat <<EOT | sudo sfdisk /dev/sdb
1M,16M,c
,,L
EOT
```

```sh
# 格式化
sudo mkfs.vfat /dev/sdb1
sudo mkfs.ext4 /dev/sdb2
# 拷贝boot进入第一个分区
sudo mount /dev/sdb1 /mnt
sudo cp -R boot/* /mnt
sync
sudo umount /mnt
# 拷贝rootfs进入第二个分区
sudo mount /dev/sdb2 /mnt
sudo cp -R root/* /mnt
sync
sudo umount /mnt
```

= 将嵌入式Linux作为ssh服务器使用

连接串口，波特率设置为 115200 。

使用 putty 软件连接嵌入式Linux开发板。

```sh
vi /etc/ssh/sshd_config
# 将文件中的PermitRootLogin设置为yes
# PermitRootLogin yes
# 保存退出

# 使用passwd设置密码
passwd
# 启动sshd服务
/usr/sbin/sshd
```

#highlight[注：这里建议重启一下板子，否则 /etc/ssh/sshd_config 不一定能够生效，我实测时就是需要重启才可以]

```sh
# 启动以太网服务
ifconfig eth0 up
udhcpc
```

= 安装无线网卡驱动

```sh
cd v3s-workspace
git clone https://github.com/al177/esp8089.git
cd esp8089
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -C ../linux/ M=${PWD} modules

# 172.17.0.58替换为你的电脑看到的Linux开发板的ip地址。
scp esp8089.ko root@172.17.0.58:/root/
ssh root@172.17.0.58
# 进入嵌入式Linux开发板之后的操作
# 加载wifi驱动
atguigu-pi>insmod esp8089.ko
atguigu-pi>ifconfig wlan0 up
atguigu-pi>wpa_passphrase your_SSID your_passphrase > your_SSID.conf
atguigu-pi>wpa_supplicant -B -i wlan0 -c your_SSID.conf
```