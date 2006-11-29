
#ifndef THEKLA_QDWIDGET_H
#define THEKLA_QDWIDGET_H

#include <QWidget>
#include <QSize>

class Log;

//--------------------------------------------------------------------------------
/**
 * Absolutely dummy widget. Not used for anything except for NOT to be used.
 */
class QDWidget : public QWidget
{
    Q_OBJECT

private:
    static Log cLog;

public:
    QDWidget(QWidget *parent = NULL);
    virtual ~QDWidget()
        {};
public:

    QSize minimumSizeHint() const
        { return QSize(0, 0); };
    QSize sizeHint() const
        { return QSize(0, 0); };
protected:
    /**
     * Something like a "REMOVE ME" or "DO NOT USE ME" icon.
     */
//     void paintEvent(QPaintEvent *event)
//         {
//             Q_UNUSED(event);
//         };
};

#endif // THEKLA_QDWIDGET_H
