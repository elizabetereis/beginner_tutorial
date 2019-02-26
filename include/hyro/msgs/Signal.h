#pragma  once

#ifndef  SIGNAL_GENERATOR_MSGS_SIGNAL_H
#define  SIGNAL_GENERATOR_MSGS_SIGNAL_H

#include  <signal_generator_components_export.h>

#include  <string>
#include  <hyro/common/Time.h>

namespace  hyro
{

struct  SIGNAL_GENERATOR_COMPONENTS_EXPORT  Signal
{
	long  int  timestamp;
	std::string  frame_id;
	double  value;
}; 

} // namespace hyro

#include  <hyro/msgs/Signal.proto.h>
#endif  // SIGNAL_GENERATOR_MSGS_SIGNAL_H