
add_library(secureproximity STATIC "")

target_sources(secureproximity
    PRIVATE
        ISecureDevice.hxx
        ISecureDeviceChannel.hxx
        SecureDevice.cxx
        SecureDevice.hxx
)

set_target_properties(secureproximity PROPERTIES FOLDER lib/secureproximity)
