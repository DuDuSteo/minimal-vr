#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <openvr.h>

std::string GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = nullptr) 
{
	uint32_t requiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if( requiredBufferLen == 0 )
		return "";

	char *pchBuffer = new char[requiredBufferLen];
	requiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice,prop,pchBuffer,requiredBufferLen,peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;

	return sResult;
}

std::string GetTrackedDeviceClassString(vr::ETrackedDeviceClass td_class) {

	std::string str_td_class = "Unknown class";

	switch (td_class)
	{
	case vr::TrackedDeviceClass_Invalid:			// = 0, the ID was not valid.
		str_td_class = "invalid";
		break;
	case vr::TrackedDeviceClass_HMD:				// = 1, Head-Mounted Displays
		str_td_class = "hmd";
		break;
	case vr::TrackedDeviceClass_Controller:			// = 2, Tracked controllers
		str_td_class = "controller";
		break;
	case vr::TrackedDeviceClass_GenericTracker:		// = 3, Generic trackers, similar to controllers
		str_td_class = "generic tracker";
		break;
	case vr::TrackedDeviceClass_TrackingReference:	// = 4, Camera and base stations that serve as tracking reference points
		str_td_class = "base station";
		break;
	case vr::TrackedDeviceClass_DisplayRedirect:	// = 5, Accessories that aren't necessarily tracked themselves, but may redirect video output from other tracked devices
		str_td_class = "display redirect";
		break;
	}

	return str_td_class;
}
  
class Application {
public:
    void run() {
        initWindow();
        initVulkan();
        initHMD();
        mainLoop();
        cleanup();
    }
private:
    GLFWwindow* window;

    vr::IVRSystem* vr_context;
    vr::TrackedDevicePose_t tracked_device_pose[vr::k_unMaxTrackedDeviceCount];

    std::string driver_name, driver_serial;
    std::string tracked_device_type[vr::k_unMaxTrackedDeviceCount];

    void initWindow() {
        if (!glfwInit())
            return;

        window = glfwCreateWindow(640, 480, "Minimal VR", nullptr, nullptr);
    }
    void initVulkan() {

    }
    int initHMD() {
        if (vr::VR_IsHmdPresent()) {
            std::cout << "An HMD was successfully found in the system" << std::endl;

            if (vr::VR_IsRuntimeInstalled()) {
                // const char* runtime_path = vr::VR_GetRuntimePath();
                // std::cout << "Runtime correctly installed at '" << runtime_path << "'" << std::endl;
            }
            else {
                std::cout << "Runtime was not found, quitting app" << std::endl;
                // return -1;
            }
        }
        else {
            std::cout << "No HMD was found in the system, quitting app" << std::endl;
            // return -1;
        }

        // Load the SteamVR Runtime
        vr::HmdError err;
        vr_context = vr::VR_Init(&err,vr::EVRApplicationType::VRApplication_Scene);
        vr_context == NULL ? std::cout << "Error while initializing SteamVR runtime. Error code is " << vr::VR_GetVRInitErrorAsSymbol(err) << std::endl : std::cout << "SteamVR runtime successfully initialized" << std::endl;

        // Obtain some basic information given by the runtime
        int base_stations_count = 0;
        for (uint32_t td=vr::k_unTrackedDeviceIndex_Hmd; td<vr::k_unMaxTrackedDeviceCount; td++) {

            if (vr_context->IsTrackedDeviceConnected(td)) {
                vr::ETrackedDeviceClass tracked_device_class = vr_context->GetTrackedDeviceClass(td);

                std::string td_type = GetTrackedDeviceClassString(tracked_device_class);
                tracked_device_type[td] = td_type;

                std::cout << "Tracking device " << td << " is connected " << std::endl;
                std::cout << "  Device type: " << td_type << ". Name: " << GetTrackedDeviceString(vr_context,td,vr::Prop_TrackingSystemName_String) << std::endl;

                if (tracked_device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_TrackingReference) base_stations_count++;

                if (td == vr::k_unTrackedDeviceIndex_Hmd) {
                    // Fill variables used for obtaining the device name and serial ID (used later for naming the SDL window)
                    driver_name = GetTrackedDeviceString(vr_context,vr::k_unTrackedDeviceIndex_Hmd,vr::Prop_TrackingSystemName_String);
                    driver_serial = GetTrackedDeviceString(vr_context,vr::k_unTrackedDeviceIndex_Hmd,vr::Prop_SerialNumber_String);
                }
            }
            else
                std::cout << "Tracking device " << td << " not connected" << std::endl;
        }

        // Check whether both base stations are found, not mandatory but just in case...
        if (base_stations_count < 2) {
            std::cout << "There was a problem indentifying the base stations, please check they are powered on" << std::endl;

            // return -1;
        }

        std::cout << "Driver name: " + driver_name << std::endl;
        std::cout << "Driver serial ID: " + driver_serial << std::endl;

        return 0;
    }
    void mainLoop() {
        while(!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            drawFrame();
        }
    }
    void drawFrame() {

    }
    void cleanup() {
        vr::VR_Shutdown();
    }
};