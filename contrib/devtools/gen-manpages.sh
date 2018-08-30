#!/bin/bash

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

DORIANCOIND=${DORIANCOIND:-$SRCDIR/doriancoind}
DORIANCOINCLI=${DORIANCOINCLI:-$SRCDIR/doriancoin-cli}
DORIANCOINTX=${DORIANCOINTX:-$SRCDIR/doriancoin-tx}
DORIANCOINQT=${DORIANCOINQT:-$SRCDIR/qt/doriancoin-qt}

[ ! -x $DORIANCOIND ] && echo "$DORIANCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
DRCVER=($($DORIANCOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for doriancoind if --version-string is not set,
# but has different outcomes for doriancoin-qt and doriancoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$DORIANCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $DORIANCOIND $DORIANCOINCLI $DORIANCOINTX $DORIANCOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${DRCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${DRCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
