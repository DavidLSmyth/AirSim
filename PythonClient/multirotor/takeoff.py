import setup_path 
import airsim

client = airsim.MultirotorClient()
client.confirmConnection()
client.enableApiControl(True)

client.armDisarm(True)

#client.showPlannedWaypoints(5690, -100, 120, 6500, -300, 180, thickness=10, lifetime=30, debug_line_color='red')
#client.showPlannedWaypoints(6500, -300, 180, 6800, -50, 300, thickness=10, lifetime=30, debug_line_color='red')
#client.showPlannedWaypoints(6800, -50, 300, 5500, -800, 350, thickness=10, lifetime=30, debug_line_color='red')



client.showPlannedWaypoints(0, 0, 0, 20, -30, -18, thickness=10, lifetime=30, debug_line_color='red')
client.showPlannedWaypoints(20, -30, -18, 40, -20, -30, thickness=10, lifetime=30, debug_line_color='red')
#client.showPlannedWaypoints(6800, -50, 300, 5500, -800, 350, thickness=10, lifetime=30, debug_line_color='red')

client.showPlannedPath(True, 3, 8)
client.takeoffAsync().join()
#showPath:'bool', debug_line_lifetime, debug_line_thickness, vehicle_name = ''):

print('moving')
client.moveToPositionAsync(20, -30, -18,4).join()
client.moveToPositionAsync(40, -20, -30,4).join()
client.armDisarm(False)
client.enableApiControl(False)
