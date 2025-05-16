import requests
import cv2
import numpy as np
import time

url = "http://192.168.32.1/frame"
output_path = "output.avi"
fps = 10

first_frame = None
writer = None

while True:
	try:
		response = requests.get(url, timeout = 2)
		response.raise_for_status()
		# print("Status code: ", response.status_code)
		# print("Headers:", response.headers)

		image_array = np.frombuffer(response.content, dtype = np.uint8)
		frame = cv2.imdecode(image_array, cv2.IMREAD_COLOR)

		if frame is None:
			print("Error: Could not decode image.")
			continue
		if writer is None:
			height, width, _ = frame.shape
			fourcc = cv2.VideoWriter_fourcc(*"XVID")
			writer = cv2.VideoWriter(output_path, fourcc, fps, (width, height))
		
		writer.write(frame)
		cv2.imshow("Live", frame)

	except requests.RequestException as e:
		print("Request failed: ", e)
		break

	if cv2.waitKey(1) & 0xFF == ord('q'):
		break
	
	time.sleep(1.0 / fps)

cv2.destroyAllWindows()