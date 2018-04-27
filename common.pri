

# Output Directory Base

message("LibraryUse is " $${LibraryUse})


DESTDIR_BASE      = ../../bin
INCLUDEPATH +=  ../Library


include( Library/libzmq.pri)

# Note the names are case sensitive...
contains( LibraryUse, libzmq ){
}
# xml
include( Library/tinyxml.pri)


contains( LibraryUse, libGPS)
{
    message("the libs contain the libGPS")
#    include (Library/gps2local.pri)
}

contains(LibraryUse, libssconfig)
{
    message("the libs contain the libssconfig")
    include (Library/ssconfig.pri)
}

contains( LibraryUse, qcustomplot ){
}

    message("the libs contain the libqcustom")
    include( Library/qcustomplot.pri )

contains( LibraryUse, libformat ){
#    include( Library/libformat.pri )
}


#conrtain the ompl lib
include (Library/ompl.pri)

#是否包含Boost库
contains(LibraryUse,libBoost)
{
    message("the libs contain the libboost")
    include(Library/boostlib.pri)
}

#Debug{
#    TARGET = $${TARGET}d
#    DESTDIR_BASE = $${DESTDIR_BASE}/Debug
#}
#Release{
#    DESTDIR_BASE = $${DESTDIR_BASE}/Release
#}

#message($${TARGET}  "- Include "    $${INCLUDEPATH})
#message($${TARGET}  "- Linking "    $${LIBS})
#message($${TARGET}  "- SourceD "    $${PWD})
#message($${TARGET}  "- OutputD "    $${OUT_PWD})
#message($${TARGET}  "- DestDir "    $${DESTDIR_BASE})
#message($${TARGET} "- SOURCESs" $${SOURCES})

#DEFINES += TIXML_USE_STL

