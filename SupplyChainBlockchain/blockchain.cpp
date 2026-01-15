#include "blockchain.h"

Blockchain::Blockchain()
{
    chain.append(createGenesisBlock());
}

Block Blockchain::createGenesisBlock()
{
    return Block(0, "GENESIS", "N/A", 0, "0");
}

void Blockchain::addBlock(const QString &batchId,
                          const QString &material,
                          int quantity)
{
    const Block &previousBlock = chain.last();
    Block newBlock(chain.size(),
                   batchId,
                   material,
                   quantity,
                   previousBlock.hash);

    chain.append(newBlock);
}

bool Blockchain::isChainValid() const
{
    for (int i = 1; i < chain.size(); ++i)
    {
        const Block &current = chain[i];
        const Block &previous = chain[i - 1];

        if (current.hash != current.calculateHash())
            return false;

        if (current.previousHash != previous.hash)
            return false;
    }
    return true;
}

const QVector<Block>& Blockchain::getChain() const
{
    return chain;
}
