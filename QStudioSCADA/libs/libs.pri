!isEmpty(LIBS_PRI_INCLUDED):error("libs.pri already included")
LIBS_PRI_INCLUDED = 1

QT       += core gui widgets

LIBRARY_OUTPUT_PATH = ../../../build-QStudioSCADA/libs/lib

DESTDIR = $$LIBRARY_OUTPUT_PATH

CONFIG += debug_and_release



