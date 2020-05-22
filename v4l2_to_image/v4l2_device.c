//一个人性化的v4l2设备操作库
#include "v4l2_device.h"

struct CameraClass {
  struct Camera Camera;
  int fd;
};
typedef struct CameraClass* CameraClass_P;

static void _close(struct Camera* camera);
static char* _getVideoPixelFormat(struct Camera* camera, unsigned int index);
static int _isSuppertPixelFormat(struct Camera* camera, unsigned int format);

// NewCamera fail return 0
// cameraName = "/dev/video0"
struct Camera* NewCamera(const char* cameraName) {
  CameraClass_P c = (CameraClass_P)(malloc(sizeof(struct CameraClass)));
  if (c == 0) return (void*)0;

  c->fd = open(cameraName, O_RDWR);
  if (c->fd == 0) return (void*)0;

  c->Camera.Close = _close;
  c->Camera.GetVideoPixelFormat = _getVideoPixelFormat;
  c->Camera.IsSuppertPixelFormat =_isSuppertPixelFormat;

  //获取设备信息
  ioctl(c->fd, VIDIOC_QUERYCAP, &(c->Camera.Info.cap));

  return (struct Camera*)c;
}

//_close　释放资源
static void _close(struct Camera* camera) {
  if (camera == 0) return;

  CameraClass_P c = (CameraClass_P)camera;

  close(c->fd);

  free(c);
}

//获取设备支持的格式：eg:YUYV 4:2:2 or Motion-JPEG
static char* _getVideoPixelFormat(struct Camera* camera, unsigned int index) {
  if (camera == 0) goto _exit;
  CameraClass_P c = (CameraClass_P)camera;

  static struct v4l2_fmtdesc fmtdesc;
  fmtdesc.index = index;
  fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (ioctl(c->fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1) {
    return fmtdesc.description;
  }

_exit:
  return (void*)0;
}

//判断设备是否支持每种格式
// eg: format = V4L2_PIX_FMT_RGB32
static int _isSuppertPixelFormat(struct Camera* camera, unsigned int format) {
  if (camera == 0) goto _exit;
  CameraClass_P c = (CameraClass_P)camera;

  struct v4l2_format fmt;
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.pixelformat = format;
  if (ioctl(c->fd, VIDIOC_TRY_FMT, &fmt) != -1) return 1;

_exit:
  return 0;
}

int cameraInfo() {
  struct v4l2_capability cap;

  int fd = open("/dev/video0", O_RDWR);  // 打开设备

  ioctl(fd, VIDIOC_QUERYCAP, &cap);

  printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n",
         cap.driver, cap.card, cap.bus_info, (cap.version >> 16) & 0XFF,
         (cap.version >> 8) & 0XFF, cap.version & 0XFF);

  struct v4l2_fmtdesc fmtdesc;
  fmtdesc.index = 0;
  fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  printf("Support format:\n");

  while (ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1)

  {
    printf("\t%d.%s\n", fmtdesc.index + 1, fmtdesc.description);

    fmtdesc.index++;
  }

  struct v4l2_format fmt;
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  ioctl(fd, VIDIOC_G_FMT, &fmt);

  printf("Current data format information:\n\twidth: %d\n\theight: %d\n ",
         fmt.fmt.pix.width, fmt.fmt.pix.height);

  struct v4l2_fmtdesc fmtd;
  fmtd.index = 0;
  fmtd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  while (ioctl(fd, VIDIOC_ENUM_FMT, &fmtd) != -1)

  {
    if (fmtd.pixelformat & fmt.fmt.pix.pixelformat)

    {
      printf("\tformat:%s\n", fmtd.description);

      break;
    }

    fmtd.index++;
  }

  close(fd);  // 关闭设备
}
