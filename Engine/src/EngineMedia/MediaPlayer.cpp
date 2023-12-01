#include "EngineMedia/MediaPlayer.h"
#include "components/Audio.h"
#include "components/Channel.h"
#include "components/AudioOcclusion.h"
#include "components/collision.h"
#include "components/Transform.h"
#include "common/utilsMat.h"
#include "scene/SceneView.h"

MediaPlayer* MediaPlayer::m_pInstance = nullptr;

MediaPlayer::MediaPlayer()
{
    m_pAudioManager = new AudioManager();

    // Singleton
    assert(!m_pInstance);
}

MediaPlayer::~MediaPlayer()
{
    delete m_pAudioManager;
}

MediaPlayer* MediaPlayer::Get()
{
    if (MediaPlayer::m_pInstance == nullptr)
    {
        MediaPlayer::m_pInstance = new MediaPlayer();
    }
    return MediaPlayer::m_pInstance;
}

bool MediaPlayer::Initialize(std::string baseAudiosPath)
{
    m_pAudioManager->SetBasePath(baseAudiosPath);

    bool isAudioInit = m_pAudioManager->Initialize(SceneView::Get()->GetNumComponents("channel"));
    if (!isAudioInit)
    {
        return false;
    }

    return true;
}

bool MediaPlayer::LoadScene()
{
    printf("Loading media player...\n");

    bool isChannelsLoaded = LoadChannels();
    if (!isChannelsLoaded)
    {
        return false;
    }

    bool isOcclusionsLoaded = LoadOcclusions();
    if (!isOcclusionsLoaded)
    {
        return false;
    }

    bool isAudiosLoaded = LoadAudios();
    if (!isAudiosLoaded)
    {
        return false;
    }

    return true;
}

bool MediaPlayer::LoadOcclusions()
{
    printf("Loading occlusions...\n");

    using namespace myutils;
    using namespace std;
    using namespace glm;

    m_pAudioManager->ClearOcclusions();

    for (SceneView::Get()->First("audioOcclusion"); !SceneView::Get()->IsDone(); SceneView::Get()->Next())
    {
        EntityID entityId = SceneView::Get()->CurrentKey();
        AudioOcclusionComponent* pAudioOcclusion = SceneView::Get()->CurrentValue<AudioOcclusionComponent>();
        TransformComponent* pTransform = SceneView::Get()->GetComponent<TransformComponent>(entityId, "transform");
        CollisionComponent* pCollision = SceneView::Get()->GetComponent<CollisionComponent>(entityId, "collision");

        // For now only AABB for occlusion
        assert(pCollision->Get_eShape() == eShape::AABB);

        sAABB* pAABB = pCollision->GetShape<sAABB>();
        vector<vec3> vertices = GenerateRectangleVertices(pAABB->minXYZ, pAABB->maxXYZ);

        bool occlusionLoaded = m_pAudioManager->AddPolygon(pAudioOcclusion->GetDirect(), pAudioOcclusion->GetReverb(),
            true, vertices, pTransform->GetPosition());

        if (!occlusionLoaded)
        {
            return false;
        }
    }

    return true;
}

bool MediaPlayer::LoadChannels()
{
    printf("Loading channels...\n");

    m_pAudioManager->ClearChannels();

    for (SceneView::Get()->First("channel"); !SceneView::Get()->IsDone(); SceneView::Get()->Next())
    {
        ChannelComponent* pChannel = SceneView::Get()->CurrentValue<ChannelComponent>();

        bool channelLoaded = m_pAudioManager->LoadChannel(pChannel->GetChannel());

        if (!channelLoaded)
        {
            return false;
        }
    }

    return true;
}

bool MediaPlayer::LoadAudios()
{
    printf("Loading audios...\n");

    m_pAudioManager->ClearAudios();

    for (SceneView::Get()->First("audio"); !SceneView::Get()->IsDone(); SceneView::Get()->Next())
    {
        AudioComponent* pAudio = SceneView::Get()->CurrentValue<AudioComponent>();
        FMOD::Sound* pSound = nullptr;

        bool audioLoaded = m_pAudioManager->LoadAudio(pAudio->GetFileName().c_str(),
            pAudio->IsStream(),
            pAudio->IsLoop(),
            pAudio->GetChannelGroup(),
            &pSound);

        if (!audioLoaded)
        {
            return false;
        }

        pAudio->SetSound(pSound);
    }

    return true;
}

