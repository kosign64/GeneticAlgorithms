TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LFLAGS += -pthread

SOURCES += \
        main.cpp

HEADERS += \
    geneticalgorithm.h \
    selection.h \
    mutation.h \
    display.h \
    crossover.h \
    prepopulation.h \
    organism.h \
    initialization.h \
    fitnesscaling.h \
    stoppingcriteria.h
