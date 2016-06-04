#include <jaus/mobility/sensors/GlobalPoseSensor.h>
#include <jaus/mobility/sensors/LocalPoseSensor.h>
#include <jaus/mobility/sensors/VelocityStateSensor.h>
#include <jaus/mobility/drivers/LocalWaypointListDriver.h>
#include <jaus/core/transport/JUDP.h>
//#include <jaus/core/transport/jtcpclient.h>
#include <jaus/core/Component.h>
#include <cxutils/Keyboard.h>
#include <cxutils/math/CxMath.h>
#include <cxutils/networking/IP4Address.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>

#include "ros/ros.h"
#include <nav_msgs/Odometry.h>

const double PI = 3.141592653589793238463;

JAUS::UShort gSubsystemID   = 152;    // ID of our subsystem to use.
JAUS::Byte gNodeID          = 1;      // ID of our node to use.
JAUS::Byte gComponentID     = 1;      // ID of the our component.

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Entry point of jaus_challenge_2010.cpp
///
///   This program demonstrates everything required to complete the JAUS
///   Interoperability Challenge for 2010.  In this program a simulated robot
///   is created which can drive to local waypoints.  This program has
///   run against the JAUS Validation Tool (JVT) and the OCP of the
///   JAUS Interopability Challange during the Autonomous Surface Vehicle
///   Competition, passing all requirements.
///
///   All you have to do is integrate on your own robot, providing real
///   sensor values and generating real control of your bot!
///
////////////////////////////////////////////////////////////////////////////////////

double latitude;
double longitude; 
double altitude;
double positionRMS;
double roll;
double pitch;
double yaw;
double velocityX;

// class JAUSCallback: public JAUS::Discovery::Callback
// {
// public:
//     JAUSCallback() 
//     {
//     }
//     ~JAUSCallback()
//     {
//     }
//     virtual void ProcessMessage(const JAUS::Message* message) 
//     {
//         if(message->GetMessageCode() == JAUS::REPORT_HEARTBEAT_PULSE)
//         {
//             if 
//         }
//     }
// }

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg) 
{
    latitude = msg->pose.pose.position.x;
    longitude = msg->pose.pose.position.y;
    altitude = msg->pose.pose.position.z;
    positionRMS = 0.1;
    roll = 0;
    pitch = (-1) * atan2(msg->twist.twist.linear.z, sqrt(pow(msg->twist.twist.linear.x,2) +  
                                                         pow(msg->twist.twist.linear.y, 2)));
    yaw = atan2(msg->twist.twist.linear.y, msg->twist.twist.linear.x) - (PI / 2);
    velocityX = msg->twist.twist.linear.x;
}