void MediaPlayer::Destroy()
{
    m_pAudioManager->Destroy();
    delete m_pAudioManager;
}

void MediaPlayer::Update(double deltaTime)
{
    m_pAudioManager->Update();
}

void MediaPlayer::SetListenerAttributes(const glm::vec3& position, const glm::vec3& velocity,
    const glm::vec3& forward, const glm::vec3& up)
{
    m_pAudioManager->SetListenerAttributes(position, velocity, forward, up);
}

void MediaPlayer::SetAudio3DAttributes(EntityID entityId, const glm::vec3& position, const glm::vec3& velocity)
{
    int channelId = GetChannelId(entityId);

    m_pAudioManager->SetChannel3DAttributes(channelId, position, velocity);
}

void MediaPlayer::PlayAudio(EntityID entityId, glm::vec3 soundVelocity)
{
    AudioComponent* pAudio = SceneView::Get()->GetComponent<AudioComponent>(entityId, "audio");
    TransformComponent* pTransform = SceneView::Get()->GetComponent<TransformComponent>(entityId, "transform");

    int channelId = m_pAudioManager->PlayAudio(pAudio->GetFileName().c_str(),
        pAudio->GetChannelGroup(),
        pTransform->GetPosition(),
        soundVelocity,
        pAudio->GetMinDistance(),
        pAudio->GetMaxDistance());

    pAudio->SetChannelId(channelId);

    // Set audio default parameters
    m_pAudioManager->SetChannelVolume(channelId, pAudio->GetInitialVolume());
}

void MediaPlayer::StopAudio(EntityID entityId)
{
    int channelId = GetChannelId(entityId);

    m_pAudioManager->StopChannel(channelId);
}

void MediaPlayer::PauseAudio(EntityID entityId, bool value)
{
    int channelId = GetChannelId(entityId);

    m_pAudioManager->SetPaused(channelId, value);
}

bool MediaPlayer::SetPitch(EntityID entityId, float value)
{
    int channelId = GetChannelId(entityId);

    return m_pAudioManager->SetChannelPitch(channelId, value);
}

int MediaPlayer::GetChannelId(EntityID entityId)
{
    AudioComponent* pAudio = SceneView::Get()->GetComponent<AudioComponent>(entityId, "audio");
    int channelId = pAudio->GetChannelId();

    return channelId;
}

bool MediaPlayer::IsPlaying(EntityID entityId)
{
    int channelId = GetChannelId(entityId);

    return m_pAudioManager->IsChannelPlaying(channelId);
}

bool MediaPlayer::IsFinished(EntityID entityId)
{
    int channelId = GetChannelId(entityId);

    bool isPlaying = m_pAudioManager->IsChannelPlaying(channelId);
    bool isPaused = m_pAudioManager->IsChannelPaused(channelId);

    if (!isPlaying && !isPaused)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MediaPlayer::SetDopplerLevel(EntityID entityId, float level)
{
    int channelId = GetChannelId(entityId);
        
    m_pAudioManager->SetDopplerLevel(channelId, level);
}

void MediaPlayer::SetReverberation(EntityID entityId, float decay, float density, float diffusion)
{
    int channelId = GetChannelId(entityId);
        
    m_pAudioManager->AddReverbFilterOnChannel(channelId);
    m_pAudioManager->SetReverbValuesOnChannel(channelId, decay, density, diffusion);
}

void MediaPlayer::SetDistortion(EntityID entityId, float level)
{
    int channelId = GetChannelId(entityId);
     
    m_pAudioManager->AddDistortionFilterOnChannel(channelId);
    m_pAudioManager->SetDistortionLevelFilterValuesOnChannel(channelId, level);
}

void MediaPlayer::SetHighpass(EntityID entityId)
{
    int channelId = GetChannelId(entityId);

    m_pAudioManager->AddHighPassFilterOnChannel(channelId);
}

void MediaPlayer::SetLowpass(EntityID entityId)
{
    int channelId = GetChannelId(entityId);

    m_pAudioManager->AddLowPassFilterOnChannel(channelId);
}
