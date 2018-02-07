from PIL import Image


file = open("output.txt", "r")
size = file.readline().split(" ")

img = Image.new("RGB", (int(size[0]), int(size[1])))

pixel = img.load()

def pro(x):
	if (x < 0):
		return 0
	elif (x > 255):
		return 255
	return x
for i in range(int(size[0])):
	for j in range(int(size[1])):
		t = file.readline().split(" ")
		a1 = int(float(t[0])*255)
		a2 = int(float(t[1])*255)
		a3 = int(float(t[2])*255)
		#print(a1, " ", a2, " ", a3)
		pixel[i, j] = (pro(a1), pro(a2), pro(a3))
img.save("x.png")
img.show()