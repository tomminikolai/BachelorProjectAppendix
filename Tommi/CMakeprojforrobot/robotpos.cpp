#include "robotpos.h"

robotpos::robotpos(int* err, bool* issafe)
    :respc(640, 360), pc(respc, sl::MAT_TYPE::F32_C4, sl::MEM::CPU), m_issafe(issafe)
{
    m_open = false;
    *err = 0;
    init_parameters.coordinate_units = sl::UNIT::METER;
    init_parameters.coordinate_system = sl::COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP;
    init_parameters.sdk_verbose = true;
    init_parameters.depth_mode = sl::DEPTH_MODE::NEURAL;
    auto returned_state = zed.open(init_parameters);
    if (returned_state != sl::ERROR_CODE::SUCCESS) {
        std::cout << "cam open fail" << std::endl;
        *err = 1;
        return;
    }
    positional_tracking_param.enable_area_memory = true;
    returned_state = zed.enablePositionalTracking(positional_tracking_param);
    if (returned_state != sl::ERROR_CODE::SUCCESS) {
        std::cout << "tracking enable fail" << std::endl;
        zed.close();
        *err = 1;
        return;
    }
    m_open = true;
    rt.confidence_threshold = 50;
    rt.texture_confidence_threshold = 100;
    rt.sensing_mode = sl::SENSING_MODE::FILL;
    transform_.setIdentity();
}

robotpos::~robotpos()
{
    if (m_open)
    {
        zed.disablePositionalTracking();
        zed.close();
    }
}

void robotpos::updatepos(float* ref)
{
    if (zed.grab(rt) == sl::ERROR_CODE::SUCCESS) {
        // Get the position of the camera in a fixed reference frame (the World Frame)
        zed.retrieveMeasure(pc, sl::MEASURE::XYZ, sl::MEM::CPU, respc);

        unsigned int safeval = 0;
        for (size_t i = 0; i < pc.getHeight(); i++)
        {
            for (size_t j = 0; j < pc.getWidth(); j++)
            {
                float testx = pc.getPtr<float>()[i * 640 * 4 + j * 4];
                float testy = pc.getPtr<float>()[i * 640 * 4 + j * 4 + 1];
                float testz = pc.getPtr<float>()[i * 640 * 4 + j * 4 + 2];
                if (((!std::isnan(testz)) && !(std::isinf(testz)) && testz > -0.500) || *(int*)&testz == 0xff800000)
                {
                    if ((!std::isnan(testx)) && !(std::isinf(testx)) && testx > -0.65 && testx < 0.185)
                    {
                        if ((!std::isnan(testy)) && !(std::isinf(testy)) && testy > -0.120 && testy < 0.030)
                        {
                            safeval += 1;
                        }
                    }
                }
            }
        }
        if (safeval < 1)
        {
            *m_issafe = true;
        }
        else
        {
            *m_issafe = false;
        }
        //std::cout << *m_issafe << std::endl;

        tracking_state = zed.getPosition(camera_path, sl::REFERENCE_FRAME::WORLD);
        if (tracking_state == sl::POSITIONAL_TRACKING_STATE::OK) {
            // Get rotation and translation and displays it
            transform_.setIdentity();
            transform_.tx = 0.06;
            transform_.tz = 0.063;
            camera_path.pose_data = sl::Transform::inverse(transform_) * camera_path.pose_data * transform_;
            ref[0] = camera_path.getEulerAngles().y;
            trans = camera_path.getTranslation();
            ref[1] = trans.x;
            ref[2] = trans.y;
            ref[3] = trans.z;
        }
        else
        {
            std::cout << "tracking not ok" << std::endl;
        }
    }
    else
    {
        sl::sleep_ms(1);
    }
}
