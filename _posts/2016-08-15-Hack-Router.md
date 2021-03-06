---
layout: posts
title: 路由器TL-WR941N V5.1：救砖、MOD
createTime: 2016-08-15
---
**为了缩短篇幅，我删除了绝大部分错误尝试的细节，仅粗略记录过程。救砖和MOD过程中的重点会在后半篇列出，以供MODer参阅。**


### 救砖

#### 起因

我早在2011年就开始折腾路由器了。当时WiFi盛行，家里也普及了各种无线设备，就随便买了台普联（TP-LINK）的WR740N组建起简易的家庭局域网。可是由于种种原因，这台路由器最终被我搞死了（乱折腾），连内存焊盘都给拖掉一个（烙铁太粗了，功率也不小）。

家里没有WiFi可不行！于是就有了继任者——普联的WR941N V5.1。这台路由的做工和硬件明显比上一台要好许多，稳定且信号好，常年缩在角落里不间断工作，几乎没有出过毛病。

直到去年，我在尝试组建WDS桥接网络时，脑抽从OpenWrt的LuCI里直接刷入了原厂固件，它才罢了工，无限重启，变砖了。

#### 修复Attempt1

用针猛捅Reset键，手都疼了，无效。

#### 修复Attempt2

在某宝斥资20人民币，购入了两根USB转TTL线（一根备用）。接着在网上查找TTL接头的位置，从废主板上拆下跳线针焊在路由器的TTL触点上（这是去年的事了，照片没有存档）。

插上网线，手动设置IP地址，连好TTL+-和GND，设置波特率115200，键映射Home->'TPL'，通电，狂按Home键……我擦怎么不中断？导演，剧本上不是这样写的啊！

网上的教程说快速按TPL即可中断U-Boot的引导，从而进入TTL的控制台，接着用TFTP刷机就行了。可我碰上了这个倒霉路由器，TTL的中断应该是被去除了，也就是无法中断启动。TTL修砖失败（现在来看应该是TTL修改不到位，导致无法输入字符）。

#### 修复Attempt3

