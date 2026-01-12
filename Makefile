all: compile link

compile:
	g++ -c main.cpp -IC:\SFML-2.4.2\include -DSFML_STATIC

link:
	g++ main.o -o Smart_Study -LC:\SFML-2.4.2\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype -ljpeg -lopengl32 -lgdi32 -lwinmm -mwindows -lsfml-main

clean:
	del /f /q Smart_Study.exe *.o
