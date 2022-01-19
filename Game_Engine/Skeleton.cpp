#include "stdafx.h"
#include "Skeleton.h"

void Skeleton::play(float dt, bool once, float scale)
{
    g_done = false;
    m_useBlendBones = false;
    if(once)
    {
        
        float time = dt;
        m_time += time * scale;
        
       
        float frameCount = m_time * m_fps;

       
        if (frameCount <= (float)m_nrOfKeyFrames)
        {
            
            play(m_identityMatrix, m_time, m_boneNrMap[0].name);

        }
        else
        {
            g_done = true;
            m_time = 0;
        }
      

    }
    else
    {
        
        float time = dt;
        m_time += time*scale;
        play(m_identityMatrix, m_time, m_boneNrMap[0].name); //the root should be at slot 0.
    }

   
}

void Skeleton::outputDebugBoneNames()
{
    for (auto bones : m_boneNameMap)
    {
        OutputDebugStringA(("\nName: " + bones.first + "\nNr: " + std::to_string(bones.second.nr)).c_str());
        OutputDebugStringA(("\n\nChildren:\n"));
        for (int i = 0; i < bones.second.childrenNames.size(); i++)
        {
            OutputDebugStringA(("\nName: " + bones.second.childrenNames[i] + "\nNr: " + std::to_string(bones.second.childrenNrs[i])).c_str());
        }
        OutputDebugStringA(("\nParent:\n"));
        OutputDebugStringA(("\nName: " + bones.second.parentName + "\nNr: " + std::to_string(bones.second.parentNr)).c_str());
        OutputDebugStringA(("\nIsRoot?:\n"));
        if (bones.second.root)
        {
            OutputDebugStringA(("Yes.\n"));
        }
        else
        {
            OutputDebugStringA(("No.\n"));
        }
        OutputDebugStringA(("\nIsLeaf?:\n"));
        if (bones.second.leaf)
        {
            OutputDebugStringA(("Yes.\n"));
        }
        else
        {
            OutputDebugStringA(("No.\n"));
        }
    }
}

Skeleton* Skeleton::combineTracks(Skeleton* trackB, std::string breakingPoint, std::string trackName)
{
   
    //Get the animation from the breaking point in one track and replace the other track with it.
    //Get jointchain.
    std::string trackType = getTrackType();
    std::string trackType2 = trackB->getTrackType();
    std::vector<std::string> chain;
    findChildren(breakingPoint, chain);
    Skeleton* combinedTrack = new Skeleton(trackName);
    combinedTrack->setTimeKeyCountAndOffsets(m_fps, m_trackLength, m_offsetMatricesNameMap);
    
    for (auto bone : chain)
    {
        combinedTrack->setBone(trackB->getBoneMap()[bone], trackB->getBoneKeyNameMap()[bone]);
    }

    for (auto bone : m_boneNameMap)
    {
        if (std::find(chain.begin(), chain.end(), bone.first) == chain.end())
        {
            combinedTrack->setBone(m_boneNameMap[bone.first], m_boneKeysNameMap[bone.first]);
        }
       
    }
    return combinedTrack;
}

void Skeleton::play(Skeleton* trackB, float dt, float t, bool once, float scale)
{
    g_done = false;
    m_useBlendBones = true;
    if (once)
    {
        
        float time = dt;
        m_time += time * scale;
        float frameCount = m_time * m_fps;
        if (frameCount <= (float)m_nrOfKeyFrames)
        {

            play(m_identityMatrix, m_time, m_boneNrMap[0].name);
            trackB->play(m_identityMatrix, m_time, trackB->m_boneNrMap[0].name);
            play(trackB, m_identityMatrix, t, m_boneNrMap[0].name);
        }
        else
        {
            g_done = true;
            m_time = 0;
        }

    }
    else
    {
       
        float time = dt;
        m_time += time * scale;
        play(m_identityMatrix,m_time, m_boneNrMap[0].name);
        trackB->play(m_identityMatrix, m_time, trackB->m_boneNrMap[0].name);
        play(trackB, m_identityMatrix, t, m_boneNrMap[0].name);
    }
    
   
}

