
#include "NearObjectProfile.hxx"

Value
NearObjectConnectionProfile::to_serial(Document::AllocatorType& allocator) {
    Value v(kObject);
    v.AddMember("Scope",NearObjectConnectionScope_ToString(Scope),allocator);
    if(Security){
        auto v_security = (*Security).to_serial(allocator);
        v.AddMember("Security",v_security,allocator);
    }
    return std::move(v);
}