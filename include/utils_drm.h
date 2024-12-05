#ifndef UTILS_DRM_H
#define UTILS_DRM_H

#include <stdio.h>
#include <stdlib.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <drm/drm_fourcc.h>
#include <xf86drmMode.h>

#define DRM_DIR "/dev/dri/"
#define DRM_PRIMARY_PREFIX "card"
#define DRM_RENDER_PREFIX "renderD"

char *find_drm_device(bool primary);
int open_primary_drm_device();
const char *get_plane_format_name(uint32_t format);

#endif