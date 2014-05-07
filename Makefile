CC		= g++
C		= cpp

CFLAGS		= -g `Magick++-config --cppflags`
LFLAGS		= -g `Magick++-config --ldflags`

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lMagick++ -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -L /usr/lib64/ -lglut -lGLU -lGL -lMagick++ -lm
  endif
endif

HFILES 	= Vector.h Utility.h ray.h color.h sphere.h scene.h plane.h image.h light.h material.h collision.h
OFILES 	= Vector.o Utility.o ray.o color.o sphere.o scene.o plane.o image.o light.o material.o collision.o

PROJECT		= shady

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

Vector.o: Vector.${C} Vector.h Utility.h 
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.h
	${CC} $(CFLAGS) -c Utility.${C}

ray.o: ray.${C} ray.h Vector.h
	${CC} $(CFLAGS) -c ray.${C}

color.o: color.${C} color.h
	${CC} $(CFLAGS) -c color.${C}

image.o: image.${C} image.h color.h
	${CC} $(CFLAGS) -c image.${C}

sphere.o: sphere.${C} sphere.h Vector.h color.h ray.h material.h
	${CC} $(CFLAGS) -c sphere.${C}

plane.o: plane.${C} plane.h Vector.h color.h ray.h
	${CC} $(CFLAGS) -c plane.${C}

scene.o: scene.${C} scene.h sphere.h color.h ray.h plane.h light.h material.h collision.h
	${CC} $(CFLAGS) -c scene.${C}

material.o: material.${C} material.h color.h
	${CC} $(CFLAGS) -c material.${C}

collision.o: collision.${C} collision.h Vector.h color.h material.h
	${CC} $(CFLAGS) -c collision.${C}

light.o: light.${C} light.h Vector.h color.h
	${CC} $(CFLAGS) -c light.${C}

clean:
	rm core.*; rm *.o; rm *~; rm ${PROJECT}
