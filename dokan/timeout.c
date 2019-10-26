/*
  Dokan : user-mode file system library for Windows

  Copyright (C) 2015 - 2019 Adrien J. <liryna.stark@gmail.com> and Maxime C. <maxime@islog.com>
  Copyright (C) 2007 - 2011 Hiroki Asakawa <info@dokan-dev.net>

  http://dokan-dev.github.io

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <process.h>
#include "dokani.h"

BOOL DOKANAPI DokanResetTimeout(ULONG Timeout, PDOKAN_FILE_INFO FileInfo) {
  BOOL status;
  ULONG returnedLength;
  DOKAN_IO_EVENT *ioEvent;
  PEVENT_INFORMATION eventInfo;
  ULONG eventInfoSize = sizeof(EVENT_INFORMATION);
  WCHAR rawDeviceName[MAX_PATH];

  ioEvent = (DOKAN_IO_EVENT*)FileInfo->DokanContext;

  if (ioEvent == NULL) {
    return FALSE;
  }

  eventInfo = (PEVENT_INFORMATION)DokanMalloc(eventInfoSize);

  if (eventInfo == NULL) {
    return FALSE;
  }
  RtlZeroMemory(eventInfo, eventInfoSize);

  eventInfo->SerialNumber = ioEvent->KernelInfo.EventContext.SerialNumber;
  eventInfo->Operation.ResetTimeout.Timeout = Timeout;
  GetRawDeviceName(ioEvent->DokanInstance->DeviceName, rawDeviceName, MAX_PATH);
  status = SendToDevice(
      rawDeviceName, IOCTL_RESET_TIMEOUT, eventInfo, eventInfoSize, NULL, 0, &returnedLength);

  DokanFree(eventInfo);

  return status;
}
