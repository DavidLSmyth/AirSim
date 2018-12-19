import setup_path 
import airsim

client = airsim.MultirotorClient()
client.confirmConnection()
client.enableApiControl(True)

client.armDisarm(True)
#client.takeoffAsync().join()

#client.showPlannedWaypoints(5690, -100, 120, 6500, -300, 180, thickness=10, lifetime=30, debug_line_color='red')
#client.showPlannedWaypoints(6500, -300, 180, 6800, -50, 300, thickness=10, lifetime=30, debug_line_color='red')
#client.showPlannedWaypoints(6800, -50, 300, 5500, -800, 350, thickness=10, lifetime=30, debug_line_color='red')

print("Detected radiation reading: ", client.getRadSensorData("Drone1"))
client.simShowPawnPath(True, 20, 8)
client.takeoffAsync().join()
client.simShowPawnPath(False, 3, 8)
client.moveToZAsync(20, 3)
print("Detected radiation reading drone1: ", client.getRadSensorData("Drone1"))
print('Getting radiation sensor data: \n\n\n')

print("Detected radiation reading drone 2: ", client.getRadSensorData("Drone2"))
print('\n\n\n')
client.moveToPositionAsync(20, -30, -18,4, vehicle_name = 'Drone1').join()
print("Detected radiation reading: ", client.getRadSensorData("Drone1"))

client.armDisarm(False)
client.enableApiControl(False)
