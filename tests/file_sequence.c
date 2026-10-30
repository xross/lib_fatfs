/* Native regression test for the R0.16 port: no USB device or disk is modified. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "ff.h"
#include "diskio.h"

#define SECTORS 8192
static BYTE disk[SECTORS][512];
static DSTATUS state = STA_NOINIT;
static int fail_read;

DSTATUS disk_initialize(BYTE drive)
{
    return drive == 0 ? (state = 0) : STA_NOINIT;
}

DSTATUS disk_status(BYTE drive)
{
    return drive == 0 ? state : STA_NOINIT;
}

DRESULT disk_read(BYTE drive, BYTE *buffer, LBA_t sector, UINT count)
{
    if (drive || !count || sector >= SECTORS || count > SECTORS - sector)
        return RES_PARERR;
    if (fail_read) return RES_ERROR;
    memcpy(buffer, disk[sector], count * 512);
    return RES_OK;
}

DRESULT disk_write(BYTE drive, const BYTE *buffer, LBA_t sector, UINT count)
{
    if (drive || !count || sector >= SECTORS || count > SECTORS - sector)
        return RES_PARERR;
    memcpy(disk[sector], buffer, count * 512);
    return RES_OK;
}

DRESULT disk_ioctl(BYTE drive, BYTE command, void *buffer)
{
    if (drive) return RES_PARERR;
    switch (command) {
    case CTRL_SYNC: return RES_OK;
    case GET_SECTOR_COUNT: *(LBA_t *)buffer = SECTORS; return RES_OK;
    case GET_SECTOR_SIZE: *(WORD *)buffer = 512; return RES_OK;
    case GET_BLOCK_SIZE: *(DWORD *)buffer = 1; return RES_OK;
    default: return RES_PARERR;
    }
}

DWORD get_fattime(void)
{
    return ((DWORD)(2026 - 1980) << 25) | (1 << 21) | (1 << 16);
}

static BYTE pattern(unsigned position, unsigned seed)
{
    return (BYTE)(position * 37 + seed);
}

static void create_file(FIL *file, const char *name, unsigned size, unsigned seed)
{
    BYTE buffer[701];
    UINT written;
    assert(f_open(file, name, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK);
    for (unsigned offset = 0; offset < size;) {
        UINT count = size - offset < sizeof(buffer) ? size - offset : sizeof(buffer);
        for (UINT i = 0; i < count; ++i) buffer[i] = pattern(offset + i, seed);
        assert(f_write(file, buffer, count, &written) == FR_OK);
        assert(written == count);
        offset += count;
    }
    assert(f_close(file) == FR_OK);
}

static void check_file(FIL *file, const char *name, unsigned size, unsigned seed)
{
    struct {
        BYTE before[16];
        BYTE data[1024];
        BYTE after[16];
    } buffer;
    UINT count;
    assert(f_open(file, name, FA_READ) == FR_OK);
    assert(f_size(file) == size);
    unsigned offset = 0;
    do {
        memset(&buffer, 0xa5, sizeof(buffer));
        assert(f_read(file, buffer.data, sizeof(buffer.data), &count) == FR_OK);
        for (unsigned i = 0; i < sizeof(buffer.before); ++i) {
            assert(buffer.before[i] == 0xa5);
            assert(buffer.after[i] == 0xa5);
        }
        for (UINT i = 0; i < count; ++i)
            assert(buffer.data[i] == pattern(offset + i, seed));
        offset += count;
    } while (count);
    assert(offset == size);
    assert(f_eof(file));
    assert(f_close(file) == FR_OK);
}

int main(void)
{
    FATFS fs;
    FIL file;
    BYTE work[4096];
    MKFS_PARM options = {FM_FAT | FM_SFD, 0, 0, 0, 0};
    assert(f_mkfs("0:", &options, work, sizeof(work)) == FR_OK);
    assert(f_mount(&fs, "0:", 1) == FR_OK);
    create_file(&file, "TEST0.JPG", 8180, 17);
    create_file(&file, "TEST.TXT", 4097, 23);
    create_file(&file, "EMPTY.TXT", 0, 0);
    assert(f_unmount("0:") == FR_OK);
    assert(f_mount(&fs, "0:", 1) == FR_OK);
    for (unsigned repeat = 0; repeat < 100; ++repeat) {
        check_file(&file, "TEST0.JPG", 8180, 17);
        check_file(&file, "TEST.TXT", 4097, 23);
        check_file(&file, "EMPTY.TXT", 0, 0);
        assert(f_open(&file, "MISSING.TXT", FA_READ) == FR_NO_FILE);
    }
    assert(f_unmount("0:") == FR_OK);
    fail_read = 1;
    assert(f_mount(&fs, "0:", 1) == FR_DISK_ERR);
    fail_read = 0;
    assert(f_mount(&fs, "0:", 1) == FR_OK);
    check_file(&file, "TEST.TXT", 4097, 23);
    assert(f_unmount("0:") == FR_OK);
    puts("PASS: 100 JPEG/TXT/empty/missing-file cycles, buffer guards, read-error recovery");
    return 0;
}
