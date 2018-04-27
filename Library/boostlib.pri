
#-------------------------------------------------
#
# Project created by QtCreator 2017-08-27T11:06:34
#
#-------------------------------------------------

message("add the boost inDir")
win32{
message("the boost in the windows")

BASE_QBOOSTLIB    = D:\boost\boost_1_65_0

}
#unix
#{
#BASE_QBOOSTLIB= /usr/local/boost
#}

message("BASE_QBOOSTLIB is " $${BASE_QBOOSTLIB})
INCLUDEPATH    += $${BASE_QBOOSTLIB}

