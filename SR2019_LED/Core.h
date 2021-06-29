
/*

pi@192.168.0.10
p:strelnieks

wifi
pi@192.168.4.1
p:strelnieks

'screen' command on linux

screen -ls :list available screens
screen -r NAME :create screen
screen -r NAME :attach to screen
screen -d -r :attach to screen, detach others

Set up startup script:

chmod +x ~/software.out
sudo nano /etc/rc.local

-d -m Start screen in "detached" mode. This creates a new session but doesn't attach to it.
sudo su - pi -c "screen -dm -S pistartup ~/software.out"

RPI3/4 UART config: https://spellfoundry.com/2016/05/29/configuring-gpio-serial-port-raspbian-jessie-including-pi-3-4/
1)
	$ sudo systemctl stop serial-getty@ttyAMA0.service
	$ sudo systemctl disable serial-getty@ttyAMA0.service
	$ sudo systemctl stop serial-getty@ttyS0.service
	$ sudo systemctl disable serial-getty@ttyS0.service
2)
	$ sudo nano /boot/cmdline.txt
	remove "console=serial0,115200"
3)
	$ sudo nano /boot/config.txt
	add: "dtoverlay=pi3-miniuart-bt"
4)
	reboot
	ls -l /dev


	pgrep SR2019	List process id
	kill [id]		Kill process


*/

#pragma once

#include <cstdio>
#include <time.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <termios.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>
#include <locale>
#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <stdio.h>
#include <vector>
#include <dirent.h>
#include "HVector3.h"
#include "WingsStructures.h"
#include "WingKeyframesResource.h"
#include "Serial.h"
#include "Application.h"

using namespace std;

const std::string clearScreen = "\033[2J";
const std::string resetCursor = "\033[H";
const std::string editorLineCursor = "\033[4;2H";
const std::string colorGreen = "\033[0;32m";
const std::string colorRed = "\033[0;31m";
const std::string colorBlue = "\033[0;34m";
const std::string colorGrey = "\033[0;37m";
const std::string colorWhite = "\033[0;37m";

#define SH_FG_BLACK			"\033[0;30m"
#define SH_FG_BLUE           "\033[0;34m"
#define SH_FG_GREEN          "\033[0;32m"
#define SH_FG_CYAN           "\033[0;36m"
#define SH_FG_RED            "\033[0;31m"
#define SH_FG_PURPLE         "\033[0;35m"
#define SH_FG_YELLOW         "\033[0;33m"
#define SH_FG_LIGHT_GREY     "\033[0;37m"
#define SH_FG_DARK_GREY      "\033[1;30m"
#define SH_FG_LIGHT_BLUE     "\033[1;34m"
#define SH_FG_LIGHT_GREEN    "\033[1;32m"
#define SH_FG_LIGHT_CYAN     "\033[1;36m"
#define SH_FG_LIGHT_RED      "\033[1;31m"
#define SH_FG_LIGHT_PURPLE   "\033[1;35m"
#define SH_FG_LIGHT_YELLOW   "\033[1;33m"
#define SH_FG_WHITE          "\033[1;37m"

//// Background Colours ////
#define SH_BG_BLACK          "\033[0;40m"
#define SH_BG_BLUE           "\033[0;44m"
#define SH_BG_GREEN          "\033[0;42m"
#define SH_BG_CYAN           "\033[0;46m"
#define SH_BG_RED            "\033[0;41m"
#define SH_BG_PURPLE         "\033[0;45m"
#define SH_BG_YELLOW         "\033[0;43m"
#define SH_BG_LIGHT_GREY     "\033[0;47m"
#define SH_BG_DARK_GREY      "\033[1;40m"
#define SH_BG_LIGHT_BLUE     "\033[1;44m"
#define SH_BG_LIGHT_GREEN    "\033[1;42m"
#define SH_BG_LIGHT_CYAN     "\033[1;46m"
#define SH_BG_LIGHT_RED      "\033[1;41m"
#define SH_BG_LIGHT_PURPLE   "\033[1;45m"
#define SH_BG_LIGHT_YELLOW   "\033[1;43m"
#define SH_BG_WHITE          "\033[1;47m"