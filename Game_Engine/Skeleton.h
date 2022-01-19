#pragma once
#ifndef SKELETON_H
#define SKELETON_H
#include "AnimationHandler.h"
#include "Transform.h"
class Skeleton :
    public AnimationHandler
{
private:
    std::unordered_map<std::string, BONE> m_boneNameMap;
    std::unordered_map<int, BONE> m_boneNrMap;
    std::unordered_map<std::string, std::vector<BONE_KEY>> m_boneKeysNameMap;
    std::unordered_map<std::string, Matrix> m_offsetMatricesNameMap;
    std::unordered_map<std::string, BONE_KEY> m_tempKeyFrameData;
    std::vector<std::string> m_leaves;
    float m_trackLength;
    short m_nrOfKeyFrames;
    Matrix m_lerpedTransform;
    Vector3 m_s;
    Vector3 m_t;
    Quaternion m_r;
    XMFLOAT4X4* m_bones = nullptr;
    XMFLOAT4X4* m_bonesBlend = nullptr;
    Matrix m_identityMatrix = XMMatrixIdentity();
    Matrix& lerp(float time, std::string currentBone);
    void play(Matrix& parent, float time, std::string currentBone);
    void play(Skeleton* trackB, Matrix& parent, float t, std::string currentBone);
    bool m_useBlendBones = false;
    Transform** m_transforms = nullptr;
    bool m_useTransform = false;
    void findChildren(std::string boneName, std::vector<std::string>& chain);
public:
    Skeleton(std::string trackType, ANIMATION_TYPE animationType = ANIMATION_TYPE::SKELETON);
    virtual ~Skeleton();
    void setBone(const BONE& bone, const std::vector<BONE_KEY>& keys);
    void setTimeKeyCountAndOffsets(float fps, float length, const std::unordered_map<std::string, Matrix>& offsets) { m_offsetMatricesNameMap = offsets;
    m_fps = fps; m_trackLength = length; }
    XMFLOAT4X4* getBones() { if (m_useBlendBones) { return m_bonesBlend; } else { return m_bones; } }
    float getFps() { return m_fps; }
    float getTrackLength() { return m_trackLength; } //in seconds
    void outputDebugBoneNames();
    void setUseTransform(bool use) { m_useTransform = use; }
    Transform* getTransforms(int index) { return m_transforms[index]; }
    std::unordered_map<std::string, BONE>& getBoneMap() { return m_boneNameMap; }
    std::unordered_map<std::string, std::vector<BONE_KEY>>& getBoneKeyNameMap() { return m_boneKeysNameMap; }
    Skeleton* combineTracks(Skeleton* trackB, std::string breakingPoint, std::string trackName); //creates a new track in heap.
    virtual void play(Skeleton* trackB, float dt, float t, bool once = false, float scale = 1.0f);
    // Inherited via AnimationHandler
    virtual void play(float dt, bool once = false, float scale = 1.0f) override;
    
};

#endif