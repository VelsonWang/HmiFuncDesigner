
include(../HmiFuncDesigner.pri)

TEMPLATE  = subdirs

SUBDIRS   = \
    Utils \
    edncrypt \
    Csv \
    qtpropertybrowser \
    qscintilla \
#    Lua \
#    Ftp \
    ProjData \
    Core




for(l, SUBDIRS) {
    QTC_LIB_DEPENDS =
    include($$l/$${l}_dependencies.pri)
    lv = $${l}.depends
    $$lv = $$QTC_LIB_DEPENDS
}



