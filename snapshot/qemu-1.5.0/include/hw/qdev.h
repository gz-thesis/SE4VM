#ifndef QDEV_H
#define QDEV_H

#include "hw/hw.h"
#include "hw/qdev-core.h"
#include "hw/qdev-properties.h"

void find_device_state_by_name(const char *dev_name);

#endif
