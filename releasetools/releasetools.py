#!/bin/env python3
#
# Copyright (C) 2021 The LineageOS Project
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

import common
import re

def FullOTA_InstallEnd(info):
  OTA_InstallEnd(info)
  return

def IncrementalOTA_InstallEnd(info):
  OTA_InstallEnd(info)
  return

def AddImage(info, basename, dest):
  data = info.input_zip.read("IMAGES/" + basename)
  common.ZipWriteStr(info.output_zip, basename, data)
  info.script.Print("Patching {} image unconditionally...".format(dest.split('/')[-1]))
  info.script.AppendExtra('package_extract_file("%s", "%s");' % (basename, dest))

def AddImageRadio(info, basename, dest, magic, simple=False):
  if ("RADIO/" + basename) in info.input_zip.namelist():
    data = info.input_zip.read("RADIO/" + basename)
    common.ZipWriteStr(info.output_zip, basename, data);
    info.script.Print("Patching {} image unconditionally...".format(basename.split('.')[0]));
    if simple:
      info.script.AppendExtra('package_extract_file("%s", "%s");' % (basename, dest));
    else:
      info.script.AppendExtra('assert(mark_header_bt("%s", 0, 0, 0));' % (dest));
      info.script.AppendExtra('assert(write_data_bt("%s", "%s", 8, %d));' % (basename, dest, magic));
      info.script.AppendExtra('assert(mark_header_bt("%s", 0, 0, 3142939818));' % (dest));

def OTA_InstallEnd(info):
  AddImage(info, "dtb.img", "/dev/block/by-name/dtb")
  AddImage(info, "dtbo.img", "/dev/block/by-name/dtbo")
  AddImage(info, "vbmeta.img", "/dev/block/by-name/vbmeta")

  AddImageRadio(info, "sboot.bin", "/dev/block/by-name/bota0", 4194304);
  AddImageRadio(info, "cm.bin", "/dev/block/by-name/bota1", 7340032);
  AddImageRadio(info, "up_param.bin", "/dev/block/by-name/bota2", 1884160);
  AddImageRadio(info, "keystorage.bin", "/dev/block/by-name/keystorage", 0, True);
  AddImageRadio(info, "uh.bin", "/dev/block/by-name/uh", 0, True);
  AddImageRadio(info, "modem.bin", "/dev/block/by-name/radio", 0, True);
  AddImageRadio(info, "modem_5g.bin", "/dev/block/by-name/radio2", 0, True);
  AddImageRadio(info, "modem_debug.bin", "/dev/block/by-name/cp_debug", 0, True);
  AddImageRadio(info, "modem_debug_5g.bin", "/dev/block/by-name/cp2_debug", 0, True);
  AddImageRadio(info, "dqmdbg.bin" "/dev/block/by-name/dqmdbg", 0, True);
  AddImageRadio(info, "param.bin" "/dev/block/by-name/param", 0, True);
  return
