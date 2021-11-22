/* x86_64_efi.c
 *
 * Copyright (C) 2021 wolfSSL Inc.
 *
 * This file is part of wolfBoot.
 *
 * wolfBoot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfBoot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

#include <stdint.h>
#include <target.h>

#include "image.h"
#include "loader.h"

#include <efi/efi.h>
#include <efi/efilib.h>

#ifdef __WOLFBOOT
void hal_init(void)
{
}

void hal_prepare_boot(void)
{
}

#endif

EFI_PHYSICAL_ADDRESS kernel_addr;

int RAMFUNCTION hal_flash_write(uint32_t address, const uint8_t *data, int len)
{
    return 0;
}

void RAMFUNCTION hal_flash_unlock(void)
{
}

void RAMFUNCTION hal_flash_lock(void)
{
}

int RAMFUNCTION hal_flash_erase(uint32_t address, int len)
{
    return 0;
}

static void panic()
{
    while(1) {}
}

UINT64 FileSize(EFI_FILE_HANDLE FileHandle)
{
  EFI_FILE_INFO *FileInfo;
  UINT64 ret;

  FileInfo = LibFileInfo(FileHandle);
  if (FileInfo == NULL)
      panic();

  ret = FileInfo->FileSize;
  FreePool(FileInfo);

  return ret;
}

EFI_FILE_HANDLE GetVolume(EFI_HANDLE image)
{
  EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
  EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  EFI_LOADED_IMAGE *loaded_image = NULL;
  EFI_FILE_IO_INTERFACE *IOVolume;
  EFI_FILE_HANDLE Volume;
  EFI_STATUS status;

  status = uefi_call_wrapper(BS->HandleProtocol, 3,
                             image, &lipGuid, (void **) &loaded_image);
  if (status != EFI_SUCCESS)
      panic();

  status = uefi_call_wrapper(BS->HandleProtocol, 3,
                             loaded_image->DeviceHandle,
                             &fsGuid, (VOID*)&IOVolume);
  if (status != EFI_SUCCESS)
      panic();

  status = uefi_call_wrapper(IOVolume->OpenVolume, 2, IOVolume, &Volume);

  if (status != EFI_SUCCESS)
      panic();

  return Volume;
}

EFI_FILE_HANDLE openFile(CHAR16 *file, EFI_FILE_HANDLE volume)
{

    EFI_FILE_HANDLE file_handle;
    EFI_STATUS status;

    status = uefi_call_wrapper(volume->Open, 5,
                             volume,
                             &file_handle,
                             file,
                             EFI_FILE_MODE_READ,
                             EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

    if (status != EFI_SUCCESS)
        file_handle = NULL;

    return file_handle;
}

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_LOADED_IMAGE *loaded_image = NULL;
    EFI_STATUS status;

    InitializeLib(ImageHandle, SystemTable);
    status = uefi_call_wrapper(SystemTable->BootServices->HandleProtocol,
                               3,
                               ImageHandle,
                               &LoadedImageProtocol,
                               (void **)&loaded_image);

#if DEBUG == 1
    if (status == EFI_SUCCESS)
        Print(L"Image base: 0x%lx\n", loaded_image->ImageBase);
#endif

    return EFI_SUCCESS;
}

