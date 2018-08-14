// Copyright (c) 2009-2016 The Doriancoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DORIANCOIN_QT_TEST_COMPATTESTS_H
#define DORIANCOIN_QT_TEST_COMPATTESTS_H

#include <QObject>
#include <QTest>

class CompatTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void bswapTests();
};

#endif // DORIANCOIN_QT_TEST_COMPATTESTS_H
