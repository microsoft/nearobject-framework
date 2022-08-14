
#include <fstream>
#include <sstream>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>

#include <nearobject/service/NearObjectProfileManager.hxx>
#include "NearObjectProfilePersisterFilesystem.hxx"

using namespace nearobject;
using namespace nearobject::service;

NearObjectProfileManager::NearObjectProfileManager() :
    NearObjectProfileManager(std::make_unique<persistence::NearObjectProfilePersisterFilesystem>())
{}

NearObjectProfileManager::NearObjectProfileManager(std::unique_ptr<persistence::NearObjectProfilePersister> persister) :
    m_persister(std::move(persister))
{}

std::vector<NearObjectProfile>
NearObjectProfileManager::FindMatchingProfiles(const NearObjectProfile& connectionProfile) const
{
    const std::shared_lock profilesLockShared(m_profilesGate);
    return {};
}

std::vector<NearObjectProfile>
NearObjectProfileManager::GetAllProfiles() const
{
    const std::shared_lock profilesLockShared(m_profilesGate);
    return m_profiles;
}

void
NearObjectProfileManager::AddProfile(const NearObjectProfile& profile, ProfileLifetime lifetime)
{
    {
        const std::unique_lock profilesLockExclusive(m_profilesGate);
        m_profiles.push_back(profile);
    }

    if (lifetime == ProfileLifetime::Persistent) {
        PersistProfile(profile);
    }
}

persist::PersistResult
NearObjectProfileManager::PersistProfile(const NearObjectProfile& profile)
{
    rapidjson::Document document;
    std::ifstream readFileHandle{ NearObjectProfileManager::m_persistLocation, std::ios::in };

    if (!readFileHandle.is_open()) {
        // there's no file, so we just prepare the document object as an array
        document.SetArray();
    } else {
        rapidjson::IStreamWrapper isw(readFileHandle);
        if (document.ParseStream(isw).HasParseError()) {
            return persist::PersistResult::FailedToParseFile;
        }
        readFileHandle.close();
        if (document.IsNull()) {
            // the file was empty so we just prepare the document object as an array
            document.SetArray();
        } else if (!document.IsArray()) {
            return persist::PersistResult::FailedToParseFile;
        }
    }

    auto& allocator = document.GetAllocator();
    auto jsonValue = profile.ToJson(allocator);
    document.PushBack(jsonValue, allocator);

    std::ofstream writeFileHandle{ NearObjectProfileManager::m_persistLocation, std::ios::out };
    if (!writeFileHandle.is_open()) {
        return persist::PersistResult::FailedToOpenFile;
    }
    rapidjson::OStreamWrapper osw(writeFileHandle);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
    document.Accept(writer);
    writeFileHandle.close();
    return persist::PersistResult::Succeeded;
}

void
NearObjectProfileManager::SetPersistLocation(std::filesystem::path persistLocation)
{
    m_persistLocation = std::move(persistLocation);
}

std::vector<NearObjectProfile>
NearObjectProfileManager::ReadPersistedProfiles(persist::PersistResult& parseResult) const
{
    std::ifstream readFileHandle;
    readFileHandle.open(NearObjectProfileManager::m_persistLocation, std::ios::in);
    if (!readFileHandle.is_open()) {
        parseResult = persist::PersistResult::FailedToOpenFile;
        return {};
    }

    rapidjson::Document document;

    rapidjson::IStreamWrapper isw(readFileHandle);
    if (document.ParseStream(isw).HasParseError()) {
        parseResult = persist::PersistResult::FailedToParseFile;
        return {};
    }
    readFileHandle.close();

    if (!document.IsArray()) {
        parseResult = persist::PersistResult::FailedToParseFile;
        return {};
    }

    std::vector<NearObjectProfile> profiles;

    // read the document object
    for (auto& jsonObject : document.GetArray()) {
        // for each object, try to parse a profile from it
        NearObjectProfile profile;
        auto res = profile.ParseAndSet(jsonObject);
        if (res != persist::ParseResult::Succeeded) {
            parseResult = persist::PersistResult::FailedToParseFile;
            return {}; 
        }
        profiles.push_back(std::move(profile));
    }

    parseResult = persist::PersistResult::Succeeded;
    return profiles;
}
