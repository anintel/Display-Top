

#include "utils_drm.h"

bool starts_with(const char *str, const char *prefix)
{
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

char *find_drm_device(bool primary)
{
    struct dirent *entry;
    DIR *drm_dir = opendir(DRM_DIR);
    char *device_path = NULL;
    const char *prefix = primary ? DRM_PRIMARY_PREFIX : DRM_RENDER_PREFIX;

    if (!drm_dir)
    {
        perror("Failed to open /dev/dri directory");
        return NULL;
    }

    while ((entry = readdir(drm_dir)) != NULL)
    {
        if (starts_with(entry->d_name, prefix))
        {
            device_path = malloc(strlen(DRM_DIR) + strlen(entry->d_name) + 1);
            if (!device_path)
            {
                perror("Failed to allocate memory for device path");
                closedir(drm_dir);
                return NULL;
            }
            sprintf(device_path, "%s%s", DRM_DIR, entry->d_name);
            break;
        }
    }

    closedir(drm_dir);

    if (!device_path)
    {
        fprintf(stderr, "No DRM %s devices found in %s\n", primary ? "primary (card*)" : "render (renderD*)", DRM_DIR);
    }

    return device_path;
}

int open_primary_drm_device()
{
    char *device_path = find_drm_device(true);
    int fd = open(device_path, O_RDWR | O_CLOEXEC);
    if (fd < 0)
    {
        perror("Failed to open DRM device");
    }
    return fd;
}

#define MODE_INFO_STRING_LEN 128

char *get_mode_info_string(const drmModeModeInfo *mode)
{
    char *info_string = malloc(MODE_INFO_STRING_LEN);

    if (!info_string)
    {
        perror("Failed to allocate memory for mode info string");
        return NULL;
    }

    snprintf(info_string, MODE_INFO_STRING_LEN,
             "Name: %s, HDisplay: %d, VDisplay: %d, VRefresh: %.2f",
             mode->name, mode->hdisplay, mode->vdisplay,
             (float)mode->vrefresh / 1000.0);

    return info_string;
}

char *get_connector_mode_name(const drmModeConnector *connector)
{
    if (connector->count_modes == 0)
    {
        return strdup("No modes available");
    }

    drmModeModeInfo *mode = &connector->modes[0];
    return get_mode_info_string(mode);
}

const char *get_plane_format_name(uint32_t format)
{
    switch (format)
    {
    case DRM_FORMAT_INVALID:
        return "INVALID";
    case DRM_FORMAT_C1:
        return "C1";
    case DRM_FORMAT_C2:
        return "C2";
    case DRM_FORMAT_C4:
        return "C4";
    case DRM_FORMAT_C8:
        return "C8";
    case DRM_FORMAT_D1:
        return "D1";
    case DRM_FORMAT_D2:
        return "D2";
    case DRM_FORMAT_D4:
        return "D4";
    case DRM_FORMAT_D8:
        return "D8";
    case DRM_FORMAT_R1:
        return "R1";
    case DRM_FORMAT_R2:
        return "R2";
    case DRM_FORMAT_R4:
        return "R4";
    case DRM_FORMAT_R8:
        return "R8";
    case DRM_FORMAT_R10:
        return "R10";
    case DRM_FORMAT_R12:
        return "R12";
    case DRM_FORMAT_R16:
        return "R16";
    case DRM_FORMAT_RG88:
        return "RG88";
    case DRM_FORMAT_GR88:
        return "GR88";
    case DRM_FORMAT_RG1616:
        return "RG1616";
    case DRM_FORMAT_GR1616:
        return "GR1616";
    case DRM_FORMAT_RGB332:
        return "RGB332";
    case DRM_FORMAT_BGR233:
        return "BGR233";
    case DRM_FORMAT_XRGB4444:
        return "XRGB4444";
    case DRM_FORMAT_XBGR4444:
        return "XBGR4444";
    case DRM_FORMAT_RGBX4444:
        return "RGBX4444";
    case DRM_FORMAT_BGRX4444:
        return "BGRX4444";
    case DRM_FORMAT_ARGB4444:
        return "ARGB4444";
    case DRM_FORMAT_ABGR4444:
        return "ABGR4444";
    case DRM_FORMAT_RGBA4444:
        return "RGBA4444";
    case DRM_FORMAT_BGRA4444:
        return "BGRA4444";
    case DRM_FORMAT_XRGB1555:
        return "XRGB1555";
    case DRM_FORMAT_XBGR1555:
        return "XBGR1555";
    case DRM_FORMAT_RGBX5551:
        return "RGBX5551";
    case DRM_FORMAT_BGRX5551:
        return "BGRX5551";
    case DRM_FORMAT_ARGB1555:
        return "ARGB1555";
    case DRM_FORMAT_ABGR1555:
        return "ABGR1555";
    case DRM_FORMAT_RGBA5551:
        return "RGBA5551";
    case DRM_FORMAT_BGRA5551:
        return "BGRA5551";
    case DRM_FORMAT_RGB565:
        return "RGB565";
    case DRM_FORMAT_BGR565:
        return "BGR565";
    case DRM_FORMAT_RGB888:
        return "RGB888";
    case DRM_FORMAT_BGR888:
        return "BGR888";
    case DRM_FORMAT_XRGB8888:
        return "XRGB8888";
    case DRM_FORMAT_XBGR8888:
        return "XBGR8888";
    case DRM_FORMAT_RGBX8888:
        return "RGBX8888";
    case DRM_FORMAT_BGRX8888:
        return "BGRX8888";
    case DRM_FORMAT_ARGB8888:
        return "ARGB8888";
    case DRM_FORMAT_ABGR8888:
        return "ABGR8888";
    case DRM_FORMAT_RGBA8888:
        return "RGBA8888";
    case DRM_FORMAT_BGRA8888:
        return "BGRA8888";
    case DRM_FORMAT_XRGB2101010:
        return "XRGB2101010";
    case DRM_FORMAT_XBGR2101010:
        return "XBGR2101010";
    case DRM_FORMAT_RGBX1010102:
        return "RGBX1010102";
    case DRM_FORMAT_BGRX1010102:
        return "BGRX1010102";
    case DRM_FORMAT_ARGB2101010:
        return "ARGB2101010";
    case DRM_FORMAT_ABGR2101010:
        return "ABGR2101010";
    case DRM_FORMAT_RGBA1010102:
        return "RGBA1010102";
    case DRM_FORMAT_BGRA1010102:
        return "BGRA1010102";
    case DRM_FORMAT_XRGB16161616:
        return "XRGB16161616";
    case DRM_FORMAT_XBGR16161616:
        return "XBGR16161616";
    case DRM_FORMAT_ARGB16161616:
        return "ARGB16161616";
    case DRM_FORMAT_ABGR16161616:
        return "ABGR16161616";
    case DRM_FORMAT_XRGB16161616F:
        return "XRGB16161616F";
    case DRM_FORMAT_XBGR16161616F:
        return "XBGR16161616F";
    case DRM_FORMAT_ARGB16161616F:
        return "ARGB16161616F";
    case DRM_FORMAT_ABGR16161616F:
        return "ABGR16161616F";
    case DRM_FORMAT_AXBXGXRX106106106106:
        return "AXBXGXRX106106106106";
    case DRM_FORMAT_YUYV:
        return "YUYV";
    case DRM_FORMAT_YVYU:
        return "YVYU";
    case DRM_FORMAT_UYVY:
        return "UYVY";
    case DRM_FORMAT_VYUY:
        return "VYUY";
    case DRM_FORMAT_AYUV:
        return "AYUV";
    case DRM_FORMAT_AVUY8888:
        return "AVUY8888";
    case DRM_FORMAT_XYUV8888:
        return "XYUV8888";
    case DRM_FORMAT_XVUY8888:
        return "XVUY8888";
    case DRM_FORMAT_VUY888:
        return "VUY888";
    case DRM_FORMAT_VUY101010:
        return "VUY101010";
    case DRM_FORMAT_Y210:
        return "Y210";
    case DRM_FORMAT_Y212:
        return "Y212";
    case DRM_FORMAT_Y216:
        return "Y216";
    case DRM_FORMAT_Y410:
        return "Y410";
    case DRM_FORMAT_Y412:
        return "Y412";
    case DRM_FORMAT_Y416:
        return "Y416";
    case DRM_FORMAT_XVYU2101010:
        return "XVYU2101010";
    case DRM_FORMAT_XVYU12_16161616:
        return "XVYU12_16161616";
    case DRM_FORMAT_XVYU16161616:
        return "XVYU16161616";
    case DRM_FORMAT_Y0L0:
        return "Y0L0";
    case DRM_FORMAT_X0L0:
        return "X0L0";
    case DRM_FORMAT_Y0L2:
        return "Y0L2";
    case DRM_FORMAT_X0L2:
        return "X0L2";
    case DRM_FORMAT_YUV420_8BIT:
        return "YUV420_8BIT";
    case DRM_FORMAT_YUV420_10BIT:
        return "YUV420_10BIT";
    case DRM_FORMAT_XRGB8888_A8:
        return "XRGB8888_A8";
    case DRM_FORMAT_XBGR8888_A8:
        return "XBGR8888_A8";
    case DRM_FORMAT_RGBX8888_A8:
        return "RGBX8888_A8";
    case DRM_FORMAT_BGRX8888_A8:
        return "BGRX8888_A8";
    case DRM_FORMAT_RGB888_A8:
        return "RGB888_A8";
    case DRM_FORMAT_BGR888_A8:
        return "BGR888_A8";
    case DRM_FORMAT_RGB565_A8:
        return "RGB565_A8";
    case DRM_FORMAT_BGR565_A8:
        return "BGR565_A8";
    case DRM_FORMAT_NV12:
        return "NV12";
    case DRM_FORMAT_NV21:
        return "NV21";
    case DRM_FORMAT_NV16:
        return "NV16";
    case DRM_FORMAT_NV61:
        return "NV61";
    case DRM_FORMAT_NV24:
        return "NV24";
    case DRM_FORMAT_NV42:
        return "NV42";
    case DRM_FORMAT_NV15:
        return "NV15";
    case DRM_FORMAT_NV20:
        return "NV20";
    case DRM_FORMAT_NV30:
        return "NV30";
    case DRM_FORMAT_P210:
        return "P210";
    case DRM_FORMAT_P010:
        return "P010";
    case DRM_FORMAT_P012:
        return "P012";
    case DRM_FORMAT_P016:
        return "P016";
    case DRM_FORMAT_P030:
        return "P030";
    case DRM_FORMAT_Q410:
        return "Q410";
    case DRM_FORMAT_Q401:
        return "Q401";
    case DRM_FORMAT_YUV410:
        return "YUV410";
    case DRM_FORMAT_YVU410:
        return "YVU410";
    case DRM_FORMAT_YUV411:
        return "YUV411";
    case DRM_FORMAT_YVU411:
        return "YVU411";
    case DRM_FORMAT_YUV420:
        return "YUV420";
    case DRM_FORMAT_YVU420:
        return "YVU420";
    case DRM_FORMAT_YUV422:
        return "YUV422";
    case DRM_FORMAT_YVU422:
        return "YVU422";
    case DRM_FORMAT_YUV444:
        return "YUV444";
    case DRM_FORMAT_YVU444:
        return "YVU444";
    default:
        return "unknown";
    }
}