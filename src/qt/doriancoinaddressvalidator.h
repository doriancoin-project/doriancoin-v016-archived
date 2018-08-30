// Copyright (c) 2011-2014 The Doriancoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DORIANCOIN_QT_DORIANCOINADDRESSVALIDATOR_H
#define DORIANCOIN_QT_DORIANCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class DoriancoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit DoriancoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Doriancoin address widget validator, checks for a valid doriancoin address.
 */
class DoriancoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit DoriancoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // DORIANCOIN_QT_DORIANCOINADDRESSVALIDATOR_H
