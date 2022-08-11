
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
    rapidjson::Document document;

    std::ifstream readfilehandle;
    std::string copyOfFileStr;

    readfilehandle.open(NearObjectProfileManager::persist_location, std::ios::in);
    if (!readfilehandle.is_open()) {
        // there's no file, so we just prepare the document object as an array
        document.SetArray();
    } else {
        rapidjson::IStreamWrapper isw(readfilehandle);
        if (document.ParseStream(isw).HasParseError()) {
            return persist::PersistResult::FailedToParseFile;
        }
        readfilehandle.close();
        if (document.IsNull()) {
            // the file was empty so we just prepare the document object as an array
            document.SetArray();
        } else if (!document.IsArray()) {
            return persist::PersistResult::FailedToParseFile;
        }
    }

    auto& allocator = document.GetAllocator();
    auto value = profile.to_json(allocator);
    document.PushBack(value, allocator);

    std::ofstream writefilehandle;
    writefilehandle.open(NearObjectProfileManager::persist_location, std::ios::out);
    if (!writefilehandle.is_open()) {
        return persist::PersistResult::FailedToOpenFile;
    }
    rapidjson::OStreamWrapper osw(writefilehandle);
    PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    document.Accept(writer);
    writefilehandle.close();
    return persist::PersistResult::Succeeded;
}

void
NearObjectProfileManager::SetPersistLocation(std::filesystem::path loc)
{
    persist_location = std::move(loc);
}

std::vector<NearObjectProfile>
NearObjectProfileManager::ReadPersistedProfiles(persist::PersistResult& rcode) const
{
    std::ifstream readfilehandle;
    readfilehandle.open(NearObjectProfileManager::persist_location, std::ios::in);
    if (!readfilehandle.is_open()) {
        rcode = persist::PersistResult::FailedToOpenFile;
        return {};
    }

    rapidjson::Document document;

    rapidjson::IStreamWrapper isw(readfilehandle);
    if (document.ParseStream(isw).HasParseError()) {
        rcode = persist::PersistResult::FailedToParseFile;
        return {};
    }
    readfilehandle.close();

    if (!document.IsArray()) {
        rcode = persist::PersistResult::FailedToParseFile;
        return {};
    }

    std::vector<NearObjectProfile> profiles;

    // read the document object
    for (auto& obj : document.GetArray()) {
        // for each object, try to parse a profile from it
        NearObjectProfile profile;
        auto res = profile.ParseAndSet(obj);
        if (res != persist::ParseResult::Succeeded) {
            rcode = persist::PersistResult::FailedToParseFile;
            return {}; 
        }
        profiles.push_back(std::move(profile));
    }

    rcode = persist::PersistResult::Succeeded;
    return profiles;
}
