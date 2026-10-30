:orphan:

#################################
lib_fatfs: FAT filesystem library
#################################

:vendor: xross
:version: 0.0.1
:scope: General Use
:description: FatFs FAT filesystem integration for XCORE
:category: General purpose
:keywords: filesystem, FAT, FatFs, storage
:devices: xcore.ai

*******
Summary
*******

This library integrates ChaN's FatFs filesystem module with XMOS xcore projects.
It provides file and directory access on block storage through application-supplied
disk I/O callbacks. The current integration uses patched FatFs R0.16.

********
Features
********

* FAT12, FAT16 and FAT32 filesystem support.
* File and directory operations, including read, write and formatting APIs.
* Hardware-independent disk interface for application-specific storage drivers.
* XMOS configuration maintained separately from the upstream source.
* CMake and legacy Make build integration.

************
Known issues
************

* None

****************
Development repo
****************

* `lib_fatfs <https://github.com/xross/lib_fatfs>`_

**************
Required tools
**************

* XMOS XTC Tools 15.3.1

*********************************
Required libraries (dependencies)
*********************************

* None

****************
Third-party code
****************

This library uses FatFs.

* Original project: `FatFs <https://elm-chan.org/fsw/ff/00index_e.html>`_.
* Upstream Git mirror: `abbrev/fatfs <https://github.com/abbrev/fatfs>`_.
* XMOS fork: `xmosforks/FatFs <https://github.com/xmosforks/FatFs>`_.
* Upstream version: ``R0.16`` with upstream patches through patch 3.
* XMOS downstream revision: ``R0.16+xmos.1``.
* Location: ``third_party/FatFs``.

*************************
Related application notes
*************************

* None

*******
Support
*******

Report issues with this integration in the
`lib_fatfs issue tracker <https://github.com/xross/lib_fatfs/issues>`_.
For upstream filesystem questions and bug reports, see the
`FatFs User Forum <https://elm-chan.org/fsw/ff/bd/>`_.
