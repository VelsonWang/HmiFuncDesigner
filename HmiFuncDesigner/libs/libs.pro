include(../HmiFuncDesigner.pri)

TEMPLATE  = subdirs
CONFIG += ordered
SUBDIRS   = \
    Utils \
    edncrypt \
    Csv \
    qtpropertybrowser \
    qscintilla \
#    Lua \
#    Ftp \
    shared \
    gradienteditor \
    core \
    running


for(l, SUBDIRS) {
    QTC_LIB_DEPENDS =
    include($$l/$${l}_dependencies.pri)
    lv = $${l}.depends
    $$lv = $$QTC_LIB_DEPENDS
}



