/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <unistd.h>

#include <edify/expr.h>
#include <otautil/error_code.h>
#include <ziparchive/zip_archive.h>

Value *MarkHeaderBtFn(const char* name, State *state,
                        const std::vector<std::unique_ptr<Expr>>& argv) {
  int ret = 0;
  std::vector<std::string> args;
  const char* partition;
  uint32_t magicOffset;
  uint32_t magic2;
  uint32_t magic1;

  if(argv.size() < 4 || argv.size() > 4 || !ReadArgs(state, argv, &args))
    return ErrorAbort(state, kArgsParsingFailure,
                      "%s() error parsing arguments", name);

  partition = args[0].c_str();
  magicOffset = std::atoi(args[1].c_str());
  magic2 = std::atoi(args[2].c_str());
  magic1 = std::atoi(args[3].c_str());

  int fd = open(partition, O_RDWR);
  if (fd < 0)
    return ErrorAbort(state, kFileOpenFailure,
                      "%s() failed to open %s", name, partition);

  magic1 = magic1 << (magicOffset * 8);
  magic2 = magic2 << (magicOffset * 8);
  write(fd, (char*)&magic1, sizeof(uint32_t));
  write(fd, (char*)&magic2, sizeof(uint32_t));
  close(fd);

  return StringValue(std::to_string(ret));
}

#define FILENAME_MAX_LEN 32

Value *WriteDataBtFn(const char* name, State *state,
                        const std::vector<std::unique_ptr<Expr>>& argv) {
  int ret = 0;
  std::vector<std::string> args;
  const char* file;
  const char* partition;
  uint32_t offset;
  uint32_t magic;

  if(argv.size() < 4 || argv.size() > 4 || !ReadArgs(state, argv, &args))
    return ErrorAbort(state, kArgsParsingFailure,
                      "%s() error parsing arguments", name);

  file = args[0].c_str();
  partition = args[1].c_str();
  offset = std::atoi(args[2].c_str());
  magic = std::atoi(args[3].c_str());

  int fd = open(partition, O_RDWR);
  if (fd < 0)
    return ErrorAbort(state, kFileOpenFailure,
                      "%s() failed to open %s", name, partition);

  char filename_padded[FILENAME_MAX_LEN] = { 0 };
  strcpy(&filename_padded[0], file);

  lseek(fd, offset, SEEK_SET);
  write(fd, &filename_padded[0], FILENAME_MAX_LEN);
  write(fd, (char*)&magic, sizeof(uint32_t));

  // write data
  ZipArchiveHandle za = state->updater->GetPackageHandle();
  ZipEntry64 entry;
  if (FindEntry(za, file, &entry) != 0) {
    return ErrorAbort(state, kPackageExtractFileFailure,
                      "%s() %s not found in package", name, file);
  }

  if (ExtractEntryToFile(za, &entry, fd))
    return ErrorAbort(state, kPackageExtractFileFailure,
                      "%s() failed to extract %s from package", name, file);

  close(fd);

  return StringValue(std::to_string(ret));
}

void Register_librecovery_updater_exynos9820() {
  RegisterFunction("mark_header_bt", MarkHeaderBtFn);
  RegisterFunction("write_data_bt", WriteDataBtFn);
}