Matrix& Skeleton::lerp(float time, std::string currentBone)
{
    assert(!isnan(time));
   
    m_nrOfKeyFrames = (short)m_boneKeysNameMap[currentBone].size();

    float frameCount = time * m_fps;

    float currentFrame = fmodf(frameCount, (float)m_nrOfKeyFrames);

    short firstIndex = 0;
    short secondIndex = 1;


    for (short index = 0; index < m_nrOfKeyFrames - 1; index++)
    {
        if (currentFrame < (float)index)
        {
            firstIndex = index;
            secondIndex = index + 1;
            break;
        }
    }


    float t = (currentFrame - (float)firstIndex) / ((float)secondIndex - (float)firstIndex);

    if (t < 0.0f)
        t = 0.0f;
    if (t > 1)
        t = 1.0f;
    assert(t >= 0.0f && t <= 1.0f);

    m_s = Vector3::Lerp(m_boneKeysNameMap[currentBone][firstIndex].s, m_boneKeysNameMap[currentBone][secondIndex].s, t);

    m_r = Quaternion::Slerp(m_boneKeysNameMap[currentBone][firstIndex].r, m_boneKeysNameMap[currentBone][secondIndex].r, t);

    m_t = Vector3::Lerp(m_boneKeysNameMap[currentBone][firstIndex].t, m_boneKeysNameMap[currentBone][secondIndex].t, t);

    m_lerpedTransform = XMMatrixScalingFromVector(m_s) * XMMatrixRotationQuaternion(m_r) * XMMatrixTranslationFromVector(m_t);
    BONE_KEY tempKey;
    tempKey.r = m_r;
    tempKey.s = m_s;
    tempKey.t = m_t;
    m_tempKeyFrameData.erase(currentBone);
    m_tempKeyFrameData.insert({ currentBone, tempKey });
    return m_lerpedTransform;
}

void Skeleton::play(Matrix& parent, float time, std::string currentBone)
{
    Matrix toRoot = lerp(time, currentBone) * parent;


    m_bones[m_boneNameMap[currentBone].nr] = (m_offsetMatricesNameMap[currentBone] * toRoot).Transpose();

    if (m_useTransform)
    {
        
        m_transforms[m_boneNameMap[currentBone].nr]->setWorldImmediately((m_offsetMatricesNameMap[currentBone] * toRoot));
       
    }

    for (unsigned short child = 0; child < (unsigned short)m_boneNameMap[currentBone].childrenNames.size(); child++)
    {

        play(toRoot, time, m_boneNameMap[currentBone].childrenNames[child]);

    }
}

void Skeleton::play(Skeleton* trackB, Matrix& parent, float t, std::string currentBone)
{
    Vector3 s = Vector3::Lerp(m_tempKeyFrameData[currentBone].s, trackB->m_tempKeyFrameData[currentBone].s, t);

    Quaternion r = Quaternion::Slerp(m_tempKeyFrameData[currentBone].r, trackB->m_tempKeyFrameData[currentBone].r, t);

    Vector3 trans = Vector3::Lerp(m_tempKeyFrameData[currentBone].t, trackB->m_tempKeyFrameData[currentBone].t, t);

    m_lerpedTransform = XMMatrixScalingFromVector(s) * XMMatrixRotationQuaternion(r) * XMMatrixTranslationFromVector(trans);

    Matrix toRoot = m_lerpedTransform * parent;

    m_bonesBlend[m_boneNameMap[currentBone].nr] = (m_offsetMatricesNameMap[currentBone] * toRoot).Transpose();
    
    if (m_useTransform)
    {
        m_transforms[m_boneNameMap[currentBone].nr]->setWorldImmediately((m_offsetMatricesNameMap[currentBone] * toRoot));
    }


    for (unsigned short child = 0; child < (unsigned short)m_boneNameMap[currentBone].childrenNames.size(); child++)
    {

        play(trackB, toRoot, t, m_boneNameMap[currentBone].childrenNames[child]);

    }
}



void Skeleton::findChildren(std::string boneName, std::vector<std::string>& chain)
{
    for (auto child : m_boneNameMap[boneName].childrenNames)
    {
        chain.push_back(child);
        findChildren(child, chain);
    }
}

Skeleton::Skeleton(std::string trackType, ANIMATION_TYPE animationType)
{
    
	m_bones = new XMFLOAT4X4[MAX_BONES];
    m_bonesBlend = new XMFLOAT4X4[MAX_BONES];
    m_transforms = new Transform*[MAX_BONES];
	for (UINT i = 0; i < MAX_BONES; i++)
	{
		m_bones[i] = m_identityMatrix;
        m_bonesBlend[i] = m_identityMatrix;
        m_transforms[i] = new Transform;
	}
    setTrackType(trackType);
    setAnimationType(animationType);
    
}

Skeleton::~Skeleton()
{
	RELEASE_ARR(m_bones);
    RELEASE_ARR(m_bonesBlend);
    for (int i = 0; i < MAX_BONES; i++)
    {
        RELEASE(m_transforms[i]);
    }
    RELEASE_ARR(m_transforms);
}

void Skeleton::setBone(const BONE& bone, const std::vector<BONE_KEY>& keys)
{
    
    m_boneNameMap.insert({ bone.name, bone });
    m_boneNrMap.insert({ bone.nr, bone });
    m_boneKeysNameMap.insert({ bone.name,keys });
    m_nrOfKeyFrames = (short)keys.size();
    if (bone.leaf)
        m_leaves.push_back(bone.name);
}
