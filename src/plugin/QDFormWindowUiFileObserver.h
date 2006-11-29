
#ifndef THEKLA_QDFORMWINDOWUIFILEOBSERVER_H
#define THEKLA_QDFORMWINDOWUIFILEOBSERVER_H

#include <QObject>
#include <QTimer>

class Log;

//--------------------------------------------------------------------------------
class QDFormWindowUiFileObserver : public QObject
{

    Q_OBJECT

private:
    static Log cLog;

public:

    static const int DEFAULT_TIMER_INTERVAL_MILLIS = 500;

private:

    QString uiFileName_;
    time_t uiFileModifyTimePrev_;
    QTimer timer_;

public:

    QDFormWindowUiFileObserver(QObject * parent = NULL);
    ~QDFormWindowUiFileObserver();

public:

    void start(const QString & uiFileName);

private slots:

    /**
     * Connected to QTimer::timeout(). Triggers the save of the THEKLA settings if the UI
     * file was previously saved. This check is performed based on the modification time
     * of the ui-file.
     *
     * @see QTimer::timeout(), uiFileLastModified_, saveSettingsToFile()
     */
    void checkUiFile();

public slots:

    void setUiFileName(const QString & uiFileName);

signals:

    void uiFileModified(const QString & uiFileName);

};

#endif // THEKLA_QDFORMWINDOWUIFILEOBSERVER_H
