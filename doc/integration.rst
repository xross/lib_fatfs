FatFs integration
=================

Build integration
-----------------

Add ``lib_fatfs`` to the application's dependent modules; xcommon-cmake handles
dependency setup. Include ``ff.h`` for the filesystem API and ``diskio.h`` for
the disk interface.

The application supplies ``disk_initialize``, ``disk_status``, ``disk_read``,
``disk_write`` and ``disk_ioctl``, together with ``get_fattime`` for timestamps.
The block driver must implement the operations used by the application; exposing
write and formatting APIs in the library does not supply a writable block driver.

Configuration
-------------

Configuration is maintained in ``lib_fatfs/api/ffconf.h``. The default
configuration supports eight volume IDs, 512-byte sectors, FAT12/16/32,
short (8.3) filenames and code page 437. exFAT and 64-bit sector addressing
are disabled. Operation is non-reentrant; access to a volume must be serialised
by the application.

See the `upstream documentation <../third_party/FatFs/documents/index.html>`_
for the current API, configuration options and release history.

Native regression test
----------------------

From the repository root, run with a native compiler supporting AddressSanitizer
and UndefinedBehaviorSanitizer::

    cc -std=c99 -g -fsanitize=address,undefined -Ilib_fatfs/api \
       tests/file_sequence.c lib_fatfs/src/ff_wrapper.c -o /tmp/fatfs-file-sequence
    /tmp/fatfs-file-sequence

The test formats an in-memory disk and runs 100 cycles reusing one file object
to read alternating files across sector and cluster boundaries. It checks
empty and missing files, buffer guards, and recovery after an injected disk-read
failure. It does not access physical storage or exercise the USB driver or JPEG
decoder.
