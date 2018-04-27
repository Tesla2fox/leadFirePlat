
BASE_OMPL    = /usr/local/include

message("BASE_OMPL is " $${BASE_OMPL})
INCLUDEPATH    += $${BASE_OMPL}/
#HEADERS        += $${BASE_QSSCONFIG}/ssconfig.hpp

LIBS +=  -"L$$PWD/OMPL" -lompl
