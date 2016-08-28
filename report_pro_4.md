#Project 4
###17级 杜耀
---
##Project要求
- 搭建拓扑：两个wifi网络，各有6个sta，1个ap；实现两个wifi网络之间的通信
- 场景范围：500m*500m
- 传播模型为：ConstantSpeedPropagationDelayModel
- Mac类型为：NqosWifiMacHelper（没有QoS保障的Mac层机制）
- 移动模型为随机游走模型，设定初始位置为两个六边形，边长10m，sta在6个顶点上，ap在六边形中间；两个六边形中心相距50m
- packet size为512byte，包间隔为100ms
- 使用PyViz查看仿真并截图


###拓扑构建
---
```C++
                                                               |
         Section 1                                             |                     Section 2
 --------------------------------------------------------------|----------------------------------------------------------
             Wifi 10.1.2.0
                                                    AP
  *      *       *        *        *       *        *
  |      |       |        |        |       |        |          10.1.1.0
  n8      n9    n10      n11      n12     n13       n0 ------------------------ n1     n2     n3     n4     n5     n6     n7
                                                            point-to-point      |      |      |      |      |      |      |
                                                                                *      *      *      *      *      *      *
                                                                                AP
                                                                                             Wifi 10.1.3.0
```
- 搭建拓扑：两个wifi网络，各有6个sta，1个ap；实现两个wifi网络之间的通信
- echo_server在Section 1的10.1.2.3 端口号为8 ; 客户端安装于Section 2的所有wifi_STA上

###PyViz动态展示

- 起始位置：
![Image of pro_4 PyViz_1]
(http://ww3.sinaimg.cn/mw690/005unglEgw1f79uzxwzzvj30jh0g9aaj.jpg)

- 仿真过程：
![Image of pro_4 PyViz_2]
(http://ww2.sinaimg.cn/large/005unglEgw1f79uzxzuztj30jj0g8gnf.jpg)
