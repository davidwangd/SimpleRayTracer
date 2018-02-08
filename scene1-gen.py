file = open("scene1.test", "w")

def write(s):
	file.write(s + "\n")

write("size 640 480")
write("camera 0 0 6 0 0 -1 0 0.3 0 45")
write("directional 0 -0.5 0.4 0.8 0.8 0.8")
write("point 0 20 1 1 1 1")
write("maxdepth 5")

for x in [-1.02, 1.02]:
	for y in [-1.02, 1.02]:
		write("vertex %f 0 %f" % (x, y))

write("pushTransform")
size = 1
write("scale %d %d 1" % (size, size))
write("shininess 20")
write("ambient 0 0 0")
for x in range(20):
	for y in range(50):
		write("pushTransform")
		write("translate %d %d %d" % ((x - 10)*size*2, -3, -y*size*2))
		if ((x + y) % 2 == 1):
			write("emission 0.03 0.03 0.03")
			write("specular 0.7 0.7 0.7")
			write("diffuse 0.3 0.3 0.3")
		else:
			write("emission 0.1 0.1 0.1")
			write("specular 0.9 0.9 0.9")
			write("diffuse 0 0 0")
		write("tri 0 1 2")
		write("tri 2 1 3")
		write("popTransform")
		write("\n")

write("popTransform")

write("diffuse 0.5 0.0 0")
write("emission 0.1 0 0")
write("specular 0.7 0.00 0")
write("sphere %d %d %d 3" % (-4 * size, 2, -5 * size))

write("diffuse 0.0 0.0 0.5")
write("emission 0 0 0.2")
write("specular 0 0.0 0.7")
write("sphere %d %d %d 3" % (4 * size, 2, -5 * size))

file.close()