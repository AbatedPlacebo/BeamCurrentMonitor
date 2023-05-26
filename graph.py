import time
import matplotlib.pyplot as plt
import subprocess
import numpy as np

plt.ion()
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_ylim([-2048,2048])
x = np.arange(0,65536)
y = np.zeros(65536) 
line1, = ax.plot(x, y, 'bo')
#subprocess.run(["./main", "192.168.1.9", "writeregs", "0", "2" "writeregs", "2", "1", "startgen"]) 
while True:
#	subprocess.check_output(["./main", "192.168.1.9", "stop", "start", "readbuffer", "0", "127", "-file", "signal.txt"]) 
	f = open("wave.txt", "r")
	data = f.read()
	splitted = data.split("\n")
	splitted.pop()
	list_data = list(map(int, splitted))
	data = np.array(list_data)
	ax.set_ylim([min(data),max(data)])
	line1.set_ydata(data)
	fig.canvas.draw()
	fig.canvas.flush_events()
	time.sleep(1)

