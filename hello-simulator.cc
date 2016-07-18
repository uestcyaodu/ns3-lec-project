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

#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("HelloSimulator");

static void printHello(
	std::string name,std::string number,float stop_at)
{
	std::cout<<Simulator::Now()<<" Hello "<<name<<"_ID:"<<number<<"_running_time(s):"<<stop_at<<std::endl;
	Simulator::Schedule(Seconds(5),&printHello,name,number,stop_at);
}
// ./test arg0 arg1 arg2
// argc=4
// argv[0] -> "test"
// argv[1] -> "arg0"
// argv[2] -> "arg1"
// argv[3] -> "arg2"
// argv[4] -> NULL
int
main (int argc, char *argv[])
{
	CommandLine cmd;
	std::string name;
	std::string number;
  float stop_at;
	cmd.AddValue ("name", "my_name", name);
	cmd.AddValue ("number","ID", number);
	cmd.AddValue ("stop_at","running_time", stop_at);
	cmd.Parse(argc,argv);
	printHello(name,number,stop_at);
	Simulator::Stop(Seconds(stop_at));
	Simulator::Run ();
	Simulator::Destroy ();
}
