
BASE_QZMQ    = ../Library/zmq

message("BASE_QZMQ is " $${BASE_QZMQ})
INCLUDEPATH    += $${BASE_QZMQ}/
INCLUDEPATH += ./connect/
#HEADERS        += $${BASE_QSSCONFIG}/ssconfig.hpp

LIBS += -L$${BASE_QZMQ}/ -lzmq
