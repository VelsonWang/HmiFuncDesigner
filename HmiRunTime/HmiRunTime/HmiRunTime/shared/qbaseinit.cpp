#include "qbaseinit.h"
#include "qwidgetfactory.h"
#include "widgets/qformwidget.h"
#include "widgets/qimagebox.h"
#include "widgets/qindicationlamp.h"
#include "widgets/qjarshape.h"
#include "widgets/qlabelex.h"
#include "widgets/qlineeditex.h"
#include "widgets/qpushbuttonex.h"
#include "widgets/qswitchbutton.h"
#include "widgets/qvaluestick.h"
#include "widgets/qtank.h"
#include "widgets/qthermometer.h"
//#include "property/qintproperty.h"
//#include "property/qstringproperty.h"
//#include "property/qsizeproperty.h"
//#include "property/qpointproperty.h"
//#include "property/qrectproperty.h"
//#include "property/qscriptproperty.h"
//#include "property/qtimeproperty.h"
//#include "property/qdatetimeproperty.h"
//#include "property/qdateproperty.h"
//#include "property/qenumproperty.h"
//#include "property/qtagproperty.h"
//#include "property/qcursorproperty.h"
//#include "property/qfloatproperty.h"
//#include "property/qfontproperty.h"
//#include "property/qmemerydataproperty.h"
//#include "property/qbytearrayproperty.h"
//#include "property/qboolproperty.h"
//#include "property/qalignmentproperty.h"
//#include "property/qstylesheetproperty.h"
//#include "property/qfileproperty.h"
//#include "property/qimageproperty.h"
//#include "property/qcolorproperty.h"
//#include "property/qtagcolorlistproperty.h"
//#include "property/qtagtextlistproperty.h"
//#include "property/qfunctionproperty.h"
//#include "qpropertyfactory.h"
//#include "host/qwidgethost.h"
//#include "host/qframehost.h"
//#include "host/qlabelhost.h"
//#include "host/qimagehost.h"
//#include "host/qindicationlamphost.h"
//#include "host/qjarshapehost.h"
//#include "host/qvaluestickhost.h"
//#include "host/qswitchbuttonhost.h"
//#include "host/qlcdnumberhost.h"
//#include "host/qpushbuttonhost.h"
//#include "host/qradiobuttonhost.h"
//#include "host/qcheckboxhost.h"
//#include "host/qlineedithost.h"
//#include "host/qdialhost.h"
//#include "host/qsliderhost.h"
//#include "host/qdoublespinboxhost.h"
//#include "host/qspinboxhost.h"
//#include "host/qdateedithost.h"
//#include "host/qdatetimeedithost.h"
//#include "host/qtimeedithost.h"
//#include "host/qscrollbarhost.h"
//#include "host/qprogressbarhost.h"
//#include "host/qcomboboxhost.h"


void qRegisterWidgets()
{
    //    qRegisterWidget("containners.widget", QWidgetHost);
    //    qRegisterWidget("containners.frame", QFrameHost);
    qRegisterWidget("display_widgets.label", QLabelEx);
    qRegisterWidget("display_widgets.image", QImageBox);
    qRegisterWidget("display_widgets.indicationlamp", QIndicationLamp);
    qRegisterWidget("display_widgets.qjarshape", QJarShape);
    qRegisterWidget("display_widgets.qvaluestick", QValueStick);
    //    qRegisterWidget("display_widgets.lcdnumber", QLCDNumberHost);
    qRegisterWidget("displaywidgets.qthermometer", QThermometer);
    qRegisterWidget("displaywidgets.qtank", QTank);
    qRegisterWidget("buttons.pushbutton", QPushButtonEx);
    qRegisterWidget("buttons.switchbutton", QSwitchButton);
    //    qRegisterWidget("buttons.checkbox", QCheckBoxHost);
    //    qRegisterWidget("buttons.radiobutton", QRadioButtonHost);
    qRegisterWidget("inputwidgets.lineedit", QLineEditEx);
    //    qRegisterWidget("inputwidgets.dial", QDialHost);
    //    qRegisterWidget("inputwidgets.slider", QSliderHost);
    //    qRegisterWidget("inputwidgets.spinbox", QSpinBoxHost);
    //    qRegisterWidget("inputwidgets.doublespinbox", QDoubleSpinBoxHost);
    //    qRegisterWidget("inputwidgets.datetimeedit", QDateTimeEditHost);
    //    qRegisterWidget("inputwidgets.dateedit", QDateEditHost);
    //    qRegisterWidget("inputwidgets.timeedit", QTimeEditHost);
    //    qRegisterWidget("inputwidgets.scrollbar", QScrollBarHost);
    //    qRegisterWidget("inputwidgets.combobox", QComboBoxHost);
    //    qRegisterWidget("displaywidgets.progressbar", QProgressBarHost);
}


