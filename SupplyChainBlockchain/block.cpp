#include "block.h"
#include <QCryptographicHash>

Block::Block(int index,
             const QString &batchId,
             const QString &material,
             int quantity,
             const QString &previousHash)
    : index(index),
    batchId(batchId),
    material(material),
    quantity(quantity),
    previousHash(previousHash)
{
    timestamp = QDateTime::currentDateTime();
    hash = calculateHash();
}

QString Block::calculateHash() const
{
    QString data = QString::number(index)
    + batchId
        + material
        + QString::number(quantity)
        + timestamp.toString(Qt::ISODate)
        + previousHash;

    QByteArray hashBytes =
        QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Sha256);

    return hashBytes.toHex();
}
