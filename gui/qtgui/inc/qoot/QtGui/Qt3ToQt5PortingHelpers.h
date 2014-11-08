#pragma once

#include <QVector>
#include <QHash>
#include <QStack>
#include <QTreeWidgetItem>
#include <QScrollArea>
#include <QTreeWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QStackedWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QMimeData>
#include <QFileDialog>

namespace Qt
{
    const auto WStyle_NoBorder = Qt::FramelessWindowHint;
    const auto WStyle_StaysOnTop = Qt::WindowStaysOnTopHint;
    const auto WDestructiveClose = Qt::WA_DeleteOnClose;
    const auto WType_TopLevel = Qt::Window;
}

template <class T>
class Q3PtrVector : public QVector<T*>
{
    // TODO: There numerous differences between Q3PtrVector and QVector that may need addressing.
public:
    T* take(int i)
    {
        return nullptr;
    }
};

template <class T>
class Q3IntDict : public QHash<int, T*>
{
    // TODO: check differences between Q3IntDict and QHash
public:
};

class Q3Icon : public QIcon
{
public:
    enum Size { Small, Large, Automatic };
};

class Q3ScrollView : public QScrollArea
{
public:
    void setUpdatesEnabled(bool b)
    {
    }

    QWidget* viewport() const
    {
        return nullptr;
    }


    void repaintContents(bool erase = true)
    {
    }
};

template <class T>
class Q3PtrStack : public QStack<T*>
{
public:
    // TODO: There numerous differences between Q3PtrVector and QVector that may need addressing.

    void setAutoDelete(bool b)
    {
    }
};

class Q3ListViewItem;

class Q3ListView : public QTreeWidget
{
public:
    Q3ListView()
    {
    }

    Q3ListView(QObject* pParent, QString sName, Qt::WidgetAttribute)
    {
    }

    Q3ListViewItem* firstChild() const
    {
        return nullptr;
    }

    void addColumn(const QString& label, int width = -1)
    {
    }
};

class Q3ListViewItem : public QTreeWidgetItem
{
public:
    Q3ListViewItem()
    {
    }

    Q3ListViewItem(Q3ListView* parent, QString)
    {
    }

    Q3ListViewItem(Q3ListViewItem* parent, QString)
    {
    }

    Q3ListViewItem(Q3ListViewItem* parent, QString, QString)
    {
    }

    Q3ListViewItem(Q3ListView* parent, Q3ListViewItem * after)
    {
    }

    Q3ListViewItem(Q3ListViewItem* parent, Q3ListViewItem * after)
    {
    }

    Q3ListViewItem(Q3ListViewItem* parent, Q3ListViewItem * after, QString)
    {
    }

    Q3ListViewItem* parent() const
    {
        return nullptr;
    }

    Q3ListViewItem* firstChild() const
    {
        return nullptr;
    }

    Q3ListViewItem * nextSibling() const
    {
        return nullptr;
    }

    void setPixmap(int column, const QPixmap & icon)
    {
    }

    void setPixmap(const QPixmap & icon)
    {
    }

    const QPixmap* pixmap(int column) const
    {
        return nullptr;
    }

    int depth() const
    {
        return 0;
    }

    void setExpandable(bool b)
    {
    }

    void setSelectable(bool b)
    {
    }

    bool isOpen() const
    {
        return false;
    }

    void setOpen(bool b)
    {
    }

    void setEnabled(bool b)
    {
    }
};

class Q3CheckListItem : public Q3ListViewItem
{
public:

    Q3CheckListItem(Q3ListViewItem* parent, QString)
    {
    }

    Q3CheckListItem(Q3ListViewItem* parent, QString, int type)
    {
    }

    Q3CheckListItem(Q3ListView* parent, QString, int type)
    {
    }

    Q3CheckListItem(Q3ListView* parent, Q3ListViewItem * after, QString, int)
    {
    }

    Q3CheckListItem(Q3ListViewItem* parent, Q3ListViewItem * after, QString, int)
    {
    }

    Q3CheckListItem(Q3ListView* parent, QString, const QPixmap&)
    {
    }

    Q3CheckListItem(Q3ListViewItem* parent, QString, const QPixmap&)
    {
    }
};

class Q3IconViewItem;

class Q3IconView : public QListWidget
{
public:
    enum Arrangement { LeftToRight, TopToBottom };

    enum ItemTextPos { Bottom, Right };

    Q3IconView()
    {
    }

    Q3IconView(QWidget* pParent, QString)
    {
    }

    Q3IconViewItem* firstItem() const
    {
        return nullptr;
    }

    Q3IconViewItem* nextItem() const
    {
        return nullptr;
    }

    void setArrangement(Arrangement a)
    {
    }

    void setItemTextPos(ItemTextPos)
    {
    }

    void setWordWrapIconText(QString)
    {
    }

    void setShowToolTips(bool b)
    {
    }

    void setGridX(bool b)
    {
    }

    void setGridY(bool b)
    {
    }
};

class Q3IconViewItem : public QListWidgetItem
{
public:
    Q3IconViewItem() {}
    Q3IconViewItem(QObject* p, QString)
    {
    }

    Q3IconViewItem(Q3IconView* p, Q3IconViewItem* p2)
    {
    }

    void setPixmap(const QPixmap & icon)
    {
    }

    Q3IconViewItem* nextItem() const
    {
        return nullptr;
    }

    void setSelectable(bool b)
    {
    }
};

class Q3WidgetStack : public QStackedWidget
{
public:
    Q3WidgetStack()
    {
    }

    Q3WidgetStack(QWidget* pParent, QString, Qt::WidgetAttribute)
    {
    }
};

class Q3Table : public QTableWidget
{
public:
    Q3Table()
    {
    }

    Q3Table(QWidget* pParent, QString)
    {
    }

    void setNumRows(int rows)
    {
        setRowCount(rows);
    }

    void setNumCols(int cols)
    {
        setColumnCount(cols);
    }

    void setText(int r, int c, QString s)
    {
        setItem(r, c, new QTableWidgetItem(s));
    }

    void sortColumn(int col, bool bAscending, bool bWholeRows)
    {
    }
};

class Q3Header : public QHeaderView
{
public:
    void setLabel(int section, const QString & s, int size = -1)
    {
    }
};

// See "Porting to Qt 4 - Drag and Drop"
class Q3TextDrag : public QMimeData
{
public:
    // TODO
    static bool decode(QDropEvent*, QString)
    {
        return false;
    }
};

class Q3UriDrag : public QMimeData
{
public:
    static QString uriToLocalFile(QString)
    {
        return QString();
    }
};

// http://qt-project.org/doc/qt-4.8/porting4.html#qhbox
class Q3HBox : public QWidget
{
public:
    Q3HBox(QWidget* pParent = nullptr, QString sName = QString(), Qt::WindowFlags f = 0)
        : QWidget(pParent, f)
    {
        setObjectName(sName);
    }
};

class Q3FileDialog : public QFileDialog
{
public:
    static QString getSaveFileName(const QString& sDir,
        const QString& sFilter,
        QWidget* pWidget,
        const QString& s,
        const QString s2,
        QString* pFilter)
    {
        // TODO
        return QFileDialog::getSaveFileName(pWidget, s, sDir, QString() /*filter*/, pFilter, Options());
    }
};
