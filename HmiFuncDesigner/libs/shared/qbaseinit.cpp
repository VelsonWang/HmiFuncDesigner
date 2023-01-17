#include "qbaseinit.h"
#include "property/qintproperty.h"
#include "property/qstringproperty.h"
#include "property/qsizeproperty.h"
#include "property/qpointproperty.h"
#include "property/qrectproperty.h"
#include "property/qscriptproperty.h"
#include "property/qtimeproperty.h"
#include "property/qdatetimeproperty.h"
#include "property/qdateproperty.h"
#include "property/qenumproperty.h"
#include "property/qtagproperty.h"
#include "property/qcursorproperty.h"
#include "property/qfloatproperty.h"
#include "property/qfontproperty.h"
#include "property/qmemerydataproperty.h"
#include "property/qbytearrayproperty.h"
#include "property/qboolproperty.h"
#include "property/qalignmentproperty.h"
#include "property/qstylesheetproperty.h"
#include "property/qfileproperty.h"
#include "property/qimageproperty.h"
#include "property/qcolorproperty.h"
#include "property/qtagcolorlistproperty.h"
#include "property/qtagtextlistproperty.h"
#include "property/qfunctionproperty.h"
#include "qpropertyfactory.h"
#include "host/qwidgethost.h"
#include "host/qframehost.h"
#include "host/qlabelhost.h"
#include "host/qimagehost.h"
#include "host/qindicationlamphost.h"
#include "host/qjarshapehost.h"
#include "host/qvaluestickhost.h"
#include "host/qswitchbuttonhost.h"
#include "host/qlcdnumberhost.h"
#include "host/qpushbuttonhost.h"
#include "host/qradiobuttonhost.h"
#include "host/qcheckboxhost.h"
#include "host/qlineedithost.h"
#include "host/qdialhost.h"
#include "host/qsliderhost.h"
#include "host/qdoublespinboxhost.h"
#include "host/qspinboxhost.h"
#include "host/qdateedithost.h"
#include "host/qdatetimeedithost.h"
#include "host/qtimeedithost.h"
#include "host/qscrollbarhost.h"
#include "host/qprogressbarhost.h"
#include "host/qcomboboxhost.h"
#include "host/qthermometerhost.h"
#include "host/qtankhost.h"
#include "qhostfactory.h"


void qRegisterBaseProperty()
{
    qRegisterProperty("Number", QIntProperty);
    qRegisterProperty("String", QStringProperty);
    qRegisterProperty("Size", QSizeProperty);
    qRegisterProperty("Rect", QRectProperty);
    qRegisterProperty("Point", QPointProperty);
    qRegisterProperty("Font", QFontProperty);
    qRegisterProperty("Float", QFloatProperty);
    qRegisterProperty("Memery", QMemeryDataProperty);
    qRegisterProperty("Bool", QBoolProperty);
    qRegisterProperty("ByteArray", QByteArrayProperty);
    qRegisterProperty("Enum", QEnumProperty);
    qRegisterProperty("Script", QScriptProperty);
    qRegisterProperty("Cursor", QCursorProperty);
    qRegisterProperty("Time", QTimeProperty);
    qRegisterProperty("Date", QDateProperty);
    qRegisterProperty("DateTime", QDateTimeProperty);
    qRegisterProperty("Alignment", QAlignmentProperty);
    qRegisterProperty("StyleSheet", QStylesheetProperty);
    qRegisterProperty("Tag", QTagProperty);
    qRegisterProperty("File", QFileProperty);
    qRegisterProperty("Image", QImageProperty);
    qRegisterProperty("Color", QColorProperty);
    qRegisterProperty("TagColorList", QTagColorListProperty);
    qRegisterProperty("TagTextList", QTagTextListProperty);
    qRegisterProperty("Function", QFunctionProperty);
}

void qRegisterBaseHost()
{
    //    qRegisterHost("containners.widget", QWidgetHost);
    qRegisterHost("containners.frame", QFrameHost);
    qRegisterHost("display_widgets.label", QLabelHost);
    qRegisterHost("display_widgets.image", QImageHost);
    qRegisterHost("display_widgets.indicationlamp", QIndicationLampHost);
    qRegisterHost("display_widgets.qjarshape", QJarShapeHost);
    qRegisterHost("display_widgets.qvaluestick", QValueStickHost);
    qRegisterHost("displaywidgets.qthermometer", QThermometerHost);
    qRegisterHost("displaywidgets.qtank", QTankHost);
    //    qRegisterHost("display_widgets.lcdnumber", QLCDNumberHost);
    qRegisterHost("buttons.pushbutton", QPushButtonHost);
    qRegisterHost("buttons.switchbutton", QSwitchButtonHost);
    //    qRegisterHost("buttons.checkbox", QCheckBoxHost);
    //    qRegisterHost("buttons.radiobutton", QRadioButtonHost);
    qRegisterHost("inputwidgets.lineedit", QLineEditHost);
    //    qRegisterHost("inputwidgets.dial", QDialHost);
    //    qRegisterHost("inputwidgets.slider", QSliderHost);
    //    qRegisterHost("inputwidgets.spinbox", QSpinBoxHost);
    //    qRegisterHost("inputwidgets.doublespinbox", QDoubleSpinBoxHost);
    //    qRegisterHost("inputwidgets.datetimeedit", QDateTimeEditHost);
    //    qRegisterHost("inputwidgets.dateedit", QDateEditHost);
    //    qRegisterHost("inputwidgets.timeedit", QTimeEditHost);
    //    qRegisterHost("inputwidgets.scrollbar", QScrollBarHost);
    //    qRegisterHost("inputwidgets.combobox", QComboBoxHost);
    //   qRegisterHost("displaywidgets.progressbar", QProgressBarHost);

}

