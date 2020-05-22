#ifndef _V4L3_DEVICE_
#define _V4L3_DEVICE_

//一个人性化的v4l2设备操作库
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct CameraInfo {
  struct v4l2_capability cap;  //设备属性
};

struct Camera {
  struct CameraInfo Info;

  void (*Close)(struct Camera* camera);
  //获取设备支持的格式：eg:YUYV 4:2:2 or Motion-JPEG
  char* (*GetVideoPixelFormat)(struct Camera* camera, unsigned int index);
  //判断设备是否支持每种格式
  // eg: format = V4L2_PIX_FMT_RGB32
  int (*IsSuppertPixelFormat)(struct Camera* camera, unsigned int format);
};

struct Camera* NewCamera(const char* cameraName);

#endif
