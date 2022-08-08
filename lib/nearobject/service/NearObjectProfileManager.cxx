
#include "NearObjectProfileManager.hxx"

using namespace nearobject;
using namespace nearobject::service;

std::vector<NearObjectProfile>
NearObjectProfileManager::FindMatchingProfiles(const NearObjectProfile& connectionProfile) const
{
    const std::shared_lock profilesLockShared{ m_profilesGate };
    return {};
}

std::vector<NearObjectProfile>
NearObjectProfileManager::GetAllProfiles() const
{
    const std::shared_lock profilesLockShared{ m_profilesGate };
    return m_profiles;
}

void
NearObjectProfileManager::AddProfile(const NearObjectProfile& profile, ProfileLifetime lifetime)
{
    const auto profileToAdd = profile;
    {
        const std::unique_lock profilesLockExclusive{ m_profilesGate };
        m_profiles.push_back(profileToAdd);
    }

    if (lifetime == ProfileLifetime::Persistent) {
        PersistProfile(profileToAdd);
    }
}

using namespace rapidjson;
using namespace std;

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <shared/jsonify/rapidjson/prettywriter.h>

void
NearObjectProfileManager::PersistProfile(const NearObjectProfile& profile)
{
    const std::shared_lock profilesLockShared{ m_profilesGate };
    std::string location = NearObjectProfileManager::persist_location;
    fstream newfile;
    newfile.open(location, ios::in);  
    if(!newfile.is_open()) return;    
    
    std::stringstream stringStream;
    stringStream << newfile.rdbuf();
    std::string copyOfStr = stringStream.str();
    
    rapidjson::Document document;

    if(copyOfStr.size()!=0) {
        if (document.Parse(copyOfStr.c_str()).HasParseError()) return ; // TODO do error handling
        if(!document.IsArray()) return;
    } else {
        document.SetArray();
    }

    auto& allocator = document.GetAllocator();
    auto value = profile.to_serial(allocator);
    document.PushBack(value,allocator);
    newfile.close(); 
    
    newfile.open(NearObjectProfileManager::persist_location,ios::out); 
    if (!newfile.is_open()) return; 
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    document.Accept(writer);
    newfile << sb.GetString();
    newfile.close(); 
}

void NearObjectProfileManager::SetPersistLocation(std::string loc){
    persist_location = loc;
}


std::vector<NearObjectProfile>
NearObjectProfileManager::ReadPersistedProfiles() const
{
    std::string location = NearObjectProfileManager::persist_location;
    fstream newfile;
    newfile.open(location, ios::in);  
    if(!newfile.is_open()) return {};    
    
    std::stringstream stringStream;
    stringStream << newfile.rdbuf();
    std::string copyOfStr = stringStream.str();

    rapidjson::Document document;
    if (document.Parse(copyOfStr.c_str()).HasParseError()) return {}; // TODO do error handling
    if(!document.IsArray()) return {};

    std::vector<NearObjectProfile> profiles;

    // read the document object
    for (auto& obj : document.GetArray()){
        // for each object, try to parse a profile from it
        NearObjectProfile profile;
        auto res = profile.parse_and_set(obj);
        if (res != persist::ParseResult::Succeeded) {
            return {}; // TODO do error handling
        }
        profiles.push_back(std::move(profile));
    }

    newfile.close(); 
    return profiles;
}
