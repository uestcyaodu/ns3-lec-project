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
//                                    |
//         Section 1                  |                     Section 2
// -----------------------------------|--------------------------------------
//     Wifi 10.1.2.0
//                      AP
//  *      *      *     *
//  |      |      |     |          10.1.1.0
// n6      n7     n8    n0 ------------------------ n1     n2     n3     n4     n5
//                              point-to-point      |      |      |      |      |
//                                                  *      *      *      *      *
//                                                  AP
//                                                          Wifi 10.1.3.0
//
//echo_server在Section 1的10.1.2.3 端口号为6 ; 客户端安装于Section 2的所有wifi_STA上
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");		//定义记录组件

int
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nWifi_1 = 3;			//wifi 区1节点数量
  uint32_t nWifi_2 = 4;				//wifi 区2节点数量
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
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);	//启动记录组件
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
  YansWifiPhyHelper phy_1 = YansWifiPhyHelper::Default ();
  phy_1.SetChannel (channel_1.Create ());

  //配置速率控制算法，AARF算法
  WifiHelper wifi_1 = WifiHelper::Default ();
  wifi_1.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac_1 = NqosWifiMacHelper::Default ();

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

  //放置echo服务端程序在Section 1 最右边的wifi节点,端口为6
  UdpEchoServerHelper echoServer (6);

  ApplicationContainer serverApps = echoServer.Install (wifiStaNodes_1.Get (nWifi_1 - 1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  //回显客户端放在最后的STA节点，指向Wifi网络服务器上面的节点地址，端口为6
  UdpEchoClientHelper echoClient (wifi_1Interfaces.GetAddress (nWifi_1 - 1), 6);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

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
      pointToPoint.EnablePcapAll ("pro_2.1");
      phy_1.EnablePcap ("pro_2.1", apDevices_1.Get (0));
      phy_2.EnablePcap ("pro_2.1", apDevices_2.Get (0));
    }

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
