---
layout: posts
title: 用SuperSU替换KingRoot
createTime: 2017-02-03 12:35:31
---

最近在调教一部Android 5.1设备。这台机器没有大厂血统，获得root权限比较麻烦。内置的recovery太垃圾，没法卡刷。尝试多种root工具后，选择了国产大流氓——KingRoot。

不得不说，流氓活就是好，没几分钟就root好了。然而这垃圾玩意特卡、自动升级、弹窗……总之流氓会的它都会了。尝试用SuperSU替换之，可流氓就是流氓，它对SuperSU等其它权限管理软件做了限制，没法替换，连su文件都是自己定制的……把KingRoot外壳删掉后无法启动……

多方搜索后，总结出如下解决方案：

1.  用新版（旧版在Android 5.1上无法工作）KingRoot取得root权限，重启。
2.  安装BusyBox、Xposed等常用xx工具（反正以后搞机也得用），安装“核心破解”（需xposed），重启后打开“核心破解”的“允许降级安装”功能。
3.  **断网**，安装KingRoot **4.6**版。这个版本可接管新版，然后被另一个软件干掉。
4.  **保持断网**，安装 SuperSU Me Pro **9.1.8** (收费，当然也可以用破解版)，启动，按提示走就行。
5.  **连网**，打开SuperSU，更新su二进制文件。
6.  重启设备，测试root权限是否可用。
7.  Have Fun!

我的测试截图如下：

![Kill KingRoot](https://images2015.cnblogs.com/blog/585442/201702/585442-20170203125033636-2023807362.png "Rpls KingRoot with SuperSU")

![Test Permission](https://images2015.cnblogs.com/blog/585442/201702/585442-20170203125043729-1710106122.png "Test Permission")

![Superuser](https://images2015.cnblogs.com/blog/585442/201702/585442-20170203125048011-2049310180.png "Superuser")
