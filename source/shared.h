//3DS and lib includes
#include <sf2d.h>
#include <sfil.h>
#include <sftd.h>
#include <3ds.h>

//Standard libs
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

//Net stuff
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

//Ogg format things
#include <ivorbiscodec.h>
#include <ivorbisfile.h>

//Util
#include "include/util.h"
#include "include/bubble.h"
#include "include/flask.h"

extern void displayError(const char * error);
extern bool romfsEnabled;
extern float delta;
extern bool channelList[24];
extern int getCurrentScreen();

//Class includes
#include "include/oggvorbis.h"