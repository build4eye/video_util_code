#include "v4l2_device.h"

void printfCapabilities(struct Camera* c) {
  printf("capabilities:\n");

  for (unsigned char _i = 0; _i < 32; _i++) {
    switch (c->Param.cap.capabilities & (0x00000001 << _i)) {
      case V4L2_CAP_VIDEO_CAPTURE:
        printf("  V4L2_CAP_VIDEO_CAPTURE => Is a video capture device\n");
        break;
      case V4L2_CAP_VIDEO_OUTPUT:
        printf("  V4L2_CAP_VIDEO_OUTPUT =>  Is a video output device \n");
        break;
      case V4L2_CAP_VIDEO_OVERLAY:
        printf("  V4L2_CAP_VIDEO_OVERLAY => Can do video overlay\n");
        break;
      case V4L2_CAP_VBI_CAPTURE:
        printf("  V4L2_CAP_VIDEO_OUTPUT => Is a raw VBI capture device\n");
        break;
      case V4L2_CAP_VBI_OUTPUT:
        printf("  V4L2_CAP_VBI_OUTPUT => Is a raw VBI output device \n");
        break;
      case V4L2_CAP_SLICED_VBI_CAPTURE:
        printf(
            "  V4L2_CAP_SLICED_VBI_CAPTURE => Is a sliced VBI capture "
            "device\n");
        break;
      case V4L2_CAP_SLICED_VBI_OUTPUT:
        printf(
            "  V4L2_CAP_SLICED_VBI_OUTPUT =>  Is a sliced VBI output device "
            "\n");
        break;
      case V4L2_CAP_RDS_CAPTURE:
        printf("  V4L2_CAP_RDS_CAPTURE => RDS data capture\n");
        break;
      case V4L2_CAP_VIDEO_OUTPUT_OVERLAY:
        printf(
            "  V4L2_CAP_VIDEO_OUTPUT_OVERLAY => Can do video output overlay\n");
        break;
      case V4L2_CAP_HW_FREQ_SEEK:
        printf("  V4L2_CAP_HW_FREQ_SEEK => Can do hardware frequency seek\n");
        break;
      case V4L2_CAP_RDS_OUTPUT:
        printf("  V4L2_CAP_RDS_OUTPUT => Is an RDS encoder\n");
        break;
      case V4L2_CAP_VIDEO_CAPTURE_MPLANE:
        printf(
            "  V4L2_CAP_VIDEO_CAPTURE_MPLANE => Is a video output device that "
            "supports multiplanar formats\n");
        break;
      case V4L2_CAP_VIDEO_OUTPUT_MPLANE:
        printf(
            "  V4L2_CAP_VIDEO_OUTPUT_MPLANE => Is a video mem-to-mem device "
            "that supports multiplanar formats \n");
        break;
      case V4L2_CAP_VIDEO_M2M_MPLANE:
        printf("  V4L2_CAP_VIDEO_M2M_MPLANE => Is a video mem-to-mem device\n");
        break;
      case V4L2_CAP_VIDEO_M2M:
        printf("  V4L2_CAP_VIDEO_M2M\n");
        break;
      case V4L2_CAP_TUNER:
        printf("  V4L2_CAP_TUNER => has a tuner \n");
        break;
      case V4L2_CAP_AUDIO:
        printf("  V4L2_CAP_AUDIO => has audio support\n");
        break;
      case V4L2_CAP_RADIO:
        printf("  V4L2_CAP_RADIO => is a radio device\n");
        break;
      case V4L2_CAP_MODULATOR:
        printf("  V4L2_CAP_MODULATOR => has a modulator\n");
        break;
      case V4L2_CAP_SDR_CAPTURE:
        printf("  V4L2_CAP_SDR_CAPTURE => Is a SDR capture device \n");
        break;
      case V4L2_CAP_EXT_PIX_FORMAT:
        printf(
            "  V4L2_CAP_EXT_PIX_FORMAT => Supports the extended pixel "
            "format\n");
        break;
      case V4L2_CAP_SDR_OUTPUT:
        printf("  V4L2_CAP_SDR_OUTPUT => Is a SDR output device\n");
        break;
      case V4L2_CAP_META_CAPTURE:
        printf("  V4L2_CAP_META_CAPTURE => Is a metadata capture device n");
        break;
      case V4L2_CAP_READWRITE:
        printf("  V4L2_CAP_READWRITE => read/write systemcalls\n");
        break;
      case V4L2_CAP_ASYNCIO:
        printf("  V4L2_CAP_ASYNCIO => async I/O\n");
        break;
      case V4L2_CAP_STREAMING:
        printf("  V4L2_CAP_STREAMING => streaming I/O ioctls\n");
        break;
      case V4L2_CAP_TOUCH:
        printf("  V4L2_CAP_TOUCH => Is a touch device\n");
        break;
      case V4L2_CAP_DEVICE_CAPS:
        printf("  V4L2_CAP_DEVICE_CAPS => sets device capabilities field\n");
        break;
      default:
        break;
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) return -1;

  struct Camera* c = NewCamera(argv[1]);

  //打印设备信息
  printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n",
         c->Param.cap.driver, c->Param.cap.card, c->Param.cap.bus_info,
         (c->Param.cap.version >> 16) & 0XFF,
         (c->Param.cap.version >> 8) & 0XFF, c->Param.cap.version & 0XFF);
  printfCapabilities(c);

  //打印设备支持的格式
  printf("Support format:\n");
  for (unsigned char _i = 0; _i < 255; _i++) {
    char* pf = (void*)0;
    pf = c->GetVideoPixelFormat(c, _i);
    if (pf != 0) printf(" %d.%s\n", _i, pf);
  }
  // TODO:打印支持的像素　eg:1920*1080

  c->Param.format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  c->Param.format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;  // jpg格式
  // c->Param.format.fmt.pix.height = 480;
  // c->Param.format.fmt.pix.width = 640;
  c->Param.format.fmt.pix.height = 720;
  c->Param.format.fmt.pix.width = 1280;
  c->Param.format.fmt.pix.field = V4L2_FIELD_NONE;
  //设置流相关，帧率
  c->Param.stream.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  c->Param.stream.parm.capture.timeperframe.denominator = 30;
  c->Param.stream.parm.capture.timeperframe.numerator = 1;

  c->Param.reqbuf.count = 4;
  c->Param.reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  c->Param.reqbuf.memory = V4L2_MEMORY_MMAP;
  c->Init(c);

  c->Start(c);

  struct RawBuffer* buf;
  char file_name[50];
  long long int t = 0;
  long long int t2 = 0;

  for (int _i = 0; _i < 9; _i++) {
    buf = c->PopRaw(c);
    memset(file_name, 0, sizeof(file_name));
    sprintf(file_name, "./%d-%d", buf->index, _i);
    strcat(file_name, ".jpg");
    FILE* fp2 = fopen(file_name, "wb");
    if (!fp2) {
      printf("open %s error\n", file_name);
      return -1;
    }
    // FIXME：为什么是*２
    fwrite(buf->start, 1280 * 720 * 2, 1, fp2);
    fclose(fp2);
    printf("save %s OK\n", file_name);

    t = buf->timestamp - t2;
    t2 = buf->timestamp;
    printf("t =  %d\n", t);

    c->PushRaw(c, buf);
  }

  c->Stop(c);

  c->Close(c);
}
