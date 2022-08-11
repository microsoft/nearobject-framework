
#include "NearObjectProfileManager.hxx"
#include <fstream>
#include <shared/jsonify/rapidjson/prettywriter.h>

#include <sstream>

using namespace nearobject;
using namespace nearobject::service;
using namespace rapidjson;

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

persist::PersistResult
NearObjectProfileManager::PersistProfile(const NearObjectProfile& profile)
{
    std::string location = NearObjectProfileManager::persist_location;

    rapidjson::Document document;

    std::fstream readfilehandle;
    std::string copyOfFileStr;

    readfilehandle.open(location, std::ios::in);
    if (readfilehandle.is_open()) {
        std::stringstream stringStream;
        stringStream << readfilehandle.rdbuf();
        copyOfFileStr = stringStream.str();
        readfilehandle.close();
    }
    if (copyOfFileStr.size() != 0) {
        if (document.Parse(copyOfFileStr.c_str()).HasParseError()) {
            return persist::PersistResult::FailedToParseFile;
        }
        if (!document.IsArray()) {
            return persist::PersistResult::FailedToParseFile;
        }
    } else {
        document.SetArray();
    }

    auto& allocator = document.GetAllocator();
    auto value = profile.to_json(allocator);
    document.PushBack(value, allocator);

    std::fstream writefilehandle;
    writefilehandle.open(NearObjectProfileManager::persist_location, std::ios::out);
    if (!writefilehandle.is_open()) {
        return persist::PersistResult::FailedToOpenFile;
    }
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    document.Accept(writer);
    writefilehandle << sb.GetString();
    writefilehandle.close();
    return persist::PersistResult::Succeeded;
}

void
NearObjectProfileManager::SetPersistLocation(std::filesystem::path loc)
{
    persist_location = loc;
}

std::vector<NearObjectProfile>
NearObjectProfileManager::ReadPersistedProfiles(persist::PersistResult& rcode) const
{
    std::fstream readfilehandle;
    readfilehandle.open(NearObjectProfileManager::persist_location, std::ios::in);
    if (!readfilehandle.is_open()) {
        rcode = persist::PersistResult::FailedToOpenFile;
        return {};
    }

    std::stringstream stringStream;
    stringStream << readfilehandle.rdbuf();
    std::string copyOfFileStr = stringStream.str();

    rapidjson::Document document;
    if (document.Parse(copyOfFileStr.c_str()).HasParseError()) {
        rcode = persist::PersistResult::FailedToParseFile;
        return {};
    }
    if (!document.IsArray()) {
        rcode = persist::PersistResult::FailedToParseFile;
        return {};
    }

    std::vector<NearObjectProfile> profiles;

    // read the document object
    for (auto& obj : document.GetArray()) {
        // for each object, try to parse a profile from it
        NearObjectProfile profile;
        auto res = profile.parse_and_set(obj);
        if (res != persist::ParseResult::Succeeded) {
            rcode = persist::PersistResult::FailedToParseFile;
            return {}; 
        }
        profiles.push_back(std::move(profile));
    }

    readfilehandle.close();
    rcode = persist::PersistResult::Succeeded;
    return profiles;
}
