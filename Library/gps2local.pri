
#GPS2LOCAL_LIB
BASE_QGPS2LOCAL    = ../Library/GPS2local

message("BASE_QGPS2LOCAL is " $${BASE_QGPS2LOCAL})
INCLUDEPATH    += $${BASE_QGPS2LOCAL}/
HEADERS        += $${BASE_QGPS2LOCAL}/gps2local.h


win32{
message("window environment ")
LIBS  += -L../Library/GPS2local -lgps2local
}

unix{
           message("unix environment" $${PWD})
LIBS +=  -"L$$PWD/GPS2local" -lgps2local
}
