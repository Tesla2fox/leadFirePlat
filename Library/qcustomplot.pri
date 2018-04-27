
BASE_QCUSTOM    = ../Library/QCustomPlot

message("BASE_QCUSTOM is " $${BASE_QCUSTOM})
QT             += printsupport
INCLUDEPATH    += $${BASE_QCUSTOM}/
HEADERS        += $${BASE_QCUSTOM}/qcustomplot.h
SOURCES        += $${BASE_QCUSTOM}/qcustomplot.cpp

