#Project 2
###17级 杜耀
---
##Project要求
- 建立两种不同的网络拓扑结构，至少使用两种信道（以third.cc为例，节点数变化不算）
- 完成4个client和1个server相互之间的通信，在终端中显示
- 设置STA的移动轨迹为线性远离AP,并尝试其他运动模型
- 记录数据，将相关四个文件Push到github

##拓扑构建
---
###拓扑一
```C++
                                    |
         Section 1                  |                     Section 2
 -----------------------------------|--------------------------------------
     Wifi 10.1.2.0
                      AP
  *      *      *     *
  |      |      |     |          10.1.1.0
 n6      n7     n8    n0 ------------------------ n1     n2     n3     n4     n5
                              point-to-point      |      |      |      |      |
                                                  *      *      *      *      *
                                                  AP
                                                          Wifi 10.1.3.0
```
- echo server安装于Section 1最左端的10.1.2.3 端口号为6
- 客户端安装于Section 2的所有wifi STA 上
###运行截图
![Image of pro_2.1 running result]
(http://ww3.sinaimg.cn/large/005unglEgw1f78fto2ghej30lm0bxtcp.jpg)
- 结合拓扑，由上图可知，Section 2的所有客户端皆可正常与Section 1最左侧服务器通信
###移动模型代码
```C++
//配置移动模型，起始位置
  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (25.0),
                                 "DeltaY", DoubleValue (25.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));
//配置STA移动方式，ConstantVelocityMobilityModel，匀速运动模型（两区STA反向运动）
   mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");

   mobility.Install (wifiStaNodes_1);
   for (uint n = 0; n < wifiStaNodes_1.GetN(); n++)
   {
   Ptr<ConstantVelocityMobilityModel> mob = wifiStaNodes_1.Get(n)->GetObject<ConstantVelocityMobilityModel>();
   mob->SetPosition(Vector(-5.0, -5.0, 0.0));
   mob->SetVelocity(Vector(-(n/2.0), 0.0, 0.0));
   }

   mobility.Install (wifiStaNodes_2);
   for (uint n = 0; n < wifiStaNodes_2.GetN(); n++)
   {
   Ptr<ConstantVelocityMobilityModel> mob = wifiStaNodes_2.Get(n)->GetObject<ConstantVelocityMobilityModel>();
   mob->SetPosition(Vector(5.0, 5.0, 0.0));
   mob->SetVelocity(Vector(+(n/2.0), 0.0, 0.0));
   }

//配置AP移动方式，ConstantPositionMobilityModel，固定位置模型
   mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
   mobility.Install (wifiApNode_1);
   mobility.Install (wifiApNode_2);
```
---
###拓扑二
```C++
                     |                                  |
         Section 1   |                                  |      Section 2
 --------------------|----------------------------------|--------------------
     Wifi 10.1.2.0                                           Wifi 10.1.3.0
                       AP                            AP
  *      *      *      *                             *      *      *      *
  |      |      |      |          10.1.1.0           |      |      |      |
 n6      n7     n8     n0 ========== n1 =========== n2     n3     n4     n5
                                    LAN
```
- echo server安装于Section 1最左端的10.1.2.3 端口号为6
- 客户端安装于Section 2的所有wifi STA 上以及LAN中的设备n1上
###运行截图
![Image of pro_2.2 running result]
(http://ww4.sinaimg.cn/large/005unglEgw1f78ooqroocj30lo0dc0w7.jpg)
- 结合拓扑，由上图可知，Section 2的所有客户端及LAN中的设备，皆可正常与Section 1最左侧服务器通信
###移动模型代码
```C++
//配置STA移动方式，两区STA设置为两种随机运动模型：RandomWalk2dMobilityModel、RandomDirection2dMobilityModel
    mobility.SetMobilityModel (
    "ns3::RandomWalk2dMobilityModel",
    "Bounds", RectangleValue (Rectangle (-100, 100, -100, 100))
    );
    mobility.Install (wifiStaNodes_1);

    mobility.SetMobilityModel (
     "ns3::RandomDirection2dMobilityModel",
    "Bounds", RectangleValue (Rectangle (-100, 100, -100, 100)),
    "Speed", StringValue ("ns3::UniformRandomVariable[Min=1.0|Max=2.0]"),
    "Pause", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]")
    );
    mobility.Install (wifiStaNodes_2);

//配置csmanodes(包括了APnodes)移动方式，ConstantPositionMobilityModel，固定位置模型
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (csmaNodes);
```
##总结
- 本次project着重于练习拓扑构建的步骤：
创建节点-创建信道，设置信道参数，并将设备安装到节点上-配置协议栈-分配IP地址-安装应用程序-设置仿真时间及启动时间
- 针对 wifista无线终端，在设定AP后，需使用YansWifiChannelHelper，使所有物理层对象使用相同的底层信道-无线信道
- 通过查阅ns-3.25自带的mobility model文件(ns-3.25/src/mobility/model),学习运动模型的使用
- 学习了ns-3.25中Helper类的使用
