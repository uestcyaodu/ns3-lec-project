/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"

// Network Topology
// Number of section 1 or section 2 's wifi nodes can be increased up to 250
//                                                               |
//         Section 1                                             |                     Section 2
// --------------------------------------------------------------|----------------------------------------------------------
//             Wifi 10.1.2.0
//                                                    AP
//   *      *      *        *        *       *        *
//  |      |      |        |        |       |        |          10.1.1.0
//  n8      n9    n10      n11      n12     n13       n0 ------------------------ n1     n2     n3     n4     n5     n6     n7
//                                                           point-to-point      |      |      |      |      |      |      |
//                                                                               *      *      *      *      *      *      *
//                                                                              AP
//                                                                                             Wifi 10.1.3.0
//搭建拓扑：两个wifi网络，各有6个sta，1个ap；实现两个wifi网络之间的通信
//echo_server在Section 1的10.1.2.3 端口号为8 ; 客户端安装于Section 2的所有wifi_STA上
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");		//定义记录组件

int
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nWifi_1 = 6;			//wifi 区1节点数量
  uint32_t nWifi_2 = 6;				//wifi 区2节点数量
   bool tracing = true;


  CommandLine cmd;
  cmd.AddValue ("nWifi_1", "Number of section 1 's wifi STA devices", nWifi_1);
  cmd.AddValue ("nWifi_2", "Number of section 2 's wifi STA devices", nWifi_2);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  if (nWifi_1 > 250 || nWifi_2 > 250)
    {
      std::cout << "Too many wifi nodes, no more than 250 in each section." << std::endl;
      return 1;
    }

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_FUNCTION);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_FUNCTION);	//启动记录组件
    }

// P2P:
  //创建2个节点，p2p链路两端
  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  //创建信道，设置信道参数，将设备安装到节点上
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

// section 1 wifi
//创建wifista无线终端，AP接入点
  NodeContainer wifiStaNodes_1;
  wifiStaNodes_1.Create (nWifi_1);
  NodeContainer wifiApNode_1 = p2pNodes.Get (0);

  //创建无线设备于无线节点之间的互联通道，并将通道对象与物理层对象关联
  //确保所有物理层对象使用相同的底层信道，即无线信道
  YansWifiChannelHelper channel_1 = YansWifiChannelHelper::Default ();
  channel_1.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");//传播模型为：ConstantSpeedPropagationDelayModel
  YansWifiPhyHelper phy_1 = YansWifiPhyHelper::Default ();
  phy_1.SetChannel (channel_1.Create ());

  //配置速率控制算法，AARF算法
  WifiHelper wifi_1 = WifiHelper::Default ();
  wifi_1.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac_1 = NqosWifiMacHelper::Default ();//Mac类型为：NqosWifiMacHelper（没有QoS保障的Mac层机制）

  //配置mac类型为sta模式，不发送探测请求
  Ssid ssid_1 = Ssid ("ns-3-ssid-section-1");
  mac_1.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid_1),
               "ActiveProbing", BooleanValue (false));

  //创建无线设备，将mac层和phy层安装到设备上
  NetDeviceContainer staDevices_1;
  staDevices_1 = wifi_1.Install (phy_1, mac_1, wifiStaNodes_1);

  //配置AP节点的mac层为AP模式，创建AP设备
  mac_1.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid_1));

  NetDeviceContainer apDevices_1;
  apDevices_1 = wifi_1.Install (phy_1, mac_1, wifiApNode_1);

