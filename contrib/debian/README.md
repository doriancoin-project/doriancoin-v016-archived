
Debian
====================
This directory contains files used to package doriancoind/doriancoin-qt
for Debian-based Linux systems. If you compile doriancoind/doriancoin-qt yourself, there are some useful files here.

## doriancoin: URI support ##


doriancoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install doriancoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your doriancoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/doriancoin128.png` to `/usr/share/pixmaps`

doriancoin-qt.protocol (KDE)

