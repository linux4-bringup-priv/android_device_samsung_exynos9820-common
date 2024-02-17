#
# Copyright (C) 2023 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

COMMON_PATH := device/samsung/exynos9820-common

## Inherit exynos9820 configuration
include $(COMMON_PATH)/BoardConfigPieLaunched.mk

## Audio
BOARD_USE_CAMCORDER_QUAD_MIC := true

## Dynamic Partitions
ifneq ($(filter d1x d2x, $(TARGET_DEVICE)),)
BOARD_SUPER_PARTITION_SIZE := 6480199680
BOARD_SAMSUNG_DYNAMIC_PARTITIONS_SIZE := 6476005376
else
BOARD_SUPER_PARTITION_SIZE := 6502219776
BOARD_SAMSUNG_DYNAMIC_PARTITIONS_SIZE := 6498025472
endif

## Platform
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/exynos9825_manifest.xml
TARGET_BOARD_PLATFORM := universal9825
TARGET_BOOTLOADER_BOARD_NAME := exynos9825

## Properties
TARGET_VENDOR_PROP += $(COMMON_PATH)/vendor.exynos9825.prop
