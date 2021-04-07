#include "qbaseinit.h"
#include "../shared/qpropertyfactory.h"
#include "qpropertylist/qbytearrayeditor.h"
#include "qpropertylist/qenumpropertyeditor.h"
#include "qpropertylist/qnumbereditor.h"
#include "qpropertylist/qcommoneditor.h"
#include "qpropertylist/qbooleditor.h"
#include "qpropertylist/qfonteditor.h"
#include "qpropertylist/qstringeditor.h"
#include "qpropertylist/qfloateditor.h"
#include "qpropertylist/qscripteditor.h"
#include "qpropertylist/qfileeditor.h"
#include "qpropertylist/stylesheeteditor/qstylesheeteditor.h"
#include <QSpinBox>
#include <QDoubleSpinBox>

void qRegisterBasePropertyEditor()
{
    qRegisterPropertyEditor("Number", QNumberEditor);
    qRegisterPropertyEditor("String", QStringEditor);
    qRegisterPropertyEditor("Size", QCommonEditor);
    qRegisterPropertyEditor("Rect", QCommonEditor);
    qRegisterPropertyEditor("Point", QCommonEditor);
    qRegisterPropertyEditor("Font", QFontEditor);
    qRegisterPropertyEditor("Float", QFloatEditor);
    //qRegisterPropertyEditor("Memery", QMemeryDataProperty);
    qRegisterPropertyEditor("Bool", QBoolEditor);
    qRegisterPropertyEditor("ByteArray", QByteArrayEditor);
    qRegisterPropertyEditor("Enum", QEnumPropertyEditor);
    qRegisterPropertyEditor("Script", QScriptEditor);
    qRegisterPropertyEditor("Cursor", QEnumPropertyEditor);
    qRegisterPropertyEditor("StyleSheet", QStyleSheetEditor);
    //qRegisterPropertyEditor("Time", QTimeProperty);
    //qRegisterPropertyEditor("Date", QDateProperty);
    //qRegisterPropertyEditor("DateTime", QDateTimeProperty);
    qRegisterPropertyEditor("Tag", QEnumPropertyEditor);
    qRegisterPropertyEditor("File", QFileEditor);
}

