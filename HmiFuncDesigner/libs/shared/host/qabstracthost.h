#ifndef QABSTRACTHOST_H
#define QABSTRACTHOST_H

#include "../sharedlibglobal.h"

#include <QObject>
#include <QMap>
#include <QList>
#include <QAction>
#include <QTimer>
#include <QUndoStack>

#define OBJECT_TITLE   "Object"
#define  HOST_TYPE      "type"

class QAbstractProperty;
class XMLObject;
class QLanguageManager;
class QPageManager;
class QScriptEngine;
class QDataManager;
class QResourceManager;

class SHAREDLIB_EXPORT QAbstractHost : public QObject
{
    Q_OBJECT
public:
    explicit QAbstractHost(QAbstractHost *parent = 0);
    ~QAbstractHost();

    void insertChildren(const QList<int> &indexs,const QList<QAbstractHost*> &children);
    void removeChildren(const QList<QAbstractHost*> &children);

    void setParent(QAbstractHost* parent,int index);

    QList<QAbstractHost*> getChildren();
    QAbstractHost* getChild(int index);
    QAbstractHost* getParent();
    int getChildCount();

    QList<QAbstractProperty*>   get_propertys();
    QAbstractProperty*          get_property(const QString &name);

    QVariant                    get_property_value(const QString &name);
    void                        set_property_value(const QString &name,const QVariant &value);

    void                        set_default();

    void                    set_attribute(const QString& key,const QString& value);
    QString                 get_attribute(const QString& key);

    QList<QAction*>         get_actions();
    QAction*                getAction(const QString& name);

    QObject*                get_object();

    void                    to_object(XMLObject* xml);
    void                    from_object(XMLObject* xml);

    void                    clear();

    void                    init();

    virtual void            create_object();

    QString                 get_uuid();
    void                    set_uuid(const QString& uuid);

    void                    set_language_manager(QLanguageManager* language);
    QLanguageManager*       get_language_manager();

    void                    set_page_manager(QPageManager* page);

    void                    set_data_manager(QDataManager* data);

    void                    set_resource_manager(QResourceManager* resource);

    virtual QString         functionInformation(const QString &name);

    QScriptEngine           *get_script_engine();

    static QString          get_show_name();
    static QString          get_show_icon();
    static QString          get_show_group();

    void                    make_stylesheet();
    QString                 get_host_type();
protected:
    void                    insert_property(QAbstractProperty* property,int index=-1);
    void                    remove_property(const QString &name);

    void                    insertAction(const QString& name,QAction* ac,int index=-1);
    void                    remove_action(const QString& name);

    virtual   void          init_property();
    bool                    eventFilter(QObject *, QEvent *);

    void                    exec(const QString &code,const QMap<QString,QString> &param);
protected:
    virtual   bool          handle_paint_event(QPaintEvent* event);
    virtual   bool          handle_focus_in_event(QFocusEvent *event);
    virtual   bool          handle_focus_out_event(QFocusEvent *event);
    virtual   bool          handle_mouse_release_event(QMouseEvent* event);
    virtual   bool          handle_mouse_press_event(QMouseEvent* event);
    virtual   bool          handle_mouse_move_event(QMouseEvent* event);
    virtual   bool          handle_double_clicked_event(QMouseEvent* event);
    virtual   bool          handle_context_menu_event(QContextMenuEvent *event);

protected slots:
    void                    property_refresh();
    void                    current_language_changed();
    void                    current_text_changed(const QString &uuid);
signals:
    void                    show_widget(QWidget* wid);
    void                    show_dialog(QAbstractHost* host);

    void                    insertChildren_signal(const QList<QAbstractHost*> &host,const QList<int> &index);
    void                    removeChildren_signal(const QList<QAbstractHost*> &host);
    void                    parent_changed();
public slots:
    
protected:
    QList<QAbstractHost*>               m_children;
    QAbstractHost*                      m_parent;
    QList<QAbstractProperty*>           m_propertys;
    QMap<QString,QAbstractProperty*>    m_nameToProperty;
    QList<QAction*>                     m_actions;
    QMap<QString,QAction*>              m_nameToAction;

    QObject*                            m_object;

    QMap<QString,QString>               m_attributes;
    QTimer                              *m_timer;
    QLanguageManager                    *m_language_manager;
    QPageManager                        *m_page_manager;
    QDataManager                        *m_data_manager;
    QScriptEngine                       *m_engine;
    QResourceManager                    *m_resource_manager;
};

#endif // QABSTRACTHOST_H