今年暑假闲着无聊，又翻出来了那个坏路由器，决定修好砖后顺便改造一下，在路由器上接个U盘跑个Nginx和FTP之类的服务。查阅前人分享的经验，得知在新flash上刷入不死引导器（Bootloader，U-Boot为引导器的一种），再通过引导器刷写固件和art（与路由器的无线模块有关，没有art将无法使用无线功能）即可。
我选用了[hackpascal](http://blog.hackpascal.net/)的Breed（他本人编写的引导器，与U-Boot作用相同，但功能更加强大）。接着在某宝斥资20人民币，购入了一个垃圾编程器、一个Winbond的16M flash、两个USB插座和一些杜邦线。

趁着快递没到，用坏电脑主板复习一下烙铁（4年没动烙铁了，*洪荒之力*少了一大半），然后把旧的flash从路由器上拆下：

可以用隔热胶带贴上周围的小阻容来防止“误伤”。我手边没有，只好用电工胶布代替：

![拆芯片时需要贴上胶带，防止误伤](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815232449921-1094396177.jpg)

![堆锡拆除旧的flash芯片](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815232522078-1588389242.jpg)

快递在下单两天后到达：

![购入物品](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815232602562-1865624949.jpg)

把新flash焊在转换板上，然后把那8根针也焊在转换板上（焊接是为了保证接触良好），装好驱动开刷。

没刷上，软件提示我没焊好……估计是假焊。又热了下烙铁，重焊了所有触点，这下可以了。这个SPI flash的写入速度奇慢，再配上这个垃圾编程器，速度绝了：

![编程器刷入Breed](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815232648218-1177029296.png)

插上网线，让本地连接自动获取IP地址和DNS服务器地址（千万不要手动配置IP地址到不同网段，那样做会导致上传奇慢，且不稳定），子网掩码不动，通电开机，ping通后进Web后台，刷入OpenWrt和提取的原机art（art是我手动从编程器固件里提取的，把最后64KiB的数据导出为新文件即可，不同型号不通用）：

![Web刷机-上传固件](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815232824937-1902934864.png)

几秒就上传完毕，刷写倒是比较慢，需要一分钟左右：

![Web刷机-刷写固件](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815232831671-1506396486.png)

趁它刷写固件，来准备接下来要用到的材料：

![准备线材](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815232914750-263690346.jpg)

刷完后重启，竟然无法进入OpenWrt（现在仍不清楚原因，只知道是固件有问题），仍然停留在Breed。尝试恢复出厂设置、刷原厂（手动执行autoboot可进）（这里已查明是TTL的问题，见后文）、刷DD-Wrt都不行后，觉得是固件大小不对，得自己编译OpenWrt，不然就是个半砖。

##### 编译个性化的OpenWrt（放弃）

下载部分文件需要梯子。我的梯子不快，就找出了我的Openshift账号，用SSH登录后台，搞个screen（防止连接被挂断导致前功尽弃），执行如下命令（懒得解释了，想了解的话自己找教程）：

	git clone git://git.OpenWrt.org/15.05/OpenWrt.git
	cd OpenWrt
	./scripts/feeds update -a
	./scripts/feeds install -a
	make defconfig
	make menuconfig #这时需要自己手动配置一下
	make download V=s
	make V=99

*在最终编译前需要调整固件的大小*，用find找到关于wr941n的配置文件，把4M改成16M即可。
官方源估计挂掉了，需要多次尝试后才会自动切换到别的源。不管了，直接关闭PuTTY窗口即可，GNU screen会保证进程仍在后台运行。嗯，现在可以去吃顿饭、喝饮料、看奥运、玩Minecraft……随你便。

![编译1](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815233019890-375109718.png)

![编译2](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815233025125-1903905088.png)

编译后，用Breed刷入，仍然无法启动（在这里折腾了近一天）。

因为耗时太多，最终决定放弃。在某网盘上搜索得到*一份TL-WR941N-OpenWrt-BB-16M固件*，刷入后成功启动。

### [////MOD////]相关改造及问题

#### TTL干扰导致无法引导

这个问题比较隐蔽，耗费了我大半天的时间来排查。

原因：

1.在引导前，如果在TTL中按下任意键，Breed会终止引导，并进入Web刷机模式。
2.我的TTL针是去年加上的，改造后路由器的电路板一直在墙角吃灰，导致TTL输入有干扰，表现为在TTL终端会无故输入乱码。

解决方法：

用杜邦线将路由器的TTL接收端接地即可，路由器会认为输入一直为低电平，即无输入。这是扣上外壳前的最后一张照片：

![完成效果图](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815232353968-878039038.jpg)

#### 配置USB支持

供电部分推荐使用3R33等成熟的DC-DC方案，性价比高（3R33一片仅仅几毛钱）、效率高。7805模块则是一个更廉价的方案，缺点是发热大（在后面的测试中，挂载一个小U盘时并不热）。由于我手边有一个拆机的带散热片的7805模块，就不额外花钱了，废物利用即可。7805输入电压要求不大于36V，输出为5V，正好符合我的要求（TP路由输入9V）。

![7805稳压模块](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815232932921-1158115013.jpg)

自己查阅AR7240的PDF来找到USB的接口：在R278和R277靠近CPU的那脚引出USB的D+（R278）、D-（R277），并在这两个空位（这两个位置出厂时并没有电阻）加上[////15K\Omega////]的下拉电阻（对于标注为153的贴片电阻，其阻值为[////15\times10^3\Omega////]，即[////15K\Omega////]）。它们将被用于USB Host（在这里是我的路由器）检测是否插入了设备，以及设备的USB协议版本。

本来我购入了两个USB插座（一个备用），但在除错过程中以为是USB插座部分接触不良，所以干脆把导线直接焊在U盘的触点上，反正这些东西都得放到壳子里，也看不见。

硬件部分处理完毕后，通电开机，连上网线，一会儿就自动获取到了IP地址。登录LuCI，从内核日志里可以看到加载了USB设备，为sda1：

![USB系统日志](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815233157468-1550480082.png)

从编译信息里得知这个固件是hackpascal编译的，功能比较丰富，直接在 *系统->挂载点* 里就可以挂载了，对我来说等效于`mount -t ext4 /dev/sda1 /mnt/u`。

**我在一开始遇到过系统日志里出现`hub 1-0:1.0: Cannot enable port 1. Maybe the USB cable is bad?`的错误，反复调整接线、下拉电阻都不行。最后才发现，原因是D+、D-接反了……上文所述为正确接法。**

#### 开启FTP、SSH、HTTP、SMB、DDNS服务

其实在编译OpenWrt时，可以在`make menuconfig`里选中需要的功能，我下载的固件已经有这些东西了，就不必再费时间编译，简单配置后直接使用即可。

### 后话

救砖、MOD和配置软件花去了我三天的时间，可见改路由器需要很大的耐心和充裕的时间（胡说什么赶快补作业去马上要开学了）。有些路由器的方案并不流行，可能在网上找不到编译好的固件，这时候就要自己动手丰衣足食了。奉劝没有入坑的新人一句，MOD路由器是一个很大的坑，掉进来后需要很长时间才能爬出去， happy hacking~

![OpenWrtBB管理页面](http://images2015.cnblogs.com/blog/585442/201608/585442-20160815233235093-404924878.png)

### 入坑推荐阅读

[OpenWrt官方网站](https://OpenWrt.org)

[OpenWrt中文网](http://OpenWrt.org.cn)

[恩山无线论坛](http://www.right.com.cn/forum/forum.php)

