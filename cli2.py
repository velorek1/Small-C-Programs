from ctypes import wintypes
import os,sys
from ctypes import *
import sys
import _thread
import time
from msvcrt import getch  # try to import Windows version
from msvcrt import kbhit  # try to import Windows version

opciones = ["1. Ejecturar Programa", "2. Introducir datos", "3. Borrar datos", "0. Salir"]
selector = 0
 
def keypress():
    char = getch()
    return char

STD_OUTPUT_HANDLE = -11
STD_INPUT_HANDLE = -10
ENABLE_LINE_INPUT = 2
ENABLE_ECHO_INPUT = 4
 
class COORD(Structure):
    pass
 
COORD._fields_ = [("X", c_short), ("Y", c_short)]
 
def print_at(r, c, s):
    h = windll.kernel32.GetStdHandle(STD_OUTPUT_HANDLE)
    windll.kernel32.SetConsoleCursorPosition(h, COORD(c, r))
 
    c = s.encode("windows-1252")
    windll.kernel32.WriteConsoleA(h, c_char_p(c), len(c), None, None)
 


windll.Kernel32.GetStdHandle.restype = c_ulong
h = windll.Kernel32.GetStdHandle(c_ulong(0xfffffff5))

def get_windows_terminal():
    from ctypes import windll, create_string_buffer
    h = windll.kernel32.GetStdHandle(-12)
    csbi = create_string_buffer(22)
    res = windll.kernel32.GetConsoleScreenBufferInfo(h, csbi)
 
    #return default size if actual size can't be determined
    if not res: return 80, 25 
 
    import struct
    (bufx, bufy, curx, cury, wattr, left, top, right, bottom, maxx, maxy)\
    = struct.unpack("hhhhHhhhhhh", csbi.raw)
    width = right - left + 1
    height = bottom - top + 1
 
    return width,height
#Default CMD colour = 7
def color(colour,backc):
	wColor = ((backc & 0x0F) << 4) + (colour & 0x0F)
	windll.Kernel32.SetConsoleTextAttribute(h, wColor)
 
def window(x1,y1,x2,y2, col):
	i=x1
	j=y1
	ancho = get_windows_terminal()[0]
	color(col,col)
	alto = get_windows_terminal()[1]
	while(j<y2):
		while (i<x2):
			print_at(j,i,' ')
			i=i+1
		j = j + 1
		i= x1

def linea(x1,y1,x2, col1,col2):
	i=x1
	j=y1
	ancho = get_windows_terminal()[0]
	color(col1,col2)
	alto = get_windows_terminal()[1]
	while (i<x2):
		print_at(j,i,chr(196))
		i=i+1
	
def displayMenu(selector):
	x=0
	i=0
	global opciones
	for x in opciones:
		if (i == selector):
			color(15,3) 
		else:
			color(0,15)
		print_at(7+i, 11, opciones[i])
		i = i + 1

def main():
	global selector
	char = ""
	color(15,1)
	os.system("cls")
	print_at(1,1, ">> CLI: Menu Demo")
	linea(0,get_windows_terminal()[1]-1,get_windows_terminal()[0], 15,1)
	print_at(get_windows_terminal()[1],0, "ESC -> ESCAPE PARA SALIR")	
	linea(1,2,20, 15,1)
	window(10,6,35,15,0)
	window(9,5,34,14,15)
	color(0,7)
	displayMenu(0)
	print("")
	color(7,0)
	cursorKey = False;
	while True:
		if kbhit():
			char = keypress()
			color(0,7)
		if char == b'\r': 
			#INTRO
			color (1,1)
			print_at(16,10,"                                        ")
			color (0,7)
			print_at(16,10,"Has seleccionado: " + opciones[selector])
			char = -1
		if char == b'\x1b':
			#ESCAPE
			break
		if char == b'\xe0':
			#Marca de cursor 
			cursorKey = True		
		if char == b'P' and cursorKey == True:
			#UP-cursor key
			char = -1
			cursorKey = False
			#if char == b'\K':
			if (selector < len(opciones)-1):
				selector = selector + 1
			displayMenu(selector)
			char = -1
		if char == b'H' and cursorKey == True:
			#DOWN-cursor key
			if (selector > 0):
				selector = selector - 1
			displayMenu(selector)
			char = -1
	#Limpiar la consola		
	color(7,0)
	os.system("cls")

#Empieza en main
if __name__ == "__main__":
    main()

