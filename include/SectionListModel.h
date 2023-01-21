#ifndef SIMPLEAUDIOPLAYER_SECTIONLISTMODEL_H_
#define SIMPLEAUDIOPLAYER_SECTIONLISTMODEL_H_

#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>

/*
This model define the element inside the side panel.
*/
class SectionListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    SectionListModel(QObject* parent = nullptr);
    ~SectionListModel();

    enum SectionType
    {
        FILE_SYSTEM,
        ALBUMS,
        PLAYLIST,
    };
    Q_ENUM(SectionType);

    enum Roles
    {
        NAME = Qt::UserRole+1,
        TYPE
    };

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = NAME) const override;
    /*
    Return the roles in a QHash object.
    This is useful to access the data using the role name in qml.
    */
    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    /*
    Get a specific index.
    This is useful to access the data from qml.
    */
    QVariantMap get(int index) const;

private:
    // Private nested class to store the name and type of one item.
    class SectionListType
    {
    public:
        SectionListType();
        SectionListType(const QString& name, int type);

        const QString& name() const;
        void setName(const QString& name);

        int type() const;
        void setType(int type);

    private:
        QString m_name;
        int m_type;
    };
    QList<SectionListType> m_data;
};

#endif // SIMPLEAUDIOPLAYER_SECTIONLISTMODEL_H_