#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <QVector>
#include "block.h"

class Blockchain
{
public:
    Blockchain();

    void addBlock(const QString &batchId,
                  const QString &material,
                  int quantity);

    bool isChainValid() const;

    const QVector<Block>& getChain() const;

private:
    QVector<Block> chain;

    Block createGenesisBlock();
};

#endif // BLOCKCHAIN_H
