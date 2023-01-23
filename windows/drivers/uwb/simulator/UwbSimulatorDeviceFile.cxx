
#include "UwbSimulatorDeviceFile.hxx"

UwbSimulatorDeviceFile::UwbSimulatorDeviceFile(WDFFILEOBJECT wdfFile) :
    m_wdfFile(wdfFile)
{}

/* static */
VOID
UwbSimulatorDeviceFile::OnWdfDestroy(WDFOBJECT wdfFile)
{
    auto instance = GetUwbSimulatorFile(wdfFile);
    if (instance->m_wdfFile != wdfFile) {
        return;
    }

    // Explicitly invoke the destructor since the object was created with placement new.
    instance->OnDestroy();
    instance->~UwbSimulatorDeviceFile();
}

/* static */
VOID
UwbSimulatorDeviceFile::OnWdfRequestCancel(WDFREQUEST request)
{
    auto wdfFile = WdfRequestGetFileObject(request);
    auto instance = GetUwbSimulatorFile(wdfFile);
    if (instance->m_wdfFile != wdfFile) {
        return;
    }

    instance->OnRequestCancel(request);
}

void
UwbSimulatorDeviceFile::OnDestroy()
{
}

void
UwbSimulatorDeviceFile::OnRequestCancel(WDFREQUEST /* request */)
{
}
