#pragma once
#include <sl/Camera.hpp>

/// <summary>
/// class to keep track of robot position using ZED sdk
/// </summary>
class robotpos
{
private:
    sl::float3 trans;
    sl::Camera zed;
    sl::Pose camera_path;
    sl::POSITIONAL_TRACKING_STATE tracking_state;
    sl::InitParameters init_parameters;
    sl::PositionalTrackingParameters positional_tracking_param;
    sl::RuntimeParameters rt;
    sl::Transform transform_;
    sl::Resolution respc;
    sl::Mat pc;
    bool m_open;
    bool* m_issafe;
public:
    /// <summary>
    /// constructor
    /// </summary>
    /// <param name="err">pointer to error variable</param>
    robotpos(int* err, bool* issafe);
    ~robotpos();
    /// <summary>
    /// updates the pose at ref
    /// </summary>
    /// <param name="ref">pointer to pose that should be updated</param>
    void updatepos(float* ref);
};