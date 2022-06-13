include(../HmiRunTime.pri)

TEMPLATE  = subdirs
CONFIG += ordered
SUBDIRS   = \
    Utils \
    edncrypt \
    shared

for(l, SUBDIRS) {
    QTC_LIB_DEPENDS =
    include($$l/$${l}_dependencies.pri)
    lv = $${l}.depends
    $$lv = $$QTC_LIB_DEPENDS
}



