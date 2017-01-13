#ifndef _ROS_earl_GlobalPose_h
#define _ROS_earl_GlobalPose_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace earl
{

  class GlobalPose : public ros::Msg
  {
    public:
      float latitude;
      float longitude;
      float altitude;
      float roll;
      float pitch;
      float yaw;

    GlobalPose():
      latitude(0),
      longitude(0),
      altitude(0),
      roll(0),
      pitch(0),
      yaw(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += serializeAvrFloat64(outbuffer + offset, this->latitude);
      offset += serializeAvrFloat64(outbuffer + offset, this->longitude);
      offset += serializeAvrFloat64(outbuffer + offset, this->altitude);
      offset += serializeAvrFloat64(outbuffer + offset, this->roll);
      offset += serializeAvrFloat64(outbuffer + offset, this->pitch);
      offset += serializeAvrFloat64(outbuffer + offset, this->yaw);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->latitude));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->longitude));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->altitude));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->roll));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->pitch));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->yaw));
     return offset;
    }

    const char * getType(){ return "earl/GlobalPose"; };
    const char * getMD5(){ return "ede7c952a0a170756890c7bafc4c4c52"; };

  };

}
#endif