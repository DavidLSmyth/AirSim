import setup_path 
import airsim

client = airsim.MultirotorClient()
client.confirmConnection()
client.enableApiControl(True)

client.armDisarm(True)

client.showPlannedWaypoints(10, 20, 200, 10, 20, 500, thickness=50, lifetime=10, debug_line_color='red')
client.armDisarm(False)
client.enableApiControl(False)
