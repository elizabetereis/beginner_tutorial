#pragma  once

#ifndef  SIGNAL_GENERATOR_MSGS_SIGNAL_PROTO_H
#define  SIGNAL_GENERATOR_MSGS_SIGNAL_PROTO_H

#include  <signal_generator_components_export.h>
#include  <hyro/msgs/Signal.h>
#include  <hyro/dynamic/ProtobufTraits.h>
#include  <hyro/msgs/Signal.pb.h>

namespace  hyro
{

template  <>
struct  ProtobufTraits<Signal> : public  ProtobufTraitsDefault<Signal, msgs::Signal>
{
  static  void
  FromMessage (const  msgs::Signal  &  msg,
  Signal  *  value)
    {
      value->timestamp  =  msg.timestamp();
      value->frame_id  =  msg.frame_id();
      value->value  =  msg.value();
    } 

  static  void
  ToMessage (const  Signal  &  value,
  msgs::Signal  *  msg)
    {
      msg->set_timestamp(value.timestamp);
      msg->set_frame_id(value.frame_id);
      msg->set_value(value.value);
    }

};

} // namespace hyro

#endif  // SIGNAL_GENERATOR_MSSG_SIGNAL_PROTO_H