int main(int argc, char* argv[])
{
    JAUS::Component component;
    
    ros::init(argc, argv, "bridge");

    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("odom", 1000, odomCallback);

    ros::Rate loop_rate(10);
    //component.AddService(new JAUS::JTCPClient());

    // Setting timeout for control to 0 (disables timeout of control).
    // This is done because the JVT and OCP do not query for the timeout period
    // and may not send messages to re-acquire/maintain control within the
    // normal 2 second timeout window.
    component.AccessControlService()->SetTimeoutPeriod(0);

    // Add the services we want/need for our component. By
    // default, the component object already has the Core Service set
    // (e.g. Discovery, Events, Access Control, Management).
    // The Discovery service automatically handles finding other JAUS
    // components, so you do not need to generate any Query Messages
    // (e.g. Query Identification) yourself, because Discovery does it
    // for you.

    ///   TO DO
    // Services to implement
    // 
    // Liveness ?
    // Waypoint Driver ?
    // Primitive Driver ?

    // Services that require config 
    // Access control ? 
    // Management - ROS connection for Emergency message
    // Local pose - resetting to 0 ? Ros related 
    // Require remote control if unable


    // In this test program, we are making a simulated robot, which
    // requires the following mobility services.

    // JAUS::Liveness* livenessService = new JAUS::Liveness();
    // component.AddService(livenessService);

    // JAUS::Waypoint* waypointService = new JAUS::Waypoint();
    // component.AddService(waypointService);

    JAUS::GlobalPoseSensor* globalPoseSensor = new JAUS::GlobalPoseSensor();
    globalPoseSensor->SetSensorUpdateRate(25);      // Updates at 25 Hz (used for periodic events).
    component.AddService(globalPoseSensor);

    JAUS::LocalPoseSensor* localPoseSensor = new JAUS::LocalPoseSensor();
    localPoseSensor->SetSensorUpdateRate(25);       // Updates at 25 Hz (used for periodic events).
    component.AddService(localPoseSensor);

    JAUS::VelocityStateSensor* velocityStateSensor = new JAUS::VelocityStateSensor();
    velocityStateSensor->SetSensorUpdateRate(25);   // Updates at 25 Hz (used for periodic events).
    component.AddService(velocityStateSensor);

    // Need a List Manager Service for Local Waypoint List Driver.  You can
    // ignore this service for the Underwater Vehicle Competition.
    component.AddService(new JAUS::ListManager());
    JAUS::LocalWaypointListDriver* localWaypointListDriver = new JAUS::LocalWaypointListDriver();
    component.AddService(localWaypointListDriver);

    // Set Vehicle Identification Information.  Available options for the
    // parameters are JAUS::Subsystem::OCU and JAUS::Subsystem::Vehicle.  Since
    // we are not an OCU, we use Vehicle.  Finally, the string represents the type
    // of robot you have (e.g. Segway RMP, XUV).  This is different than the
    // name of your vehicle, but you can use that if you want.  Your subsystem number
    // is your unique identifier.
    component.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
     "JROS");

    // Initialize component with a given ID.  Remember, you must
    // add all your services before you initialize the component, because you 
    // cannot add them after initialization.  All services will be deleted by
    // the component on program exit.
    if(component.Initialize(JAUS::Address(gSubsystemID, gNodeID, gComponentID)) == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }

    // Set state to Standby since we have initialized OK.
    component.ManagementService()->SetStatus(JAUS::Management::Status::Standby);

    // Now that we are initialized, lets create a fixed connection to the
    // JVT for testing.  Since the JVT and the OCP of the JAUS 
    // Interoperability Challenge do not support multicast for Discovery, you
    // must make a direct connection to them.
    JAUS::JUDP* transportService = NULL;
    transportService = (JAUS::JUDP*)component.TransportService();


    // Create connection to JAUS Validation Tool (JVT)
    JAUS::Address::Set discoveredSubsystems;
    discoveryService->GetSubsystems(discoveredSubsystems);

    JAUS::JUDP::ListenForSubsystems(discoveredSubsystems);
    transportService->AddNetworkConnection(JAUS::Address(90, 1, 1),
                                           "239.255.0.1",
                                           3794);
    // Create connection to OCP for the JAUS Interoperability Challenge.
    // transportService->AddConnection("192.168.1.42", JAUS::Address(42, 1, 1));
    

    // Set an initial global pose.
    JAUS::GlobalPose globalPose;
    
    // Save the data to the service.

    // You can set local pose data using global pose
    // as it is automatically converted to local pose data for you.
    // How convenient!

    // Set an initial velocity state.
    JAUS::VelocityState velocityState;

    JAUS::Time::Stamp printTimeMs = 0;

    // double timeDiff = 0.33; // Used for simulation of robot physics.

    while(ros::ok() && CxUtils::GetChar() != 27 && 
      component.ManagementService()->GetStatus() != JAUS::Management::Status::Shutdown)
    {

        // globalPose.SetLatitud(latitude);
        // globalPose.SetLongitude(longitude);
        // globalPose.SetAltitude(altitude);
        // globalPose.SetPositionRMS(positionRMS);
        // globalPose.SetRoll(roll);
        // globalPose.SetPitch(pitch);
        // globalPose.SetYaw(yaw);

        globalPose.SetLatitude(32.703356);
        globalPose.SetLongitude(-117.253919);
        globalPose.SetAltitude(300);
        globalPose.SetPositionRMS(0.0);
        globalPose.SetRoll(0.0);
        globalPose.SetPitch(0.0);
        globalPose.SetYaw(CxUtils::CxToRadians(45));
        globalPose.SetAttitudeRMS(0.0);
        globalPose.SetTimeStamp(JAUS::Time::GetUtcTime());

        globalPoseSensor->SetGlobalPose(globalPose);
        localPoseSensor->SetLocalPose(globalPose);
        // desired thrust and steering 
        // to reach local waypoints.


        // velocityState.SetVelocityX(velocityX);
        // velocityState.SetYawRate();

        velocityState.SetVelocityX(0.0);
        velocityState.SetYawRate(0.0);
        velocityState.SetVelocityRMS(0.0);
        velocityState.SetTimeStamp(JAUS::Time::GetUtcTime());
        // Save the data to the service.
        velocityStateSensor->SetVelocityState(velocityState);

        // double linearDistance = timeDiff*thrust*0.025;
        // double linearVelocity = linearDistance/timeDiff;
        // double rotation = timeDiff*steering*CxUtils::CxToRadians(1);
        // double rotationRate = rotation/timeDiff;
        // double yaw = globalPose.GetYaw();

        // JAUS::Point3D localPosChange(linearDistance, 0, 0);        
        // localPosChange = localPosChange.Rotate(yaw, JAUS::Point3D::Z);

        // CxUtils::Wgs worldPositionWgs(globalPose.GetLatitude(),
        //   globalPose.GetLongitude(),
        //   globalPose.GetAltitude());
        // CxUtils::Utm worldPositionUtm(worldPositionWgs);

        // worldPositionUtm.mNorthing += localPosChange.mX;
        // worldPositionUtm.mEasting += localPosChange.mY;
        // yaw = CxUtils::Orientation::AddToAngle(yaw, rotation);
        // Convert to WGS
        // worldPositionWgs << worldPositionUtm;

        // Save newly calculated position and orientation.
        // globalPose.SetYaw(yaw);
        // globalPose.SetLatitude(worldPositionWgs.mLatitude);
        // globalPose.SetLongitude(worldPositionWgs.mLongitude);
        // globalPose.SetAltitude(worldPositionWgs.mElevation);
        // globalPose.SetTimeStamp(JAUS::Time(true));

        // velocityState.SetVelocityX(linearVelocity);
        // velocityState.SetYawRate(rotationRate);
        // velocityState.SetTimeStamp(JAUS::Time(true));

        // Save global pose information.
        // globalPoseSensor->SetGlobalPose(globalPose);
        // The Local Pose Sensor service supports converting
        // global pose data to local pose data.  It automatically
        // calculates offsets and relative changes for you.
        // localPoseSensor->SetLocalPose(globalPose);
        // Save velocity state information.
        // velocityStateSensor->SetVelocityState(velocityState);

        if(JAUS::Time::GetUtcTimeMs() - printTimeMs > 500)
        {
            // Print status of services.
            std::cout << "\n======================================================\n";
            component.AccessControlService()->PrintStatus(); std::cout << std::endl;
            component.ManagementService()->PrintStatus(); std::cout << std::endl;
            globalPoseSensor->PrintStatus(); std::cout << std::endl;
            localPoseSensor->PrintStatus(); std::cout << std::endl;
            velocityStateSensor->PrintStatus(); std::cout << std::endl;
            localWaypointListDriver->PrintStatus();
            printTimeMs = JAUS::Time::GetUtcTimeMs();
        }

        // Exit if escape key pressed.
        if(CxUtils::GetChar() == 27)
        {
            break;
        }

        // CxUtils::SleepMs((unsigned int)(timeDiff*1000.0));

        ros::spinOnce();

    }

    // Don't delete services, they are
    // deleted by the Component class.

    // Shutdown any components associated with our subsystem.
    
    component.Shutdown();

    return 0;
}
