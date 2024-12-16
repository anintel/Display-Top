#ifndef UTILS_DRM_H
#define UTILS_DRM_H

#include <stdio.h>
#include <stdint.h>
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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm/drm.h>

#define DRM_DIR "/dev/dri/"
#define DRM_PRIMARY_PREFIX "card"
#define DRM_RENDER_PREFIX "renderD"

/**
 * @brief Finds a DRM (Direct Rendering Manager) device.
 *
 * @param primary If true, searches for the primary DRM device.
 * @return A pointer to a string containing the DRM device path. 
 *         The caller is responsible for freeing this memory.
 */
char *find_drm_device(bool primary);

/**
 * @brief Opens the primary DRM device based on find_drm_device().
 *
 * @return A file descriptor for the primary DRM device, or -1 on failure.
 */
int open_primary_drm_device();

/**
 * @brief Gets the name of a DRM object type.
 *
 * @param object_type The DRM object type.
 * @return A string representing the name of the DRM object type.
 */
const char *drmModeGetObjectTypeName(uint32_t object_type);

/**
 * @brief Gets the string representation of a DRM format.
 *
 * @param format The DRM format.
 * @return A string representing the DRM format.
 */
const char *get_format_str(uint32_t format);

/**
 * @brief Gets the string representation of a DRM modifier.
 *
 * @param modifier The DRM modifier.
 * @return A string representing the DRM modifier.
 */
const char *get_basic_modifier_str(uint64_t modifier);

/**
 * @brief Gets the name of a DRM connector type.
 *
 * @param connector_type The DRM connector type.
 * @return A string representing the name of the DRM connector type.
 */
const char *get_connector_type_name(uint32_t connector_type);

/**
 * @brief Gets the name of a DRM encoder type.
 *
 * @param encoder_type The DRM encoder type.
 * @return A string representing the name of the DRM encoder type.
 */
const char *get_encoder_type_name(uint32_t encoder_type);

#endif // UTILS_DRM_H