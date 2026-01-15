#ifndef BLOCK_H
#define BLOCK_H

#include <QString>
#include <QDateTime>

class Block
{
public:
    Block(int index,
          const QString &batchId,
          const QString &material,
          int quantity,
          const QString &previousHash);

    QString calculateHash() const;

    int index;
    QString batchId;
    QString material;
    int quantity;
    QDateTime timestamp;
    QString previousHash;
    QString hash;
};

#endif // BLOCK_H
