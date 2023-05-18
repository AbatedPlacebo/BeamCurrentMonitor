import time
import matplotlib.pyplot as plt
import subprocess
import numpy as np

plt.ion()
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_ylim([-2048,2048])
x = np.arange(0,65535)
y = np.zeros(65535) 
line1, = ax.plot(x, y, 'bo')
subprocess.run(["./main", "192.168.1.9", "writeregs", "0", "2" "writeregs", "2", "1"]) 
while True:
	subprocess.check_output(["./main", "192.168.1.9", "-S", "script.bcm" "-file", "signal.txt"]) 
	f = open("signal.txt", "r")
	data = f.read()
	list_data = list(map(int, data.split("\n")))
	data = np.array(list_data)
	ax.set_ylim([min(data),max(data)])
	line1.set_ydata(data)
	fig.canvas.draw()
	fig.canvas.flush_events()
	time.sleep(1)

