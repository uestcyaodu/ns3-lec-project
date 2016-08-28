#Project 3
###17级 杜耀
---
##Project要求
以各自的Project-2.cc为示例，并在新的要求下完成任务
- 使用Logging系统记录错误信息，学会如何进行调试
- 使用Tracing系统，针对至少两种信道，输出Ascii与Pcap两种文件
- 自行安装PyViz，使用PyViz查看Project-2仿真过程
- 自行安装Wireshark，熟悉学会使用TcpDump和Wireshark查看文件

###Logging系统
![Image of pro_2.1 logging]
(http://ww2.sinaimg.cn/large/005unglEgw1f79i19erv0j30kw0sdtfv.jpg)

```C++
if (verbose)
  {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_FUNCTION);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_FUNCTION);	//启动记录组件
  }
```
- 修改pro_2.1 更改LOG_LEVEL_INFO为LOG_LEVEL_FUNCTION,记录更加详细的参数
- NS_LOG_FUNCTION——Log a message describing each function called. 记录描述每个调用函数信息

###Tracing系统
![Image of ‘.tr’ ASCII file]
(http://ww3.sinaimg.cn/large/005unglEgw1f79i78z5bvj31250m7gwi.jpg)

```C++
if (tracing == true)
  {
    AsciiTraceHelper ascii;
    phy_1.EnableAsciiAll(ascii.CreateFileStream("pro_3_wifi_1.tr"));
    phy_1.EnablePcapAll ("pro_3_wifi_1");
    phy_2.EnableAsciiAll(ascii.CreateFileStream("pro_3_wifi_2.tr"));
    phy_2.EnablePcapAll ("pro_3_wifi_2");
    csma.EnableAsciiAll(ascii.CreateFileStream("pro_3_csma.tr"));
    csma.EnablePcapAll ("pro_3_csma");
  }
```
- 使用AsciiTraceHelper类，结合ascii.CreateFileStream,生成tracing文件
- 打开csma.tr文件后，即可查看完整的ASCII格式的链路信息
- 生成共22个数据文件（包括 .tr 文件 和 .pcap 文件）已整合在 “Project3_data.zip”中

###PyViz动态展示
- pro_2.1.cc：
![Image of pro_2.1 PyViz]
(http://ww3.sinaimg.cn/large/005unglEgw1f79i18mpqqj30o20iqq6e.jpg)

- pro_2.2.cc：
![Image of pro_2.2 PyViz]
(http://ww4.sinaimg.cn/large/005unglEgw1f79i180mq5j30o90iqgq8.jpg)

###Wireshark抓包
![Image of Wireshark]
(http://ww3.sinaimg.cn/large/005unglEgw1f79i17ftrij30qu0olwhy.jpg)
- 如图清楚地展示了pro_2.1.cc仿真运行中UDP数据包的流向，通过wireshark可抓包查看数据包的内容、起始地址、目的地址、长度等信息

###安装代码：
- PyViz: sudo apt-get install python-dev python-pygraphviz python-kiwi python-pygoocanvas python-gnome2 python-gnomedesktop python-rsvg
- Wireshark:sudo apt-get install wireshark (手动安装方法详见：http://blog.csdn.net/bo5509/article/details/8189757)

###总结
- 本次Project着重训练了监测仿真过程及处理仿真数据的技术
- 多角度观测仿真过程，利于进一步修改仿真代码
