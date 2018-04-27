
BASE_QXML    = ../Library/xml

message("BASE_QXML is " $${BASE_QXML})
INCLUDEPATH    += $${BASE_QXML}/
HEADERS        += $${BASE_QXML}/tinyxml.h \
                $${BASE_QXML}/tinystr.h
SOURCES +=      $${BASE_QXML}/tinyxml.cpp \
                $${BASE_QXML}/tinystr.cpp\
                $${BASE_QXML}/tinyxmlerror.cpp \
                $${BASE_QXML}/tinyxmlparser.cpp