// section 2 wifi
//创建wifista无线终端，AP接入点
  NodeContainer wifiStaNodes_2;
  wifiStaNodes_2.Create (nWifi_2);
  NodeContainer wifiApNode_2 = p2pNodes.Get (1);

  //创建无线设备于无线节点之间的互联通道，并将通道对象与物理层对象关联
  //确保所有物理层对象使用相同的底层信道，即无线信道
  YansWifiChannelHelper channel_2 = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy_2 = YansWifiPhyHelper::Default ();
  phy_2.SetChannel (channel_2.Create ());

  //配置速率控制算法，AARF算法
  WifiHelper wifi_2 = WifiHelper::Default ();
  wifi_2.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac_2 = NqosWifiMacHelper::Default ();

  //配置mac类型为sta模式，不发送探测请求
  Ssid ssid_2 = Ssid ("ns-3-ssid-section-2");
  mac_2.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid_2),
               "ActiveProbing", BooleanValue (false));

  //创建无线设备，将mac层和phy层安装到设备上
  NetDeviceContainer staDevices_2;
  staDevices_2 = wifi_2.Install (phy_2, mac_2, wifiStaNodes_2);

  //配置AP节点的mac层为AP模式，创建AP设备
  mac_2.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid_2));

  NetDeviceContainer apDevices_2;
  apDevices_2 = wifi_2.Install (phy_2, mac_2, wifiApNode_2);


  //配置移动模型;
  MobilityHelper mobility;

  //配置STA移动方式，RandomWalk2dMobilityModel
   mobility.SetMobilityModel (
   "ns3::RandomWalk2dMobilityModel",
   "Bounds", RectangleValue (Rectangle (-250, 250, -250, 250))//场景范围：500m*500m
   );

   mobility.Install (wifiStaNodes_1);
   Ptr<RandomWalk2dMobilityModel> mob_8 = wifiStaNodes_1.Get(0)->GetObject<RandomWalk2dMobilityModel>();
   mob_8->SetPosition(Vector(-50.0, 0.0, 0.0));
   Ptr<RandomWalk2dMobilityModel> mob_9 = wifiStaNodes_1.Get(1)->GetObject<RandomWalk2dMobilityModel>();
   mob_9->SetPosition(Vector(-25.0, 43.3, 0.0));
   Ptr<RandomWalk2dMobilityModel> mob_10 = wifiStaNodes_1.Get(2)->GetObject<RandomWalk2dMobilityModel>();
   mob_10->SetPosition(Vector(25.0, 43.3, 0.0));
   Ptr<RandomWalk2dMobilityModel> mob_11 = wifiStaNodes_1.Get(3)->GetObject<RandomWalk2dMobilityModel>();
   mob_11->SetPosition(Vector(50.0, 0.0, 0.0));
   Ptr<RandomWalk2dMobilityModel> mob_12 = wifiStaNodes_1.Get(4)->GetObject<RandomWalk2dMobilityModel>();
   mob_12->SetPosition(Vector(25.0, -43.3, 0.0));
   Ptr<RandomWalk2dMobilityModel> mob_13 = wifiStaNodes_1.Get(5)->GetObject<RandomWalk2dMobilityModel>();
   mob_13->SetPosition(Vector(-25.0,-43.3, 0.0));

   mobility.Install (wifiStaNodes_2);
   Ptr<RandomWalk2dMobilityModel> mob_2 = wifiStaNodes_2.Get(0)->GetObject<RandomWalk2dMobilityModel>();
   mob_2->SetPosition(Vector(75.0, 0.0, 0.0));
   Ptr<RandomWalk2dMobilityModel> mob_3 = wifiStaNodes_2.Get(1)->GetObject<RandomWalk2dMobilityModel>();
   mob_3->SetPosition(Vector(100.0, 43.3, 0.0));
   Ptr<RandomWalk2dMobilityModel> mob_4 = wifiStaNodes_2.Get(2)->GetObject<RandomWalk2dMobilityModel>();
   mob_4->SetPosition(Vector(150.0, 43.3, 0.0));
   Ptr<RandomWalk2dMobilityModel> mob_5 = wifiStaNodes_2.Get(3)->GetObject<RandomWalk2dMobilityModel>();
   mob_5->SetPosition(Vector(175.0, 0.0, 0.0));
   Ptr<RandomWalk2dMobilityModel> mob_6 = wifiStaNodes_2.Get(4)->GetObject<RandomWalk2dMobilityModel>();
   mob_6->SetPosition(Vector(150.0, -43.3, 0.0));
   Ptr<RandomWalk2dMobilityModel> mob_7 = wifiStaNodes_2.Get(5)->GetObject<RandomWalk2dMobilityModel>();
   mob_7->SetPosition(Vector(100,-43.3, 0.0));

  //配置AP移动方式，ConstantPositionMobilityModel，固定位置模型
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode_1);
  mobility.Install (wifiApNode_2);
  Ptr<ConstantPositionMobilityModel> mob_ap_1 = wifiApNode_1.Get(0)->GetObject<ConstantPositionMobilityModel>();
  mob_ap_1->SetPosition(Vector(0.0,0.0,0.0));
  Ptr<ConstantPositionMobilityModel> mob_ap_2 = wifiApNode_2.Get(0)->GetObject<ConstantPositionMobilityModel>();
  mob_ap_2->SetPosition(Vector(125.0,0.0,0.0));
  //已经创建了节点，设备，信道和移动模型，接下来配置协议栈
  InternetStackHelper stack;
  stack.Install (wifiApNode_1);
  stack.Install (wifiStaNodes_1);
  stack.Install (wifiApNode_2);
  stack.Install (wifiStaNodes_2);

  //分配IP地址
  Ipv4AddressHelper address;
 //P2P信道
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);
  //wifi信道
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer wifi_1Interfaces;
  wifi_1Interfaces = address.Assign (staDevices_1);
  address.Assign (apDevices_1);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer wifi_2Interfaces;
  wifi_2Interfaces = address.Assign (staDevices_2);
  address.Assign (apDevices_2);

  //放置echo服务端程序在Section 1 最右边的wifi节点,端口为8
  UdpEchoServerHelper echoServer (8);

  ApplicationContainer serverApps = echoServer.Install (wifiStaNodes_1.Get (nWifi_1 - 1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  //回显客户端放在最后的STA节点，指向Wifi网络服务器上面的节点地址，端口为8
  UdpEchoClientHelper echoClient (wifi_1Interfaces.GetAddress (nWifi_1 - 1), 8);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (512));//packet size为512byte，包间隔为100ms

  //安装其他节点应用程序
  for(int i = 0; i < (int)nWifi_2; i++){
  ApplicationContainer clientApps_1 = echoClient.Install (wifiStaNodes_2.Get (nWifi_2 - 1 - i));
  clientApps_1.Start (Seconds (2.0+i));
  clientApps_1.Stop (Seconds (10.0));
  }

  //启动互联网络路由
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));

  if (tracing == true)
    {
      AsciiTraceHelper ascii;
      pointToPoint.EnablePcapAll ("pro_4");
      phy_1.EnablePcap ("pro_4", apDevices_1.Get (0));
      phy_1.EnableAsciiAll(ascii.CreateFileStream("pro_4_wifi_1.tr"));
      phy_2.EnablePcap ("pro_4", apDevices_2.Get (0));
      phy_2.EnableAsciiAll(ascii.CreateFileStream("pro_4_wifi_2.tr"));
    }

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
